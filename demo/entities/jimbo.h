#ifndef DEMO_JIMBO_H
#define DEMO_JIMBO_H

// Jimbo is an NPC that has a small amount of dialog.

#include "crumbs.h"

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
 *   int - Jimbo's initial x position
 *   int - Jimbo's initial y position
 *
 * Returns:
 *   eg_entity* - a pointer to the Jimbo.
 */
eg_entity *jimbo_demo_create(eg_app *, int, int);

#endif