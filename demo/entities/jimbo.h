#ifndef DEMO_JIMBO_H
#define DEMO_JIMBO_H

// Jimbo is an NPC that has a small amount of dialog.

#include "crumbs.h"

/**
 * Populates an entity type struct with information about the jimbo entity.
 *
 * Params:
 *   cr_entity_type* - the jimbo's entity type definition
 */
void demo_register_jimbo(cr_entity_type *);

/**
 * Creates a jimbo entity.
 *
 * Params:
 *   int - Jimbo's initial x position
 *   int - Jimbo's initial y position
 *
 * Returns:
 *   cr_entity* - a pointer to the Jimbo.
 */
cr_entity *demo_create_jimbo(cr_app *, int, int);

#endif