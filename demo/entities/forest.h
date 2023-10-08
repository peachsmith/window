#ifndef EG_DEMO_FOREST_H
#define EG_DEMO_FOREST_H

// A heads-up display (FOREST)

#include "example.h"

/**
 * Populates an entity type struct with information about the forest entity.
 *
 * Params:
 *   eg_entity_type* - the forest's entity type definition
 */
void forest_demo_register(eg_entity_type *);

/**
 * Creates a forest entity.
 *
 * Params:
 *   int - the forest entity's initial x position
 *   int - the forest entity's initial y position
 *
 * Returns:
 *   eg_entity* - a pointer to the new forest entity.
 */
eg_entity *forest_demo_create(eg_app *, int, int);

#endif