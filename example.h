#ifndef EXAMPLE_SDL_INTERFACE_H
#define EXAMPLE_SDL_INTERFACE_H

#include <SDL2/SDL.h>

// This interface is a wrapper around SDL.
// Since this is an example, all functions an data types will be prefixed
// with eg (for exempli gratia).

/**
 * This structure wraps the various SDL data types.Î
 */
typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event e;
    int done;
} eg_app;

/**
 * Initializes the SDL library.
 * This should be called once before allocating any resources or using any
 * other functions in this interface.
 * 
 * Returns:
 *   int - 1 on success, or 0 on failure
 */
int eg_initialize();

/**
 * Terminates the SDL library.
 * This should be called after freeing all other resources that were allocated
 * during execution.
 */
void eg_terminate();

/**
 * Creates a new instance of the app struct.
 * 
 * Returns:
 *   app* - a pointer to an app struct
 */
eg_app *eg_create_app();

/**
 * Frees the memory allocated for an app struct.
 * 
 * Params:
 *   app* - a pointer to an app struct
 */
void eg_destroy_app(eg_app *);

/**
 * Processes all events in the event queue.
 * 
 * Params:
 *   app* - a pointer to an app struct
 */
void eg_process_events(eg_app *);

/**
 * Clears the contents of the screen.
 * 
 * Params:
 *   app* - a pointer to an app struct
 */
void eg_clear(eg_app *);

/**
 * Prepares graphical content for rendering.
 * 
 * Params:
 *   app* - a pointer to an app struct
 */
void eg_draw(eg_app *);

/**
 * Renders graphical content to the screen.
 * 
 * Params:
 *   app* - a pointer to an app struct
 */
void eg_show(eg_app *);

/**
 * Pauses execution of the main loop.
 */
void eg_delay();

#endif