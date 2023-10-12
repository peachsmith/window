#ifndef DEMO_JIMBO_DIALOG_H
#define DEMO_JIMBO_DIALOG_H

// This file contains an entity that represents the fish menu.

#include "crumbs.h"

/**
 * Populates an entity type struct with jimbormation about the fish menu
 * entity.
 *
 * Params:
 *   eg_entity_type* - the fish menu entity type definition
 */
void jimbo_dialog_demo_register(eg_entity_type *);

/**
 * Creates a fish menu entity.
 *
 * Returns:
 *   eg_entity* - a pointer to the new fish menu entity.
 */
eg_entity *jimbo_dialog_demo_create();

void jimbo_dialog_demo_open(eg_app *, eg_entity *);

#endif