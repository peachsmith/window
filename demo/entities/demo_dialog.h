#ifndef DEMO_DEMO_DIALOG_H
#define DEMO_DEMO_DIALOG_H

// This file contains an entity that represents the fish menu.

#include "crumbs.h"

/**
 * Populates an entity type struct with information about the fish menu
 * entity.
 *
 * Params:
 *   cr_entity_type* - the fish menu entity type definition
 */
void demo_dialog_demo_register(cr_entity_type *);

/**
 * Creates a fish menu entity.
 *
 * Returns:
 *   cr_entity* - a pointer to the new fish menu entity.
 */
cr_entity *demo_dialog_demo_create(cr_app *);

void demo_dialog_demo_open(cr_app *, cr_entity *);

#endif