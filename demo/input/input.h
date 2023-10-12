#ifndef DEMO_INPUT_H
#define DEMO_INPUT_H

// This file contains input handler callback functions.

#include "crumbs.h"

#define MAX_INPUT_HANDLERS 20

void demo_init_input(cr_app *);

void default_input_handler(cr_app *);

void root_input_handler(cr_app *);
void fish_menu_input_handler(cr_app *);
void info_menu_input_handler(cr_app *);
void pause_menu_input_handler(cr_app *);
void common_dialog_input_handler(cr_app *);
void debug_menu_input_handler(cr_app *);
void scene_menu_input_handler(cr_app *);
void input_menu_input_handler(cr_app *);

#endif