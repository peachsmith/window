#ifndef EG_DEMO_COLLISION_H
#define EG_DEMO_COLLISION_H

#include "example.h"

// defintion of eg_collision struct
struct eg_collision
{
    eg_point cp; // contact point
    eg_point cn; // contact normal
    float t;     // t such that P(t) = CP
};

typedef struct col_res
{
    eg_collision col;
    eg_entity *target;
} col_res;

// a helper type for a dynamic array of collision results
typedef struct collision_list
{
    int cap;
    int count;
    col_res *data;
} collision_list;

// utility functions for the collision_list type
// TODO: replace this with a non-heap approach.
collision_list *col_create_list();
void col_destroy_list(collision_list *);
void col_clear_list(collision_list *);
int col_push_result(collision_list *, col_res *);

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
int demo_swept_aabb(
    eg_app *app,
    eg_entity *a,
    eg_entity *b,
    eg_collision *res);

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
int demo_is_overlapped(
    eg_rect *a,
    eg_rect *b,
    eg_overlap *res);

/**
 * Detects and handles collisions between all entities in an application.
 * TODO: clipping through the floor is still possible. This requires further
 * investigation.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 */
void demo_handle_collisions(eg_app *app);

#endif