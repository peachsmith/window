#include "example.h"
#include "impl.h"

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
        SDL_Quit();
        return 0;
    }

    if (!IMG_Init(IMG_INIT_PNG))
    {
        fprintf(stderr,
                "failed to initialize SDL_image error: %s\n",
                IMG_GetError());
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    eg_impl_init_keyboard();

    return 1;
}

void eg_impl_term()
{
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

static void destroy_font_atlas_textures(eg_font *atlas)
{
    for (int i = 0; i < FONT_ATLAS_MAX; i++)
    {
        if (atlas->glyphs[i] != NULL)
        {
            SDL_DestroyTexture(atlas->glyphs[i]);
        }
    }

    if (atlas->atlas != NULL)
    {
        SDL_DestroyTexture(atlas->atlas);
    }
}

static void destroy_images(eg_sprite_sheet *sheet)
{
    if (sheet->img != NULL)
    {
        SDL_DestroyTexture(sheet->img);
        sheet->img = NULL;
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

    // Zero out the font data.
    impl->font.atlas = NULL;
    for (int i = 0; i < FONT_ATLAS_MAX; i++)
    {
        impl->font.glyphs[i] = NULL;
    }

    // Zero out image data
    impl->sprite_sheet.img = NULL;

    // Create the window.
    // NOTE: we use SDL_WINDOW_ALLOW_HIGHDPI on macOS.
    window = SDL_CreateWindow(
        "Example",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screen_width,
        screen_height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
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

    // TEMP: get window and screen info for scaling on mac.
    int ww, wh;
    int rw, rh;
    SDL_GetWindowSize(window, &ww, &wh);
    SDL_GetRendererOutputSize(renderer, &rw, &rh);
    printf("[DEBUG] window: (%d x %d), renderer: (%d, %d)\n", ww, wh, rw, rh);
    if (rw > ww)
    {
        int scale_correction_x = rw / ww;
        int scale_correction_y = rh / wh;
        printf("[DEBUG] scale correction factor: (%d, %d)\n",
               scale_correction_x,
               scale_correction_y);
        SDL_RenderSetScale(renderer, (float)scale_correction_x, (float)scale_correction_y);
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

    // Populate the fields of the wrapper struct.
    impl->window = window;
    impl->renderer = renderer;
    impl->keystates = keystates;
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

    destroy_font_atlas_textures(&(impl->font));
    destroy_images(&(impl->sprite_sheet));
    SDL_DestroyRenderer(impl->renderer);
    SDL_DestroyWindow(impl->window);
    free(impl);
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

    // SDL_SetRenderDrawColor(impl->renderer, 0X04, 0X35, 0X8D, 255);
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

void eg_impl_set_color(eg_app *app, uint32_t color)
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

void eg_impl_draw_line(eg_app *app, eg_point *a, eg_point *b)
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

void eg_impl_draw_rect(eg_app *app, eg_rect *r, int filled)
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
