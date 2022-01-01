#ifndef EG_INPUT_DEMO_H
#define EG_INPUT_DEMO_H

// This file contains examples of input handler callback functions.
// Each of these functions print out some message in response to pressing the
// arrow keys.
// The root_input_callback function will push a new input handler onto the
// stack when the N key is pressed. This new handler will use the
// second_input_callback function. The second_input_callback function will pop
// itself from the stack when the escape key is pressed.

#include "example.h"

/**
 * The root input handler callback.
 * This is the default input handler for the application. It should be the
 * first input handler pushed onto the input handler stack.
 * In this example, pressing the N key pushes another input handler onto
 * the stack.
 * 
 * For entity example code:
 * When the Q key is pressed, the "Q" entity will be added to the app.
 * When the W key is pressed, the "W" entity will be added to the app.
 * When the E key is pressed, the "E" entity will be added to the app.
 * If the shift key is pressed when any of the above three keys are pressed,
 * the corresponding entity will be removed.
 * 
 * Params:
 *   eg_app* - a pointer to an app struct
 */
void root_input_callback(eg_app *);

/**
 * An example of an input handler callback.
 * This function is intended to be used in an input handler that is pushed
 * onto the stack at runtime. Pressing the escape key pops this input handler
 * off the stack.
 * 
 * Params:
 *   eg_app* - a pointer to an app struct
 */
void second_input_callback(eg_app *);

#endif