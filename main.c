#include <stdio.h>
#include <SDL2/SDL.h>

int main(int argc, char **argv)
{
    printf("window example\n");

    // Initialize SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "failed to initialize SDL. error: %s\n", SDL_GetError());
        return 1;
    }

    // Create the window.
    // The Game Boy Advance had a screen resolution of 240 × 160 pixels.
    // TODO: investigate SDL_WINDOW_ALLOW_HIGHDPI on Mac.
    SDL_Window *win;
    win = SDL_CreateWindow(
        "Example Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        240,
        160,
        SDL_WINDOW_SHOWN);

    if (win == NULL)
    {
        fprintf(stderr, "failed to create window. error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create the renderer.
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (ren == NULL)
    {
        fprintf(stderr, "failed to create renderer. error: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    // A rectangle to draw on the screen.
    SDL_Rect r;
    r.x = 10;
    r.y = 10;
    r.w = 20;
    r.h = 20;

    SDL_Event e;
    int done = 0;
    while (!done)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                done = 1;
            }
        }

        // Clear the screen.
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        // Draw a rectangle.
        SDL_SetRenderDrawColor(ren, 200, 220, 0, 255);
        SDL_RenderFillRect(ren, &r);

        // Render content to the screen.
        SDL_RenderPresent(ren);

        // Delay to regulate framerate.
        SDL_Delay(64);
    }

    // Destroy the renderer.
    SDL_DestroyRenderer(ren);

    // Destroy the window.
    SDL_DestroyWindow(win);

    // Terminate SDL.
    SDL_Quit();

    return 0;
}
