#ifndef DEMO_INFO_DIALOG_H
#define DEMO_INFO_DIALOG_H

// This file contains an entity that represents the fish menu.

#include "crumbs.h"

/**
 * Populates an entity type struct with information about the fish menu
 * entity.
 *
 * Params:
 *   cr_entity_type* - the fish menu entity type definition
 */
void info_dialog_demo_register(cr_entity_type *);

/**
 * Creates a fish menu entity.
 *
 * Returns:
 *   cr_entity* - a pointer to the new fish menu entity.
 */
cr_entity *info_dialog_demo_create(cr_app *);

void info_dialog_demo_open(cr_app *, cr_entity *);

#endif