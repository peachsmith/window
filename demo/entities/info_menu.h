#ifndef DEMO_INFO_MENU_H
#define DEMO_INFO_MENU_H

// This file contains an entity that represents the info menu.

#include "crumbs.h"

/**
 * Populates an entity type struct with information about the info menu
 * entity.
 *
 * Params:
 *   eg_entity_type* - the info menu entity type definition
 */
void info_menu_demo_register(eg_entity_type *);

/**
 * Creates a info menu entity.
 *
 * Returns:
 *   eg_entity* - a pointer to the new info menu entity.
 */
eg_entity *info_menu_demo_create(eg_app *);

#endif