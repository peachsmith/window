#ifndef DEMO_TRANSITION_H
#define DEMO_TRANSITION_H

// This file contains examples of a screen transition.
// This will be rendered over all contents of a given scene to make the
// process of switching the contents of the screen less jaring.

#include "crumbs.h"

/**
 * Populates an entity type struct with information about the screen
 * transition entity.
 *
 * Params:
 *   cr_entity_type* - the screen transition entity type definition
 */
void demo_register_transition(cr_entity_type *);

/**
 * Creates a screen transition entity.
 *
 * Returns:
 *   cr_entity* - a pointer to the new screen transition entity.
 */
cr_entity *demo_create_transition(cr_app *);

#endif