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
 *   eg_entity_type* - the sign's entity type definition
 */
void sign_demo_register(eg_entity_type *);

/**
 * Creates a sign entity.
 *
 * Params:
 *   int - the sign entity's initial x position
 *   int - the sign entity's initial y position
 *
 * Returns:
 *   eg_entity* - a pointer to the new sign entity.
 */
eg_entity *sign_demo_create(eg_app *, int, int);

#endif