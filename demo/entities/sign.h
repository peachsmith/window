#ifndef DEMO_SIGN_H
#define DEMO_SIGN_H

// This file contains examples of a sign avatar. It is an entity that is
// controlled directly by user input. In this example, the sign entity can
// be moved around the screen with the arrow keys.

#include "crumbs.h"

/**
 * Populates an entity type struct with information about the sign entity.
 *
 * Params:
 *   cr_entity_type* - the sign's entity type definition
 */
void demo_register_sign(cr_entity_type *);

/**
 * Creates a sign entity.
 *
 * Params:
 *   int - the sign entity's initial x position
 *   int - the sign entity's initial y position
 *
 * Returns:
 *   cr_entity* - a pointer to the new sign entity.
 */
cr_entity *demo_create_sign(cr_app *, int, int);

#endif