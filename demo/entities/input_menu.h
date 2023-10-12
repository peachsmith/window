#ifndef DEMO_INPUT_MENU_H
#define DEMO_INPUT_MENU_H

// This file contains an entity that represents the input menu.

#include "crumbs.h"

/**
 * Populates an entity type struct with inputrmation about the input menu
 * entity.
 *
 * Params:
 *   cr_entity_type* - the input menu entity type definition
 */
void input_menu_demo_register(cr_entity_type *);

/**
 * Creates a input menu entity.
 *
 * Returns:
 *   cr_entity* - a pointer to the new input menu entity.
 */
cr_entity *input_menu_demo_create(cr_app *);

#endif