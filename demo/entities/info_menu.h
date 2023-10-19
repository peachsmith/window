#ifndef DEMO_INFO_MENU_H
#define DEMO_INFO_MENU_H

// This file contains an entity that represents the info menu.

#include "crumbs.h"

/**
 * Populates an entity type struct with information about the info menu
 * entity.
 *
 * Params:
 *   cr_entity_type* - the info menu entity type definition
 */
void demo_register_info_menu(cr_entity_type *);

/**
 * Creates a info menu entity.
 *
 * Returns:
 *   cr_entity* - a pointer to the new info menu entity.
 */
cr_entity *demo_create_info_menu(cr_app *);

#endif