#ifndef DEMO_FIREBALL_H
#define DEMO_FIREBALL_H

// fireball is a hostile entity. We call him "Hostile fireball".

#include "crumbs.h"

/**
 * Populates an entity type struct with information about the fireball entity.
 *
 * Params:
 *   cr_entity_type* - the fireball's entity type definition
 */
void demo_register_fireball(cr_entity_type *);

/**
 * Creates a fireball entity.
 *
 * Params:
 *   int - the fireball entity's initial x position
 *   int - the fireball entity's initial y position
 *
 * Returns:
 *   cr_entity* - a pointer to the new fireball entity.
 */
cr_entity *demo_create_fireball(cr_app *, int, int);

#endif