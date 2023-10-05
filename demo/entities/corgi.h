#ifndef EG_DEMO_CORGI_H
#define EG_DEMO_CORGI_H

// This file contains examples of a corgi avatar. It is an entity that is
// controlled directly by user input. In this example, the corgi entity can
// be moved around the screen with the arrow keys.

#include "example.h"

/**
 * Populates an entity type struct with information about the corgi entity.
 *
 * Params:
 *   eg_entity_type* - the corgi's entity type definition
 */
void corgi_demo_register(eg_entity_type *);

/**
 * Creates a corgi entity.
 *
 * Params:
 *   int - the corgi entity's initial x position
 *   int - the corgi entity's initial y position
 *
 * Returns:
 *   eg_entity* - a pointer to the new corgi entity.
 */
eg_entity *corgi_demo_create(eg_app *, int, int);

#endif