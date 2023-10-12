#ifndef DEMO_PLAYER_H
#define DEMO_PLAYER_H

// This file contains examples of a player avatar. It is an entity that is
// controlled directly by user input. In this example, the player entity can
// be moved around the screen with the arrow keys. The player entity may also
// be referred to as Frank.

#include "crumbs.h"

/**
 * Populates an entity type struct with information about the player entity.
 *
 * Params:
 *   cr_entity_type* - the player's entity type definition
 */
void demo_register_player(cr_entity_type *);

/**
 * Creates a player entity.
 *
 * Params:
 *   int - the player entity's initial x position
 *   int - the player entity's initial y position
 *
 * Returns:
 *   cr_entity* - a pointer to the new player entity.
 */
cr_entity *demo_create_player(cr_app *, int, int);

#endif