#ifndef CRUMBS_COMMON_UTIL_H
#define CRUMBS_COMMON_UTIL_H

#include "crumbs.h"

// flags
#define ENTITY_FLAG_GROUND 0
#define ENTITY_FLAG_JUMP 1
#define ENTITY_FLAG_DOWN 2
#define ENTITY_FLAG_MOVE 3
#define ENTITY_FLAG_UPDATE 4
#define ENTITY_FLAG_SLOPE 5
#define ENTITY_FLAG_MIRROR 6
#define ENTITY_FLAG_INVINCIBLE 7
#define ENTITY_FLAG_PAUSE 8
#define ENTITY_FLAG_MENU 9
#define ENTITY_FLAG_10 10
#define ENTITY_FLAG_11 11
#define ENTITY_FLAG_12 12
#define ENTITY_FLAG_13 13
#define ENTITY_FLAG_14 14
#define ENTITY_FLAG_15 15

// Camera Boundary Configurations
//
// EG_CAMERA_NONE:
//   no camera boundaries
//
// EG_CAMERA_ALL:
//   left, right, top and bottom boundaries
#define EG_CAMERA_NONE 0
#define EG_CAMERA_ALL 1

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
void util_draw_collision(
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
void util_draw_sat(
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
void util_draw_camera(eg_app *);

/**
 * Draws the camera boundaries on the screen.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 */
void util_draw_camera(eg_app *);

/**
 * Sets the camera boundary configuration.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   int - the camera boundary configuration
 */
void util_set_camera(eg_app *, int);

#endif