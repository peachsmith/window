#ifndef EG_DEMO_UI_H
#define EG_DEMO_UI_H

#include "example.h"

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
 */
void ui_draw_indicator(eg_app *);

#endif