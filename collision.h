#ifndef TEMP_EG_COLLISION_H
#define TEMP_EG_COLLISION_H

#include "example.h"

// This is a temporary interface for collision detection.
// In the future, it may be kept in its own file, or it may be incorporated
// into the core source file.

// A point represents the x and y coordinates in 2D space.
typedef struct eg_point
{
    int x;
    int y;
} eg_point;

// A rectangle.
typedef struct eg_rect
{
    eg_point p; // location of top left corner
    int w;
    int h;
} eg_rect;

// The result of testing a ray for intersection with a target rectangle.
typedef struct eg_t_res
{
    eg_point cp; // contact point
    eg_point cn; // contact normal
    float t;     // t such that P(t) = CP
} eg_t_res;

/**
 * Determines if a ray intersects a rectangle.
 *
 * Given an origin point P, a direction vector D, and a target rectangle R,
 * the goal of this function is to determine if the line segment formed by
 * direction vector D intersects with the sides of R.
 *
 * The fourth argument is a reference to a eg_t_res struct. This contains the
 * contact point, contact normal, and t value such that P(t) = CP.
 *
 * Params:
 *   eg_point* - the origin point of the ray
 *   eg_point* - the direction vector of the ray
 *   eg_rect* - the rectangle
 *   eg_t_res* - results of intersection detection
 */
int eg_ray_v_rect(
    eg_point *p,
    eg_point *d,
    eg_rect *r,
    eg_t_res *res);

int eg_check_col(
    eg_app *,
    eg_entity *,
    eg_entity *,
    eg_t_res *);

#endif