#ifndef TNS_SPRITE_H
#define TNS_SPRITE_H

#include "crumbs.h"

/**
 * Renders the corgi character.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   int - the x coordinate on the screen
 *   int - the y coordinate on the screen
 *   int - 1 for mirrored, or 0 for not mirrored
 *   int - indicates which tile from the sprite sheet should be rendered
 */
void sprite_draw_corgi(eg_app *app, int x, int y, int mirror, int tile);

/**
 * Renders a musical note.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   int - the x coordinate on the screen
 *   int - the y coordinate on the screen
 */
void sprite_draw_note(eg_app *app, int x, int y);

/**
 * Renders a critter.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   int - the x coordinate on the screen
 *   int - the y coordinate on the screen
 *   int - 1 for mirrored, or 0 for not mirrored
 *   int - indicates which tile from the sprite sheet should be rendered
 */
void sprite_draw_critter(eg_app *app, int x, int y, int mirror, int tile);

/**
 * Renders a sparkling effect.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   int - the x coordinate on the screen
 *   int - the y coordinate on the screen
 *   int - indicates which tile from the sprite sheet should be rendered
 */
void sprite_draw_sparkle(eg_app *app, int x, int y, int tile);

/**
 * Renders a puff of smoke.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   int - the x coordinate on the screen
 *   int - the y coordinate on the screen
 *   int - indicates which tile from the sprite sheet should be rendered
 */
void sprite_draw_puff(eg_app *app, int x, int y, int tile);

/**
 * Renders some leaves rustling in the trees.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   int - the x coordinate on the screen
 *   int - the y coordinate on the screen
 *   int - indicates which tile from the sprite sheet should be rendered
 */
void sprite_draw_leaves(eg_app *app, int x, int y, int tile);

/**
 * Renders a tiny trumpet.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   int - the x coordinate on the screen
 *   int - the y coordinate on the screen
 *   int - 1 for mirrored, or 0 for not mirrored
 */
void sprite_draw_trumpet(eg_app *app, int x, int y, int mirror);

/**
 * Renders a piece of the forest floor.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   int - the x coordinate on the screen
 *   int - the y coordinate on the screen
 */
void sprite_draw_ground(eg_app *app, int x, int y);

/**
 * Renders the forest background.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   int - the x coordinate on the screen
 *   int - the y coordinate on the screen
 */
void sprite_draw_forest(eg_app *app, int x, int y);

#endif