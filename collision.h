#ifndef TEMP_EG_COLLISION_H
#define TEMP_EG_COLLISION_H

#include "example.h"

// This is a temporary interface for collision detection.
// In the future, it may be kept in its own file, or it may be incorporated
// into the core source file.

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

#endif