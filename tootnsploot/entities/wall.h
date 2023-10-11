#ifndef EG_TNS_WALL_H
#define EG_TNS_WALL_H

// A wall functions as a barrier restricting certain entities movement.

#include "example.h"

/**
 * Populates an entity type struct with information about the wall entity.
 *
 * Params:
 *   eg_entity_type* - the wall's entity type definition
 */
void tns_register_wall(eg_entity_type *);

/**
 * Creates a wall entity.
 *
 * Params:
 *   int - the wall entity's initial x position
 *   int - the wall entity's initial y position
 *
 * Returns:
 *   eg_entity* - a pointer to the new wall entity.
 */
eg_entity *tns_create_wall(eg_app *, int, int);

#endif