#ifndef EG_DEMO_PLAYER_H
#define EG_DEMO_PLAYER_H

// This file contains examples of a player avatar. It is an entity that is
// controlled directly by user input. In this example, the player entity can
// be moved around the screen with the arrow keys.

#include "example.h"

/**
 * Populates an entity type struct with information about the player entity.
 *
 * Params:
 *   eg_entity_type* - the player's entity type definition
 */
void player_demo_register(eg_entity_type *);

/**
 * Creates a player entity.
 *
 * Params:
 *   int - the player entity's initial x position
 *   int - the player entity's initial y position
 *
 * Returns:
 *   eg_entity* - a pointer to the new player entity.
 */
eg_entity *player_demo_create(int, int);

#endif