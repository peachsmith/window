#ifndef EG_DEMO_INPUT_H
#define EG_DEMO_INPUT_H

// This file contains input handler callback functions.

#include "example.h"

/**
 * A utility function for invoking the input handlers.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 */
void handle_input(eg_app *);

/**
 * The root input handler callback.
 * This is the default input handler for the application. It should be the
 * first input handler pushed onto the input handler stack.
 * In this example, pressing the N key pushes another input handler onto
 * the stack.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 */
void root_input_callback(eg_app *, eg_entity *);

#endif