#include "demo/input/input.h"
#include "demo/entities/player.h"
#include "demo/entities/entity_types.h"
#include "demo/menu/menu.h"

#include <stdlib.h>

void pause_menu_input_handler(eg_app *app)
{
    if (eg_consume_input(app, EG_KEYCODE_X) || eg_consume_input(app, EG_KEYCODE_Q))
    {
        app->menu_count--;
        app->pause = 0;
        eg_pop_input_handler(app);
        return;
    }

    if (eg_consume_input(app, EG_KEYCODE_UP))
    {
        // Get the menu at the top of the stack.
        eg_menu *m = app->menus[app->menu_count - 1];

        if (m->cursor.y > 0)
        {
            m->cursor.y--;
        }
    }

    if (eg_consume_input(app, EG_KEYCODE_DOWN))
    {
        eg_menu *m = app->menus[app->menu_count - 1];

        if (m->cursor.y < 1)
        {
            m->cursor.y++;
        }
    }

    if (eg_consume_input(app, EG_KEYCODE_LEFT))
    {
        eg_menu *m = app->menus[app->menu_count - 1];

        if (m->cursor.x > 0)
        {
            m->cursor.x--;
        }
    }

    if (eg_consume_input(app, EG_KEYCODE_RIGHT))
    {
        eg_menu *m = app->menus[app->menu_count - 1];

        if (m->cursor.x < 1)
        {
            m->cursor.x++;
        }
    }

    // menu item selection
    if (eg_consume_input(app, EG_KEYCODE_Z))
    {
        eg_menu *m = app->menus[app->menu_count - 1];

        // Determine which menu item has been selected.
        // Layout:
        // items[0]  items[2]
        // items[1]  items[3]
        eg_menu_item *item = m->items[m->cursor.y + 2 * m->cursor.x];

        item->callback(app, NULL);
    }
}