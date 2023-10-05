#ifndef EG_DEMO_FIREBALL_H
#define EG_DEMO_FIREBALL_H

// fireball is a hostile entity. We call him "Hostile fireball".

#include "example.h"

/**
 * Populates an entity type struct with information about the fireball entity.
 *
 * Params:
 *   eg_entity_type* - the fireball's entity type definition
 */
void fireball_demo_register(eg_entity_type *);

/**
 * Creates a fireball entity.
 *
 * Params:
 *   int - the fireball entity's initial x position
 *   int - the fireball entity's initial y position
 *
 * Returns:
 *   eg_entity* - a pointer to the new fireball entity.
 */
eg_entity *fireball_demo_create(eg_app *, int, int);

#endif