#ifndef TNS_PAUSE_MENU_H
#define TNS_PAUSE_MENU_H

// The pause menu halts normal gameplay and allows the user to exit the
// application or return to the main menu.

#include "crumbs.h"

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