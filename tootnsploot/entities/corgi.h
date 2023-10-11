#ifndef EG_TNS_CORGI_H
#define EG_TNS_CORGI_H

// The corgi represents the player avatar. His name is Skippy.
// His job is to restore the woodland critters' will to live with the power
// of music.

#include "example.h"

/**
 * Populates an entity type struct with information about the corgi entity.
 *
 * Params:
 *   eg_entity_type* - the corgi's entity type definition
 */
void tns_register_corgi(eg_entity_type *);

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
eg_entity *tns_create_corgi(eg_app *, int, int);

#endif