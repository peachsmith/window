#ifndef EG_DEMO_FISH_MENU_H
#define EG_DEMO_FISH_MENU_H

// This file contains an entity that represents the fish menu.

#include "example.h"

/**
 * Populates an entity type struct with information about the fish menu
 * entity.
 *
 * Params:
 *   eg_entity_type* - the fish menu entity type definition
 */
void fish_menu_demo_register(eg_entity_type *);

/**
 * Creates a fish menu entity.
 *
 * Returns:
 *   eg_entity* - a pointer to the new fish menu entity.
 */
eg_entity *fish_menu_demo_create();

#endif