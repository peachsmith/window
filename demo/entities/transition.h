#ifndef EG_DEMO_TRANSITION_H
#define EG_DEMO_TRANSITION_H

// This file contains examples of a screen transition.
// This will be rendered over all contents of a given scene to make the
// process of switching the contents of the screen less jaring.

#include "example.h"

/**
 * Populates an entity type struct with information about the screen
 * transition entity.
 *
 * Params:
 *   eg_entity_type* - the screen transition entity type definition
 */
void transition_demo_register(eg_entity_type *);

/**
 * Creates a screen transition entity.
 *
 * Returns:
 *   eg_entity* - a pointer to the new screen transition entity.
 */
eg_entity *transition_demo_create();

#endif