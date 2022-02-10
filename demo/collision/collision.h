#ifndef EG_DEMO_COLLISION_H
#define EG_DEMO_COLLISION_H

#include "example.h"

// a helper type for a dynamic array of collision results
typedef struct collision_list
{
    int cap;
    int count;
    eg_col_res *data;
} collision_list;

// utility functions for the collision_list type
collision_list *col_create_list();
void col_destroy_list(collision_list *);
void col_clear_list(collision_list *);
int col_push_result(collision_list *, eg_col_res *);

/**
 * Determines if a collision will occur between a moving source entity A and
 * a static target entity B. This uses an implementation of Swept AABB, which
 * assumes that all rectangles are axis-aligned.
 *
 * If A is static and B is moving, then this function treats A as if it were
 * the target and B as if it were the source.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_entity* - the source entity
 *   eg_entity* - the target entity
 *   eg_t-res* - a pointer to the collision result struct
 */
int eg_swept_aabb(
    eg_app *app,
    eg_entity *a,
    eg_entity *b,
    eg_ray_res *res);

/**
 * Determines if two rectangles overlap.
 * The third argument is a pointer to an eg_overlap struct.
 * This will receive the results of the overlap calculations.
 *
 * Params:
 *   eg_rect* - a reference to a rect
 *   eg_rect* - a reference to another rect
 *   eg_overlap* - a reference to the overlap calculation result struct
 */
int is_overlapped(
    eg_rect *a,
    eg_rect *b,
    eg_overlap *res);

/**
 * Detects and handles collisions between all entities in an application.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 */
void handle_collisions(eg_app *app);

#endif