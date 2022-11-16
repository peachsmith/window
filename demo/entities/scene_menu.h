#ifndef EG_DEMO_SCENE_MENU_H
#define EG_DEMO_SCENE_MENU_H

// This file contains an entity that represents the scene menu.

#include "example.h"

/**
 * Populates an entity type struct with scenermation about the scene menu
 * entity.
 *
 * Params:
 *   eg_entity_type* - the scene menu entity type definition
 */
void scene_menu_demo_register(eg_entity_type *);

/**
 * Creates a scene menu entity.
 *
 * Returns:
 *   eg_entity* - a pointer to the new scene menu entity.
 */
eg_entity *scene_menu_demo_create();

#endif