#ifndef EG_DEMO_DEBUG_MENU_H
#define EG_DEMO_DEBUG_MENU_H

// This file contains an entity that represents the debug menu.

#include "example.h"

/**
 * Populates an entity type struct with debugrmation about the debug menu
 * entity.
 *
 * Params:
 *   eg_entity_type* - the debug menu entity type definition
 */
void debug_menu_demo_register(eg_entity_type *);

/**
 * Creates a debug menu entity.
 *
 * Returns:
 *   eg_entity* - a pointer to the new debug menu entity.
 */
eg_entity *debug_menu_demo_create();

#endif