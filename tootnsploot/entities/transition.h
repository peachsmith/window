#ifndef TNS_TRANSITION_H
#define TNS_TRANSITION_H

// This file contains examples of a screen transition.
// This will be rendered over all contents of a given scene to make the
// process of switching the contents of the screen less jaring.

#include "crumbs.h"

/**
 * Populates an entity type struct with information about the screen
 * transition entity.
 *
 * Params:
 *   eg_entity_type* - the screen transition entity type definition
 */
void tns_register_transition(eg_entity_type *);

/**
 * Creates a screen transition entity.
 *
 * Returns:
 *   eg_entity* - a pointer to the new screen transition entity.
 */
eg_entity *tns_create_transition(eg_app *);

#endif