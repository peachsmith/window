#ifndef EG_DEMO_HUD_H
#define EG_DEMO_HUD_H

// A heads-up display (HUD)

#include "example.h"

/**
 * Populates an entity type struct with information about the hud entity.
 *
 * Params:
 *   eg_entity_type* - the hud's entity type definition
 */
void tns_register_hud(eg_entity_type *);

/**
 * Creates a hud entity.
 *
 * Params:
 *   int - the hud entity's initial x position
 *   int - the hud entity's initial y position
 *
 * Returns:
 *   eg_entity* - a pointer to the new hud entity.
 */
eg_entity *tns_create_hud(eg_app *, int, int);

#endif