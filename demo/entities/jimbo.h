#ifndef EG_DEMO_jimbo_H
#define EG_DEMO_jimbo_H

// This file contains examples of a jimbo avatar. It is an entity that is
// controlled directly by user input. In this example, the jimbo entity can
// be moved around the screen with the arrow keys.

#include "example.h"

/**
 * Populates an entity type struct with information about the jimbo entity.
 *
 * Params:
 *   eg_entity_type* - the jimbo's entity type definition
 */
void jimbo_demo_register(eg_entity_type *);

/**
 * Creates a jimbo entity.
 *
 * Params:
 *   int - the jimbo entity's initial x position
 *   int - the jimbo entity's initial y position
 *
 * Returns:
 *   eg_entity* - a pointer to the new jimbo entity.
 */
eg_entity *jimbo_demo_create(int, int);

#endif