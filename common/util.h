#ifndef COMMON_UTIL_H
#define COMMON_UTIL_H

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
// CR_CAMERA_NONE:
//   no camera boundaries
//
// CR_CAMERA_ALL:
//   left, right, top and bottom boundaries
#define CR_CAMERA_NONE 0
#define CR_CAMERA_ALL 1

// asset management
int common_init_textures(cr_app *);
int common_init_fonts(cr_app *);
int common_init_audio(cr_app *);

/**
 * Draws various elements of a collision event on the screen.
 * This function is intended for debugging purposes.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   cr_rect* - reference to the target rectangle
 *   cr_t_res* - a reference to a collision result struct
 *   cr_point* - a reference to an origin point
 *   cr_point* - a reference to a direction vector
 */
void util_draw_collision(
    cr_app *app,
    cr_rect *r,
    cr_collision *res,
    cr_point *p,
    cr_point *d);

/**
 * Draws various elements of SAT collision detection.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   cr_point* - an array of points for entity A
 *   cr_point* - an array of points for entity B
 */
void util_draw_sat(
    cr_app *app,
    cr_point *pa,
    cr_point *pb,
    cr_point *n0,
    cr_point *n1,
    cr_point *q);

/**
 * Draws the camera boundaries on the screen.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 */
void util_draw_camera(cr_app *);

/**
 * Draws the camera boundaries on the screen.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 */
void util_draw_camera(cr_app *);

/**
 * Sets the camera boundary configuration.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   int - the camera boundary configuration
 */
void util_set_camera(cr_app *, int);

#endif