#ifndef EG_DEMO_ENTITY_H
#define EG_DEMO_ENTITY_H

// This file contains utility functions for demonstrating entities.
// For this demo, there are three types of entities defined: Q entity,
// W entity, and E entity.

#include "example.h"

/**
 * Populates the entity type structs with information about the Q entity.
 * 
 * Params:
 *   eg_entity_type* - the player's entity type definition
 */
void entity_demo_register_q(eg_entity_type*);

/**
 * Populates the entity type structs with information about the W entity.
 * 
 * Params:
 *   eg_entity_type* - the player's entity type definition
 */
void entity_demo_register_w(eg_entity_type*);

/**
 * Populates the entity type structs with information about the E entity.
 * 
 * Params:
 *   eg_entity_type* - the player's entity type definition
 */
void entity_demo_register_e(eg_entity_type*);

/**
 * Adds a demo entity to the entity list.
 * 
 * Params:
 *   eg_app* - a pointer to an app struct
 *   int - an identifier that signifies the type of entity to add
 * 
 * Returns:
 *   int - 1 on success or 0 on failure
 */
int entity_demo_add(eg_app *app, int);

/**
 * Removes a demo entity from the entity list.
 * 
 * Params:
 *   eg_app* - a pointer to an app struct
 *   int - an identifier that signifies the type of entity to remove
 */
void entity_demo_remove(eg_app *app, int);

/**
 * Prints the contents of the entity list to standard output.
 * 
 * Params:
 *   eg_app* - a pointer to an app struct
 */
void entity_demo_print(eg_app *app);

#endif