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

/**
 * Result of rectangle overlap calculation.
 */
typedef struct eg_overlap
{
    // The fields dx0, dx1, dy0, and dy1 are referred to as the deltas.
    // They represent the differences between the positions of the sides
    // of two rectangles.
    // Given two rectangles A and B:
    // dx0 is difference between the right side of A and the left side of B.
    // dx1 is difference between the right side of B and the left side of A.
    // dy0 is difference between the bottom side of A and the top side of B.
    // dy1 is difference between the bottom side of B and the top side of A.
    int dx0;
    int dx1;
    int dy0;
    int dy1;
} eg_overlap;

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
 * Used for slopes.
 * UNDER CONSTRUCTION
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_entity* - the source entity
 *   eg_entity* - the target entity
 *   eg_t-res* - a pointer to the collision result struct
 */
int demo_sat(
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