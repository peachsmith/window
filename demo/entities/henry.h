#ifndef DEMO_HENRY_H
#define DEMO_HENRY_H

// Henry is a hostile entity. We call him "Hostile Henry".

#include "crumbs.h"

/**
 * Populates an entity type struct with information about the henry entity.
 *
 * Params:
 *   cr_entity_type* - the henry's entity type definition
 */
void demo_register_henry(cr_entity_type *);

/**
 * Creates a henry entity.
 *
 * Params:
 *   int - the henry entity's initial x position
 *   int - the henry entity's initial y position
 *
 * Returns:
 *   cr_entity* - a pointer to the new henry entity.
 */
cr_entity *demo_create_henry(cr_app *, int, int);

#endif