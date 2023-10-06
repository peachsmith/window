#ifndef EG_DEMO_CRITTER_H
#define EG_DEMO_CRITTER_H

// a critter is a friendly forest animal

#include "example.h"

#define CRITTER_SQUIRREL 0
#define CRITTER_RACCOON 1
#define CRITTER_PANDA 2

/**
 * Populates an entity type struct with information about the critter entity.
 *
 * Params:
 *   eg_entity_type* - the critter's entity type definition
 */
void critter_demo_register(eg_entity_type *);

/**
 * Creates a critter entity.
 *
 * Params:
 *   int - the critter entity's initial x position
 *   int - the critter entity's initial y position
 *
 * Returns:
 *   eg_entity* - a pointer to the new critter entity.
 */
eg_entity *critter_demo_create(eg_app *, int, int);

#endif