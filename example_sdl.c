#include "example.h"
#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>

// complete definition of the eg_impl type
struct eg_impl
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    const Uint8 *keystates;
    Uint64 ticks;
    Uint64 frame_len;
};

eg_impl *eg_create_impl()
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
        240,
        160,
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

void eg_destroy_impl(eg_impl *impl)
{
    if (impl == NULL)
    {
        return;
    }

    SDL_DestroyRenderer(impl->renderer);
    SDL_DestroyWindow(impl->window);
    free(impl);
}

void eg_process_events(eg_app *app)
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
        if (impl->event.type == SDL_QUIT)
        {
            app->done = 1;
        }

        // Clear the key press capture flags.
        if (impl->event.type == SDL_KEYUP)
        {
            app->key_captures[impl->event.key.keysym.scancode] = 0;
        }
    }
}

void eg_delay(eg_app *app)
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

void eg_clear_screen(eg_app *app)
{
    if (app == NULL || app->impl == NULL)
    {
        return;
    }

    eg_impl *impl = app->impl;

    SDL_SetRenderDrawColor(impl->renderer, 0, 0, 0, 255);
    SDL_RenderClear(impl->renderer);
}

void eg_render_screen(eg_app *app)
{
    if (app == NULL || app->impl == NULL)
    {
        return;
    }

    eg_impl *impl = app->impl;
    SDL_RenderPresent(impl->renderer);
}

void eg_set_color(eg_app *app, int r, int g, int b, int a)
{
    if (app == NULL || app->impl == NULL)
    {
        return;
    }

    eg_impl *impl = app->impl;
    SDL_SetRenderDrawColor(impl->renderer, r, g, b, a);
}

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

int eg_peek_key(eg_app *app, int v)
{
    if (app == NULL || app->impl == NULL)
    {
        return 0;
    }

    eg_impl *impl = app->impl;

    if (v >= SDL_NUM_SCANCODES)
    {
        return 0;
    }

    if (impl->keystates[v])
    {
        return 1;
    }

    return 0;
}

int eg_consume_key(eg_app *app, int v)
{
    if (app == NULL || app->impl == NULL)
    {
        return 0;
    }

    eg_impl *impl = app->impl;

    if (v >= SDL_NUM_SCANCODES)
    {
        return 0;
    }

    if (app->key_captures[v])
    {
        return 0;
    }

    if (impl->keystates[v])
    {
        app->key_captures[v] = 1;
        return 1;
    }

    return 0;
}
