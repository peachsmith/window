#ifndef EG_DEMO_SPRITE_H
#define EG_DEMO_SPRITE_H

#include "example.h"

/**
 * Renders the player character to the screen.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   int - the x coordinate on the screen
 *   int - the y coordinate on the screen
 *   int - 1 for mirrored, or 0 for not mirrored
 *   int - indicates which tile from the sprite sheet should be rendered
 *
 */
void sprite_draw_character(eg_app *, int, int, int, int);

void sprite_draw_brick(eg_app *, int, int);

void sprite_draw_grass_block(eg_app *, int, int, int, int);

void sprite_draw_background(eg_app *, int);

#endif