#include "example.h"

// SDL is used for things like creating windows and handling input.
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>

// keyboard functions defined in keyboard.c
SDL_Scancode eg_impl_get_sdl_scancode(eg_keycode k);
eg_keycode eg_impl_get_eg_keycode(SDL_Scancode sc);
void eg_impl_init_keyboard();

int eg_impl_init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(
            stderr,
            "failed to initialize SDL. error: %s\n",
            SDL_GetError());
        return 0;
    }

    if (TTF_Init() == -1)
    {
        fprintf(stderr,
                "failed to initialize SDL_ttf error: %s\n",
                TTF_GetError());
        return 0;
    }

    eg_impl_init_keyboard();

    return 1;
}

void eg_impl_term()
{
    TTF_Quit();
    SDL_Quit();
}

// maximum number of characters in a font atlas
#define FONT_ATLAS_MAX 128

typedef struct font_atlas
{
    SDL_Texture *chars[FONT_ATLAS_MAX];
    int count; // current number of characters in the atlas.
} font_atlas;

// complete definition of the eg_impl type
struct eg_impl
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    const Uint8 *keystates;
    Uint64 ticks;
    Uint64 frame_len;
    TTF_Font *font;   // the default font for rendering text
    font_atlas atlas; // a collection of textures for rendering text
};

static int init_font_atlas(SDL_Renderer *r, TTF_Font *font, font_atlas *atlas)
{
    for (int i = 0; i < FONT_ATLAS_MAX; i++)
    {
        atlas->chars[i] = NULL;
    }

    SDL_Color white = {.a = 255, .r = 255, .g = 255, .b = 255};
    for (int i = 32; i < 127 && i < FONT_ATLAS_MAX; i++)
    {
        const char c[2] = {(char)i, '\0'};

        // Create the SDL_Surface
        SDL_Surface *surface = TTF_RenderUTF8_Blended(font, c, white);
        if (surface == NULL)
        {
            return 0;
        }

        // Convert the SDL_Surface into an SDL_Texture.
        // Since we don't need the surface anymore, it should be destroyed
        // regardless of whether or not we succeeded in creating the texture.
        SDL_Texture *texture = SDL_CreateTextureFromSurface(r, surface);
        SDL_FreeSurface(surface);
        if (texture == NULL)
        {
            return 0;
        }

        atlas->chars[i] = texture;
        atlas->count++;
    }

    return 1;
}

static void destroy_font_atlas_textures(font_atlas *atlas)
{
    for (int i = 0; i < FONT_ATLAS_MAX; i++)
    {
        if (atlas->chars[i] != NULL)
        {
            SDL_DestroyTexture(atlas->chars[i]);
        }
    }
}

eg_impl *eg_impl_create(int screen_width, int screen_height)
{
    eg_impl *impl;
    SDL_Window *window;
    SDL_Renderer *renderer;
    const Uint8 *keystates;

    // Create the wrapper struct.
    impl = (eg_impl *)malloc(sizeof(struct eg_impl));
    if (impl == NULL)
    {
        return NULL;
    }

    // Create the window.
    window = SDL_CreateWindow(
        "Example",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screen_width,
        screen_height,
        SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        free(impl);
        return NULL;
    }

    // Create the renderer.
    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        SDL_DestroyWindow(window);
        free(impl);
        return NULL;
    }

    // Get the keyboard state.
    keystates = SDL_GetKeyboardState(NULL);
    if (keystates == NULL)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        free(impl);
        return NULL;
    }

    // Load a font.
    TTF_Font *font = TTF_OpenFont("../JetBrainsMonoNL-Regular.ttf", 14);
    if (font == NULL)
    {
        fprintf(stderr,
                "failed to load font. error: %s\n",
                TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        free(impl);
        return NULL;
    }

    if (!init_font_atlas(renderer, font, &(impl->atlas)))
    {
        fprintf(stderr,
                "failed to create font atlas\n");
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        free(impl);
        return NULL;
    }

    // Populate the fields of the wrapper struct.
    impl->window = window;
    impl->renderer = renderer;
    impl->keystates = keystates;
    impl->font = font;
    impl->frame_len = 16;
    impl->ticks = 0;

    // Notes on frame_len field:
    // Since the desired framerate is 60 frames per second, we set the
    // approximate milliseconds per frame to 16. This is the truncated result
    // of 1000 / 60.

    return impl;
}

void eg_impl_destroy(eg_impl *impl)
{
    if (impl == NULL)
    {
        return;
    }

    destroy_font_atlas_textures(&(impl->atlas));
    TTF_CloseFont(impl->font);
    SDL_DestroyRenderer(impl->renderer);
    SDL_DestroyWindow(impl->window);
    free(impl);
}

void eg_draw_text(eg_app *app, const char *msg)
{
    eg_impl *impl = app->impl;

    // text starting position
    int x = 10;
    int y = 10;

    // character dimensions
    int w;
    int h;

    for (int i = 0; msg[i] != '\0'; i++)
    {
        SDL_Texture *tex = impl->atlas.chars[(int)msg[i]];
        int q = SDL_QueryTexture(tex, NULL, NULL, &w, &h);
        if (!q)
        {
            SDL_Rect r = {.x = x, .y = y, .w = w, .h = h};
            x += w;
            SDL_RenderCopy(impl->renderer, tex, NULL, &r);
        }
    }
}

void eg_impl_process_events(eg_app *app)
{
    if (app == NULL || app->impl == NULL)
    {
        return;
    }

    eg_impl *impl = app->impl;

    // First, we get the current tick count for regulating framerate.
    impl->ticks = SDL_GetTicks64();

    // According to the wiki, it is common practice to process all events in
    // the event queue at the beginning of each iteration of the main loop.
    // The SDL_PollEvent function also calls SDL_PumpEvents, which updates the
    // keyboard state array.
    while (SDL_PollEvent(&(impl->event)))
    {
        // If the window was closed, exit the application.
        if (impl->event.type == SDL_QUIT)
        {
            app->done = 1;
        }

        // If a key was released, convert the SDL_Scancode into an eg_keycode
        // and set the corresponding key capture flag to 0.
        if (impl->event.type == SDL_KEYUP)
        {
            SDL_Scancode sc = impl->event.key.keysym.scancode;
            eg_keycode k = eg_impl_get_eg_keycode(sc);
            app->key_captures[k] = 0;
        }
    }
}

void eg_impl_delay(eg_app *app)
{
    if (app == NULL || app->impl == NULL)
    {
        return;
    }

    eg_impl *impl = app->impl;

    // Get the approximate number of milliseconds since the beginning of the
    // current iteration of the main loop.
    Uint64 elapsed = SDL_GetTicks64() - impl->ticks;

    // If the frame length is greater than the elapsed milliseconds since the
    // beginning of the frame, wait for the duration of the difference.
    if (impl->frame_len > elapsed)
    {
        // The SDL_Delay function expects a Uint32 as its argument.
        // Converting from Uint64 to Uint32 will truncate the value, but the
        // difference between the frame length and the elapsed milliseconds
        // should never be greater than UINT32_MAX.
        SDL_Delay((Uint32)(impl->frame_len - elapsed));
    }
}

void eg_impl_clear_screen(eg_app *app)
{
    if (app == NULL || app->impl == NULL)
    {
        return;
    }

    eg_impl *impl = app->impl;

    SDL_SetRenderDrawColor(impl->renderer, 0, 0, 0, 255);
    SDL_RenderClear(impl->renderer);
}

void eg_impl_render_screen(eg_app *app)
{
    if (app == NULL || app->impl == NULL)
    {
        return;
    }

    eg_impl *impl = app->impl;
    SDL_RenderPresent(impl->renderer);
}

// Public API function
// TODO: refactor this to eg_impl_set_color and wrap it
void eg_set_color(eg_app *app, uint32_t color)
{
    if (app == NULL || app->impl == NULL)
    {
        return;
    }

    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;

    b = (Uint8)(color & 0xFF);
    g = (Uint8)((color >> 8) & 0xFF);
    r = (Uint8)((color >> 16) & 0xFF);
    a = (Uint8)((color >> 24) & 0xFF);

    eg_impl *impl = app->impl;
    SDL_SetRenderDrawColor(impl->renderer, r, g, b, a);
}

// Public API function
// TODO: refactor this to eg_impl_draw_line and wrap it
void eg_draw_line(eg_app *app, eg_point *a, eg_point *b)
{
    if (app == NULL || app->impl == NULL)
    {
        return;
    }

    eg_impl *impl = app->impl;
    SDL_RenderDrawLine(
        impl->renderer,
        a->x,
        a->y,
        b->x,
        b->y);
}

// Public API function
// TODO: refactor this to eg_impl_draw_rect and wrap it
void eg_draw_rect(eg_app *app, eg_rect *r, int filled)
{
    if (app == NULL || app->impl == NULL)
    {
        return;
    }

    eg_impl *impl = app->impl;

    SDL_Rect sr = {
        .x = r->x,
        .y = r->y,
        .w = r->w,
        .h = r->h};

    if (filled)
    {
        SDL_RenderFillRect(impl->renderer, &sr);
        return;
    }

    SDL_RenderDrawRect(impl->renderer, &sr);
}

int eg_impl_peek_key(eg_app *app, int v)
{
    if (app == NULL || app->impl == NULL)
    {
        return 0;
    }

    eg_impl *impl = app->impl;

    SDL_Scancode sc = eg_impl_get_sdl_scancode(v);

    if (sc >= SDL_NUM_SCANCODES || sc <= SDL_SCANCODE_UNKNOWN)
    {
        return 0;
    }

    if (impl->keystates[sc])
    {
        return 1;
    }

    return 0;
}

int eg_impl_consume_key(eg_app *app, int v)
{
    if (app == NULL || app->impl == NULL)
    {
        return 0;
    }

    eg_impl *impl = app->impl;

    SDL_Scancode sc = eg_impl_get_sdl_scancode(v);

    if (sc >= SDL_NUM_SCANCODES || sc <= SDL_SCANCODE_UNKNOWN)
    {
        return 0;
    }

    if (app->key_captures[v])
    {
        return 0;
    }

    if (impl->keystates[sc])
    {
        app->key_captures[v] = 1;
        return 1;
    }

    return 0;
}
