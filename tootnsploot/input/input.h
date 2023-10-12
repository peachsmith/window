#ifndef TNS_INPUT_H
#define TNS_INPUT_H

// This file contains input handler callback functions.

#include "crumbs.h"

#define MAX_INPUT_HANDLERS 20

void tns_init_input(eg_app *);

void default_input_handler(eg_app *);

void main_menu_input_handler(eg_app *);
void controls_menu_input_handler(eg_app *);
void characters_menu_input_handler(eg_app *);
void forest_input_handler(eg_app *);
void pause_menu_input_handler(eg_app *);

#endif