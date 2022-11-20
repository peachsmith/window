#ifndef EG_DEMO_BILLY_H
#define EG_DEMO_BILLY_H

// Billy is an NPC that walks around a little, then does nothing.

#include "example.h"

/**
 * Populates an entity type struct with information about the billy entity.
 *
 * Params:
 *   eg_entity_type* - the billy's entity type definition
 */
void billy_demo_register(eg_entity_type *);

/**
 * Creates a billy entity.
 *
 * Params:
 *   int - the billy entity's initial x position
 *   int - the billy entity's initial y position
 *
 * Returns:
 *   eg_entity* - a pointer to the new billy entity.
 */
eg_entity *billy_demo_create(eg_app *, int, int);

#endif