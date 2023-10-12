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
void jimbo_dialog_demo_register(cr_entity_type *);

/**
 * Creates a fish menu entity.
 *
 * Returns:
 *   cr_entity* - a pointer to the new fish menu entity.
 */
cr_entity *jimbo_dialog_demo_create();

void jimbo_dialog_demo_open(cr_app *, cr_entity *);

#endif