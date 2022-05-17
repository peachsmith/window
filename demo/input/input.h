#ifndef EG_DEMO_INPUT_H
#define EG_DEMO_INPUT_H

// This file contains input handler callback functions.

#include "example.h"

#define MAX_INPUT_HANDLERS 20

void demo_init_input(eg_app *);

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
void root_input_handler(eg_app *);
void fish_input_handler(eg_app *);
void pause_input_handler(eg_app *);
void demo_dialog_input_handler(eg_app *);

#endif