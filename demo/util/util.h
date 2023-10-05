#ifndef EG_DEMO_UTIL_H
#define EG_DEMO_UTIL_H

#include "example.h"

/**
 * Draws various elements of a collision event on the screen.
 * This function is intended for debugging purposes.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_rect* - reference to the target rectangle
 *   eg_t_res* - a reference to a collision result struct
 *   eg_point* - a reference to an origin point
 *   eg_point* - a reference to a direction vector
 */
void demo_draw_collision(
    eg_app *app,
    eg_rect *r,
    eg_collision *res,
    eg_point *p,
    eg_point *d);

/**
 * Draws various elements of SAT collision detection.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_point* - an array of points for entity A
 *   eg_point* - an array of points for entity B
 */
void demo_draw_sat(
    eg_app *app,
    eg_point *pa,
    eg_point *pb,
    eg_point *n0,
    eg_point *n1,
    eg_point *q);

/**
 * Draws the camera boundaries on the screen.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 */
void demo_draw_camera(eg_app *);

//============================================================================
// camera boundary configurations

// no camera boundaries
#define EG_CAMERA_NONE 0

// left, right, top and bottom boundaries
#define EG_CAMERA_ALL 1

void demo_set_camera(eg_app *, int);

#endif