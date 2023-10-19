#ifndef DEMO_BILLY_H
#define DEMO_BILLY_H

// Billy is an NPC that walks around a little, then does nothing.

#include "crumbs.h"

/**
 * Populates an entity type struct with information about the billy entity.
 *
 * Params:
 *   cr_entity_type* - the billy's entity type definition
 */
void demo_register_billy(cr_entity_type *);

/**
 * Creates a billy entity.
 *
 * Params:
 *   int - the billy entity's initial x position
 *   int - the billy entity's initial y position
 *
 * Returns:
 *   cr_entity* - a pointer to the new billy entity.
 */
cr_entity *demo_create_billy(cr_app *, int, int);

#endif