#ifndef TEMP_EG_COLLISION_H
#define TEMP_EG_COLLISION_H

#include "example.h"

// This is a temporary interface for collision detection.
// In the future, it may be kept in its own file, or it may be incorporated
// into the core source file.

/**
 * Determines if a collision has occurred based two entities.
 * This function only considers a collision to have occurred if one of the
 * entities has a velocity vector with at least one non zero component.
 *
 * Known Issues
 * ------------
 * There is a chance that on a row or column of target rectangles, a source
 * rectangle may have its position adjusted much too far.
 * When moving parallel to a target rectangle while flush with the side,
 * a collision may detected despite no overlap.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_entity* - the source entity
 *   eg_entity* - the target entity
 *   eg_t-res* - a pointer to the collision result struct
 */
int eg_check_past_col(
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