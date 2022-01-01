#ifndef EG_ENTITY_DEMO_H
#define EG_ENTITY_DEMO_H

// This file contains utility functions for demonstrating entities.
// For this demo, there are three types of entities defined: Q entity,
// W entity, and E entity.

#include "example.h"

// entity IDs used for the entity demo code
#define ENTITY_ID_Q 1
#define ENTITY_ID_W 2
#define ENTITY_ID_E 3

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