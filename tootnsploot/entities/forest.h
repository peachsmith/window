#ifndef TNS_FOREST_H
#define TNS_FOREST_H

// The background scenery for the forest scene.

#include "crumbs.h"

/**
 * Populates an entity type struct with information about the forest entity.
 *
 * Params:
 *   eg_entity_type* - the forest's entity type definition
 */
void tns_register_forest(eg_entity_type *);

/**
 * Creates a forest entity.
 *
 * Params:
 *   int - the forest entity's initial x position
 *   int - the forest entity's initial y position
 *
 * Returns:
 *   eg_entity* - a pointer to the new forest entity.
 */
eg_entity *tns_create_forest(eg_app *, int, int);

#endif