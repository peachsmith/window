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
void tns_main_menu_input_handler(eg_app *);
void tns_controls_menu_input_handler(eg_app *);
void tns_characters_menu_input_handler(eg_app *);
void tns_forest_input_handler(eg_app *);
void tns_pause_menu_input_handler(eg_app *);

// The default input handler functions as a way for the user to exit the
// application if all other input handlers have been removed.
void default_input_handler(eg_app *);

#endif