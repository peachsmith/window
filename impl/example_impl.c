#include "example.h"

// SDL is used for things like creating windows and handling input.
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>

// TEMP
static const char *tmp_msg = "Hello, World!";

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

// complete definition of the eg_impl type
struct eg_impl
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    const Uint8 *keystates;
    Uint64 ticks;
    Uint64 frame_len;
    TTF_Font *font;        // the default font for rendering text
    SDL_Texture *tmp_text; // temporary texture for testing font rendering
};

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

    // Create a surface.
    SDL_Color c = {.a = 255, .r = 250, .g = 250, .b = 250};
    SDL_Surface *s = TTF_RenderUTF8_Blended(font, "Hello, World!", c);
    if (s == NULL)
    {
        fprintf(stderr,
                "failed to create glyph surface. error: %s\n",
                TTF_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        free(impl);
        return NULL;
    }

    SDL_Texture *tmp_tex = SDL_CreateTextureFromSurface(renderer, s);
    SDL_FreeSurface(s);
    if (s == NULL)
    {
        fprintf(stderr,
                "failed to create glyph texture. error: %s\n",
                TTF_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        free(impl);
        return NULL;
    }
    impl->tmp_text = tmp_tex;

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

    SDL_DestroyTexture(impl->tmp_text);
    TTF_CloseFont(impl->font);
    SDL_DestroyRenderer(impl->renderer);
    SDL_DestroyWindow(impl->window);
    free(impl);
}

void eg_draw_text(eg_app *app)
{
    eg_impl *impl = app->impl;

    int w;
    int h;
    TTF_SizeUTF8(impl->font, tmp_msg, &w, &h);
    SDL_Rect r = {.x = 10, .y = 10, .w = w, .h = h};
    SDL_RenderCopy(impl->renderer, impl->tmp_text, NULL, &r);
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
