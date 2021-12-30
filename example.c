#include "example.h"
#include "input_handler.h"

#include <stdio.h>
#include <stdlib.h>

//----------------------------------------------------------------------------
// core functions

int eg_initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(
            stderr,
            "failed to initialize SDL. error: %s\n",
            SDL_GetError());
        return 0;
    }

    return 1;
}

void eg_terminate()
{
    SDL_Quit();
}

eg_app *eg_create_app()
{
    eg_app *app = NULL;

    // Allocate memory for the eg_app struct.
    app = (eg_app *)malloc(sizeof(eg_app));

    // Verify memory allocation.
    if (app == NULL)
    {
        return NULL;
    }

    // Create the window.
    app->window = SDL_CreateWindow(
        "Example Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        240,
        160,
        SDL_WINDOW_SHOWN);

    // Verify window creation.
    if (app->window == NULL)
    {
        fprintf(
            stderr,
            "failed to create window. error: %s\n",
            SDL_GetError());
        free(app);
        return NULL;
    }

    // Create the renderer.
    app->renderer = SDL_CreateRenderer(
        app->window,
        -1,
        SDL_RENDERER_ACCELERATED);

    // Verify renderer creation.
    if (app->renderer == NULL)
    {
        fprintf(
            stderr,
            "failed to create renderer. error: %s\n",
            SDL_GetError());
        SDL_DestroyWindow(app->window);
        free(app);
        return NULL;
    }

    // Set the main loop sentinel value to 0.
    app->done = 0;

    // Since the desired framerate is 60 frames per second, we set the
    // approximate milliseconds per frame to 16.
    // This is the truncated result of 1000 / 16.
    app->frame_len = 16;

    // Get the keyboard state.
    app->keystates = SDL_GetKeyboardState(NULL);

    // Initialize the key press flags to 0.
    for (int i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        app->key_captures[i] = 0;
    }

    // Set the initial input handler.
    app->input_handler = my_input_handler;

    return app;
}

void eg_destroy_app(eg_app *app)
{
    // Destroy the renderer, the window, then the wrapper structure.
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
    free(app);
}

void eg_process_events(eg_app *app)
{
    // First, we get the current tick count for regulating framerate.
    app->ticks = SDL_GetTicks64();

    // According to the wiki, it is common practice to process all events in
    // the event queue at the beginning of each iteration of the main loop.
    // The SDL_PollEvent function also calls SDL_PumpEvents, which updates the
    // keyboard state array.
    while (SDL_PollEvent(&(app->event)))
    {
        if (app->event.type == SDL_QUIT)
        {
            app->done = 1;
        }

        // Clear the key press capture flags.
        if (app->event.type == SDL_KEYUP)
        {
            app->key_captures[app->event.key.keysym.scancode] = 0;
        }
    }
}

void eg_handle_input(eg_app *app)
{
    // Currently, there is only one input handler for the application.
    // Eventually, we will have the ability to dynamically add and remove
    // input handlers during runtime.
    app->input_handler(app);
}

void eg_draw(eg_app *app)
{
    // Clear the contents of the previous frame.
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);

    // Prepare the contents of the current frame.

    // For this example, we draw a green rectangle.
    SDL_Rect r;
    r.x = 10;
    r.y = 10;
    r.w = 20;
    r.h = 20;
    SDL_SetRenderDrawColor(app->renderer, 30, 220, 20, 255);
    SDL_RenderFillRect(app->renderer, &r);

    // Show the contents of the current frame.
    SDL_RenderPresent(app->renderer);
}

void eg_delay(eg_app *app)
{
    // Get the approximate number of milliseconds since the beginning of the
    // current iteration of the main loop.
    Uint64 elapsed = SDL_GetTicks64() - app->ticks;

    // If the frame length is greater than the elapsed milliseconds since the
    // beginning of the frame, wait for the duration of the difference.
    if (app->frame_len > elapsed)
    {
        // The SDL_Delay function expects a Uint32 as its argument.
        // Converting from Uint64 to Uint32 will truncate the value, but the
        // difference between the frame length and the elapsed milliseconds
        // should never be greater than UINT32_MAX.
        SDL_Delay((Uint32)(app->frame_len - elapsed));
    }
}

//----------------------------------------------------------------------------
// input handling functions

int eg_peek_input(eg_app *app, int code)
{
    if (code >= SDL_NUM_SCANCODES)
    {
        return 0;
    }

    if (app->keystates[code])
    {
        return 1;
    }

    return 0;
}

int eg_consume_input(eg_app *app, int code)
{
    if (code >= SDL_NUM_SCANCODES)
    {
        return 0;
    }

    if (app->key_captures[code])
    {
        return 0;
    }

    if (app->keystates[code])
    {
        app->key_captures[code] = 1;
        return 1;
    }

    return 0;
}

void eg_test_func(eg_callback c, eg_app *app)
{
    c(app);
}
