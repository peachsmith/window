#ifndef EXAMPLE_SDL_INTERFACE_H
#define EXAMPLE_SDL_INTERFACE_H

// This interface is a wrapper around SDL.
// Since this is an example, all functions an data types will be prefixed
// with eg (for exempli gratia).

#include <SDL2/SDL.h>

/**
 * This structure wraps the various SDL data types.
 */
typedef struct eg_app eg_app;

/**
 * A node in a list of input handlers.
 */
typedef struct eg_input_handler eg_input_handler;

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
    // The window is an abstraction layer that SDL provides for the GUI
    // capabilities of the underlying operating system.
    SDL_Window *window;

    // The renderer is an abstraction layer that SDL provides for drawing
    // graphical data to the screen.
    SDL_Renderer *renderer;

    // This field represents a system event. It's internal fields will be set
    // by SDL_PollEvent during the event processing loop.
    SDL_Event event;

    // This pointer references an array containing flags that indicate
    // if keys on the keyboard are pressed. It contains a Uint8 value for each
    // key. If the value is 1, then the key is pressed. If the value is 0,
    // then the key is not pressed.
    const Uint8 *keystates;

    // This is the approximate number of milliseconds elapsed since the
    // initialization of SDL.
    Uint64 ticks;

    // Frame length is the approximate number of milliseconds expected to
    // elapse during each iteration of the main loop.
    Uint64 frame_len;

    // This is an array of flags to indicate that a key press has already been
    // detected.
    unsigned char key_captures[SDL_NUM_SCANCODES];

    // This flag is used as a sentinel value by the main loop.
    // As long as this value is 0, the main loop should continue to execute.
    // Once this flag is set to a non 0 value, any resources allocated by the
    // application should be freed and the application should terminate
    // gracefully.
    int done;

    // The input handler stack is a dynamic linked list of input handlers.
    // The input handler on the top of the stack is the only input handler
    // that can perform any action at any given time.
    eg_input_handler *input_handler_stack;
};

// definition of the eg_input_handler struct
struct eg_input_handler
{
    // The callback performs some action in response to input events.
    eg_callback callback;

    // The previous handler will become the top when the node that references
    // it is popped from the stack.
    eg_input_handler *previous;
};

//----------------------------------------------------------------------------
// core functions

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
 * Draws the graphical contents of the current frame to the screen.
 * 
 * Params:
 *   app* - a pointer to an app struct
 */
void eg_draw(eg_app *);

/**
 * Pauses execution of the main loop.
 * 
 * Params:
 *   app* - a pointer to an app struct
 */
void eg_delay(eg_app *);

//----------------------------------------------------------------------------
// input handling functions

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

/**
 * Creates a new input handler.
 * 
 * Params:
 *   eg_callback - a function that handles input
 * 
 * Returns:
 *   eg_input_handler* - a pointer to a new input handler
 */
eg_input_handler *eg_create_input_handler(eg_callback);

/**
 * Frees the memory allocated for an input handler.
 * 
 * Params:
 *   eg_input_handler* - a pointer to an input handler
 */
void eg_destroy_input_handler(eg_input_handler *);

/**
 * Pushes an input handler onto the top of the input handler stack.
 * Once pushed, the new input handler will be the current handler as long as
 * remains at the top.
 * 
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_input_handler* - a pointer to an input handler.
 */
void eg_push_input_handler(eg_app *, eg_input_handler *);

/**
 * Pops an input handler off the top of the input handler stack.
 * This function returns a pointer to the input handler that was removed from
 * the stack. It does not free any memory. It is the repsonsibility of the
 * caller to dispose of any input handlers popped from the stack.
 * 
 * Params:
 *   eg_app* - a pointer to an app struct
 * 
 * Returns:
 *   eg_input_handler* - the handler that was removed from the stack
 */
eg_input_handler *eg_pop_input_handler(eg_app *);

#endif