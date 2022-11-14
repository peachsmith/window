#ifndef EG_DEMO_PAUSE_MENU_ENTITY_H
#define EG_DEMO_PAUSE_MENU_ENTITY_H

// This file contains an entity that represents the pause menu.

#include "example.h"

/**
 * Populates an entity type struct with information about the pause menu
 * entity.
 *
 * Params:
 *   eg_entity_type* - the pause menu entity type definition
 */
void pause_menu_entity_demo_register(eg_entity_type *);

/**
 * Creates a pause menu entity.
 *
 * Returns:
 *   eg_entity* - a pointer to the new pause menu entity.
 */
eg_entity *pause_menu_entity_demo_create();

#endif