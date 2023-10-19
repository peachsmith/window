#ifndef DEMO_JIMBO_DIALOG_H
#define DEMO_JIMBO_DIALOG_H

// This file contains an entity that represents the fish menu.

#include "crumbs.h"

/**
 * Populates an entity type struct with jimbormation about the fish menu
 * entity.
 *
 * Params:
 *   cr_entity_type* - the fish menu entity type definition
 */
void demo_register_jimbo_dialog(cr_entity_type *);

/**
 * Creates a fish menu entity.
 *
 * Returns:
 *   cr_entity* - a pointer to the new fish menu entity.
 */
cr_entity *demo_create_jimbo_dialog(cr_app*);

void demo_open_jimbo_dialog(cr_app *, cr_entity *);

#endif