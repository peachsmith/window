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

#endif