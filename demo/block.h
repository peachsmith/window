#ifndef EG_DEMO_BLOCK_H
#define EG_DEMO_BLOCK_H

// A block is an example of a solid object.

#include "example.h"

/**
 * Populates an entity type struct with information about the bloc entity
 * type.
 * 
 * Params:
 *   eg_entity_type* - the block entity type definition
 */
void block_demo_register(eg_entity_type*);

/**
 * Creates a block entity.
 * 
 * Params:
 *   int - the x position
 *   int - the y position
 * 
 * Returns:
 *   eg_entity* - a pointer to the new block entity.
 */
eg_entity* block_demo_create(int, int);

#endif