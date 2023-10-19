#ifndef DEMO_BLOCK_H
#define DEMO_BLOCK_H

// A block is an example of a solid object.
// It is typically a static rectangle. A block's only interaction with other
// entities is to prevent them from passing through itself.
// Each type of block has a create function and a register function.

#include "crumbs.h"

// register and create functions for a small, square block.
void demo_register_block(cr_entity_type *);
cr_entity *demo_create_block(cr_app *, int, int);

// register and create functions for a large, square block.
void demo_register_big_block(cr_entity_type *);
cr_entity *demo_create_big_block(cr_app *, int, int);

// register and create functions for a long, rectangular block.
void demo_register_long_block(cr_entity_type *);
cr_entity *demo_create_long_block(cr_app *, int, int);

// register and create functions for a long, rectangular block that can be
// passed through with certain input.
void demo_register_long_throughblock(cr_entity_type *);
cr_entity *demo_create_long_throughblock(cr_app *, int, int);

// register and create functions for a moving block.
void demo_register_moving_block(cr_entity_type *);
cr_entity *demo_create_moving_block(cr_app *, int, int, int);

// register and create functions for a sloped block.
void demo_register_sloped_block(cr_entity_type *);
cr_entity *demo_create_sloped_block(cr_app *, int, int, int);


/**
 * add a row of blocks to a scene
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   int - the starting x position of the row
 *   int - the starting y position of the row
 *   int - the number of blocks in the row
 */
void block_demo_row(cr_app *, int, int, int);

/**
 * add a column of blocks to a scene
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   int - the starting x position of the column
 *   int - the starting y position of the column
 *   int - the number of blocks in the column
 */
void block_demo_col(cr_app *, int, int, int);

#endif