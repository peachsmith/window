#ifndef DEMO_UI_H
#define DEMO_UI_H

#include "crumbs.h"

#define UI_INDICATOR_ADVANCE 1
#define UI_INDICATOR_SCROLL_UP 2
#define UI_INDICATOR_SCROLL_DOWN 3
#define UI_INDICATOR_SCROLL_LEFT 4
#define UI_INDICATOR_SCROLL_RIGHT 5

/**
 * Draws a panel.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 */
void ui_draw_panel(eg_app *, int, int, int, int);

/**
 * Draws text.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 */
void ui_draw_text(eg_app *);

/**
 * Draws a cursor. This indicates the position of control, such as which
 * menu item will be selected.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 */
void ui_draw_cursor(eg_app *, int, int);

/**
 * Draws an indicator. An indicator signifies that more content will be
 * accessible through user input. For example, a list of items may have
 * an arrow at the bottom indicating that the user can scroll down to see
 * more items.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   int - the x coordinate on the screen
 *   int - the y coordinate on the screen
 *   int - the type of indicator to render
 */
void ui_draw_indicator(eg_app *, int, int, int);

#endif