#ifndef EG_DEMO_JIMBO_H
#define EG_DEMO_JIMBO_H

// Jimbo is an NPC that has a small amount of dialog.

#include "example.h"

/**
 * Populates an entity type struct with information about the jimbo entity.
 *
 * Params:
 *   eg_entity_type* - the jimbo's entity type definition
 */
void jimbo_demo_register(eg_entity_type *);

/**
 * Creates a jimbo entity.
 *
 * Params:
 *   int - the jimbo entity's initial x position
 *   int - the jimbo entity's initial y position
 *
 * Returns:
 *   eg_entity* - a pointer to the new jimbo entity.
 */
eg_entity *jimbo_demo_create(int, int);

#endif