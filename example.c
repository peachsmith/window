#include "example.h"
#include <stdio.h>
#include <stdlib.h>

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
    // According to the wiki, it is common practice to process
    // all events in the event queue at the beginning of each
    // iteration of the main loop.
    while (SDL_PollEvent(&(app->e)))
    {
        if (app->e.type == SDL_QUIT)
        {
            app->done = 1;
        }
    }
}

void eg_clear(eg_app *app)
{
    // Set the draw color to black and clear the screen.
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);
}

void eg_draw(eg_app *app)
{
    // Draw a rectangle.
    SDL_Rect r;
    r.x = 10;
    r.y = 10;
    r.w = 20;
    r.h = 20;
    SDL_SetRenderDrawColor(app->renderer, 200, 220, 0, 255);
    SDL_RenderFillRect(app->renderer, &r);
}

void eg_show(eg_app *app)
{
    SDL_RenderPresent(app->renderer);
}

void eg_delay()
{
    // This is where the framerate can be regulated.
    // For now, we just use a 64 millisecond delay.
    SDL_Delay(64);
}
