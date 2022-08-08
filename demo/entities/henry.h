#ifndef EG_DEMO_HENRY_H
#define EG_DEMO_HENRY_H

// Henry is a hostile entity. We call him "Hostile Henry".

#include "example.h"

/**
 * Populates an entity type struct with information about the henry entity.
 *
 * Params:
 *   eg_entity_type* - the henry's entity type definition
 */
void henry_demo_register(eg_entity_type *);

/**
 * Creates a henry entity.
 *
 * Params:
 *   int - the henry entity's initial x position
 *   int - the henry entity's initial y position
 *
 * Returns:
 *   eg_entity* - a pointer to the new henry entity.
 */
eg_entity *henry_demo_create(int, int);

#endif