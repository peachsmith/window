#ifndef EG_TNS_CONTROLS_MENU_H
#define EG_TNS_CONTROLS_MENU_H

// The controls menu for Toot n Sploot.
// This menu explains how to play the game.

#include "example.h"

/**
 * Populates an entity type struct with information about the controls menu
 * entity.
 *
 * Params:
 *   eg_entity_type* - the controls menu entity type definition
 */
void tns_register_controls_menu(eg_entity_type *);

/**
 * Creates a controls menu entity.
 *
 * Returns:
 *   eg_entity* - a pointer to the new controls menu entity.
 */
eg_entity *tns_create_controls_menu(eg_app *);

#endif