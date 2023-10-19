#ifndef DEMO_DEBUG_MENU_H
#define DEMO_DEBUG_MENU_H

// This file contains an entity that represents the debug menu.

#include "crumbs.h"

/**
 * Populates an entity type struct with debugrmation about the debug menu
 * entity.
 *
 * Params:
 *   cr_entity_type* - the debug menu entity type definition
 */
void demo_register_debug_menu(cr_entity_type *);

/**
 * Creates a debug menu entity.
 *
 * Returns:
 *   cr_entity* - a pointer to the new debug menu entity.
 */
cr_entity *demo_create_debug_menu(cr_app *);

#endif