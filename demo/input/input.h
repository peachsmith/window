#ifndef EG_DEMO_INPUT_H
#define EG_DEMO_INPUT_H

// This file contains input handler callback functions.

#include "example.h"

#define MAX_INPUT_HANDLERS 20

void demo_init_input(eg_app *);

void root_input_handler(eg_app *);
void fish_menu_input_handler(eg_app *);
void info_menu_input_handler(eg_app *);
void pause_menu_input_handler(eg_app *);
void common_dialog_input_handler(eg_app *);
void debug_menu_input_handler(eg_app *);
void scene_menu_input_handler(eg_app *);
void input_menu_input_handler(eg_app *);

// Toot n Sploot input handlers
void tns_root_input_handler(eg_app *);

// used when no input should be processed
void empty_input_handler(eg_app *);

#endif