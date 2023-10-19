#ifndef DEMO_PAUSE_MENU_H
#define DEMO_PAUSE_MENU_H

// This file contains an entity that represents the pause menu.

#include "crumbs.h"

/**
 * Populates an entity type struct with information about the pause menu
 * entity.
 *
 * Params:
 *   cr_entity_type* - the pause menu entity type definition
 */
void demo_register_pause_menu(cr_entity_type *);

/**
 * Creates a pause menu entity.
 *
 * Returns:
 *   cr_entity* - a pointer to the new pause menu entity.
 */
cr_entity *demo_create_pause_menu(cr_app *);

#endif