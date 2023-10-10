#ifndef EG_TNS_CHARACTERS_MENU_H
#define EG_TNS_CHARACTERS_MENU_H

// The characters menu for Toot n Sploot.
// This menu explains how to play the game.

#include "example.h"

/**
 * Populates an entity type struct with information about the characters menu
 * entity.
 *
 * Params:
 *   eg_entity_type* - the characters menu entity type definition
 */
void tns_register_characters_menu(eg_entity_type *);

/**
 * Creates a characters menu entity.
 *
 * Returns:
 *   eg_entity* - a pointer to the new characters menu entity.
 */
eg_entity *tns_create_characters_menu(eg_app *);

#endif