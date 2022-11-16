#ifndef EG_DEMO_INPUT_MENU_H
#define EG_DEMO_INPUT_MENU_H

// This file contains an entity that represents the input menu.

#include "example.h"

/**
 * Populates an entity type struct with inputrmation about the input menu
 * entity.
 *
 * Params:
 *   eg_entity_type* - the input menu entity type definition
 */
void input_menu_demo_register(eg_entity_type *);

/**
 * Creates a input menu entity.
 *
 * Returns:
 *   eg_entity* - a pointer to the new input menu entity.
 */
eg_entity *input_menu_demo_create();

#endif