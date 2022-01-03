#ifndef EG_PLAYER_DEMO_H
#define EG_PLAYER_DEMO_H

// This file contains examples of a player avatar. It is an entity that is
// controlled directly by user input. In this example, the player entity can
// be moved around the screen with the arrow keys.

#include "example.h"

/**
 * Creates a player entity.
 * 
 * Returns:
 *   eg_entity* - a pointer to the new player entity.
 */
eg_entity* player_demo_create();

#endif