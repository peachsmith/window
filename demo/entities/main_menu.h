#ifndef EG_DEMO_MAIN_MENU_H
#define EG_DEMO_MAIN_MENU_H

// The main menu for Toot n Sploot.
// This represents the content of the title screen.

#include "example.h"

/**
 * Populates an entity type struct with information about the main menu
 * entity.
 *
 * Params:
 *   eg_entity_type* - the main menu entity type definition
 */
void tns_register_main_menu(eg_entity_type *);

/**
 * Creates a main menu entity.
 *
 * Returns:
 *   eg_entity* - a pointer to the new main menu entity.
 */
eg_entity *tns_create_main_menu(eg_app *);

#endif