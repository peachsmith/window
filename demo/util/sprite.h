#ifndef DEMO_SPRITE_H
#define DEMO_SPRITE_H

#include "crumbs.h"

/**
 * Renders the player character to the screen.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   int - the x coordinate on the screen
 *   int - the y coordinate on the screen
 *   int - 1 for mirrored, or 0 for not mirrored
 *   int - indicates which tile from the sprite sheet should be rendered
 *
 */
void sprite_draw_frank(cr_app *app, int x, int y, int mirror, int tile);
void sprite_draw_jimbo(cr_app *app, int x, int y, int mirror, int tile);
void sprite_draw_billy(cr_app *app, int x, int y, int mirror, int tile);
void sprite_draw_henry(cr_app *app, int x, int y, int mirror, int tile);

void sprite_draw_fireball(cr_app *, int x, int y, int mirror, int tile);


void sprite_draw_slope(cr_app *app, int x, int y, int length, int mirror);
void sprite_draw_flat_slope(cr_app *app, int x, int y, int length);

void sprite_draw_sign(cr_app *app, int x, int y);
void sprite_draw_brick(cr_app *app, int x, int y);

void sprite_draw_grass_block(cr_app *app, int x, int y, int w, int h);

void sprite_draw_background(cr_app *app, int type);

#endif