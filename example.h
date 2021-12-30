#ifndef EXAMPLE_SDL_INTERFACE_H
#define EXAMPLE_SDL_INTERFACE_H

#include <SDL2/SDL.h>

// This interface is a wrapper around SDL.
// Since this is an example, all functions an data types will be prefixed
// with eg (for exempli gratia).

/**
 * This structure wraps the various SDL data types.
 */
typedef struct eg_app eg_app;

/**
 * Performs some task.
 * 
 * Params:
 *   eg_app* - a pointer to an app struct
 */
typedef void (*eg_callback)(struct eg_app *);

// definition of the eg_app struct
struct eg_app
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event e;
    const Uint8 *keystates;
    unsigned char key_captures[SDL_NUM_SCANCODES];
    int done;
    eg_callback input_handler;
};

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
 * Handles input.
 * 
 * Params:
 *   app* - a pointer to an app struct
 */
void eg_handle_input(eg_app *);

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

//----------------------------------------------------------------------------

/**
 * Checks to see if an input is actuated.
 * As long as the input is actuated, this function will return 1. This is used
 * for handling input events where some task must be performed for as long as
 * the input is actuated.
 * 
 * Params:
 *   eg_app* - a pointer to an app struct
 *   int - the scancode of the key to check
 */
int eg_peek_input(eg_app *app, int);

/**
 * Checks to see if an input is actuated.
 * The first time the input is actuated, this function will return 1.
 * All subsequent calls to this function will return 0 until the state of
 * the input in question is reset. This is used for handling input events
 * where a task must be performed a single time for a single input actuation.
 * 
 * Params:
 *   eg_app* - a pointer to an app struct
 *   int - the scancode of the key to check
 */
int eg_consume_input(eg_app *, int);

// temporary test function
void eg_test_func(eg_callback, eg_app *);

#endif