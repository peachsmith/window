#ifndef DEMO_SCENE_MENU_H
#define DEMO_SCENE_MENU_H

// This file contains an entity that represents the scene menu.

#include "crumbs.h"

/**
 * Populates an entity type struct with scenermation about the scene menu
 * entity.
 *
 * Params:
 *   cr_entity_type* - the scene menu entity type definition
 */
void scene_menu_demo_register(cr_entity_type *);

/**
 * Creates a scene menu entity.
 *
 * Returns:
 *   cr_entity* - a pointer to the new scene menu entity.
 */
cr_entity *scene_menu_demo_create(cr_app *);

#endif