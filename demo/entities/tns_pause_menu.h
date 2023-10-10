#ifndef EG_TNS_PAUSE_MENU_H
#define EG_TNS_PAUSE_MENU_H

// The pause menu for Toot n Sploot.
// This menu explains how to play the game.

#include "example.h"

/**
 * Populates an entity type struct with information about the pause menu
 * entity.
 *
 * Params:
 *   eg_entity_type* - the pause menu entity type definition
 */
void tns_register_pause_menu(eg_entity_type *);

/**
 * Creates a pause menu entity.
 *
 * Returns:
 *   eg_entity* - a pointer to the new pause menu entity.
 */
eg_entity *tns_create_pause_menu(eg_app *);

#endif