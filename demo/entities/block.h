#ifndef DEMO_BLOCK_H
#define DEMO_BLOCK_H

// A block is an example of a solid object.
// It is typically a static rectangle. A block's only interaction with other
// entities is to prevent them from passing through itself.
// Each type of block has a create function and a register function.

#include "crumbs.h"

// register and create functions for a small, square block.
void block_demo_register(eg_entity_type *);
eg_entity *block_demo_create(eg_app *, int, int);

// register and create functions for a large, square block.
void block_demo_register_big(eg_entity_type *);
eg_entity *block_demo_create_big(eg_app *, int, int);

// register and create functions for a long, rectangular block.
void block_demo_register_long(eg_entity_type *);
eg_entity *block_demo_create_long(eg_app *, int, int);

// register and create functions for a long, rectangular block that can be
// passed through with certain input.
void throughblock_demo_register_long(eg_entity_type *);
eg_entity *throughblock_demo_create_long(eg_app *, int, int);

// register and create functions for a moving block.
void block_demo_register_moving(eg_entity_type *);
eg_entity *block_demo_create_moving(eg_app *, int, int, int);

// register and create functions for a sloped block.
void block_demo_register_sloped(eg_entity_type *);
eg_entity *block_demo_create_sloped(eg_app *, int, int, int);

// register and create functions for Toot n Sploot
void tns_register_floor(eg_entity_type *);
eg_entity *tns_create_floor(eg_app *, int, int);


/**
 * add a row of blocks to a scene
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   int - the starting x position of the row
 *   int - the starting y position of the row
 *   int - the number of blocks in the row
 */
void block_demo_row(eg_app *, int, int, int);

/**
 * add a column of blocks to a scene
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   int - the starting x position of the column
 *   int - the starting y position of the column
 *   int - the number of blocks in the column
 */
void block_demo_col(eg_app *, int, int, int);

#endif