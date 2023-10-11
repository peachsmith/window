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
void sprite_draw_frank(eg_app *app, int x, int y, int mirror, int tile);
void sprite_draw_jimbo(eg_app *app, int x, int y, int mirror, int tile);
void sprite_draw_billy(eg_app *app, int x, int y, int mirror, int tile);
void sprite_draw_henry(eg_app *app, int x, int y, int mirror, int tile);

void sprite_draw_fireball(eg_app *, int x, int y, int mirror, int tile);


void sprite_draw_slope(eg_app *app, int x, int y, int length, int mirror);
void sprite_draw_flat_slope(eg_app *app, int x, int y, int length);

void sprite_draw_sign(eg_app *app, int x, int y);
void sprite_draw_brick(eg_app *app, int x, int y);

void sprite_draw_grass_block(eg_app *app, int x, int y, int w, int h);

void sprite_draw_background(eg_app *app, int type);

#endif