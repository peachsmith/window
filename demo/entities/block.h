#ifndef EG_DEMO_BLOCK_H
#define EG_DEMO_BLOCK_H

// A block is an example of a solid object.
// It is typically a static rectangle. A block's only interaction with other
// entities is to prevent them from passing through itself.
// Each type of block has a create function and a register function.

#include "example.h"

/**
 * Populates an entity type struct with information about the block entity
 * type.
 *
 * Params:
 *   eg_entity_type* - the block entity type definition
 */
void block_demo_register(eg_entity_type *);

/**
 * Creates a new block entity.
 *
 * Params:
 *   int - the x position
 *   int - the y position
 */
eg_entity *block_demo_create(int, int);

// register and create functions for a large, square block.
void block_demo_register_big(eg_entity_type *);
eg_entity *block_demo_create_big(int, int);

// register and create functions for a long, rectangular block.
void block_demo_register_long(eg_entity_type *);
eg_entity *block_demo_create_long(int, int);

// register and create functions for a long, rectangular block that can be
// passed through with certain input.
void throughblock_demo_register_long(eg_entity_type *);
eg_entity *throughblock_demo_create_long(int, int);

// register and create functions for a moving block.
void block_demo_register_moving(eg_entity_type *);
eg_entity *block_demo_create_moving(int, int, int);

// register and create functions for a sloped block.
void block_demo_register_sloped(eg_entity_type *);
eg_entity *block_demo_create_sloped(int, int);

#endif