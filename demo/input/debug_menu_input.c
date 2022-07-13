#include "demo/input/input.h"
#include "demo/menu/menu.h"

#include <stdio.h>

void debug_menu_input_handler(eg_app *app)
{
    if (eg_consume_input(app, EG_KEYCODE_X) ||
        eg_consume_input(app, EG_KEYCODE_Q) ||
        eg_consume_input(app, EG_KEYCODE_ESCAPE))
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

        if (m->cursor.y < 4)
        {
            m->cursor.y++;
        }
    }

    // menu item selection
    if (eg_consume_input(app, EG_KEYCODE_Z))
    {
        eg_menu *m = app->menus[app->menu_count - 1];

        // Determine which menu item has been selected.
        // Layout:
        // items[0]
        // items[1]
        // items[2]
        // items[3]
        // items[4]
        eg_menu_item *item = m->items[m->cursor.y];

        if (item->callback != NULL)
        {
            item->callback(app, NULL);
        }
    }

    if (eg_consume_input(app, EG_KEYCODE_LEFT))
    {
        eg_menu *m = app->menus[app->menu_count - 1];

        if (m->cursor.y == 1 && app->debug.frame_len > 1)
        {
            app->debug.frame_len /= 2;
            printf("[DEBUG] frame length: %d\n", app->debug.frame_len);
        }

        if (m->cursor.y == 2 && app->debug.hitboxes)
        {
            app->debug.hitboxes = 0;
        }

        if (m->cursor.y == 3 && app->debug.collisions)
        {
            app->debug.collisions = 0;
        }
    }

    if (eg_consume_input(app, EG_KEYCODE_RIGHT))
    {
        eg_menu *m = app->menus[app->menu_count - 1];

        if (m->cursor.y == 1 && app->debug.frame_len < 8)
        {
            app->debug.frame_len *= 2;
            printf("[DEBUG] frame length: %d\n", app->debug.frame_len);
        }

        if (m->cursor.y == 2 && !(app->debug.hitboxes))
        {
            app->debug.hitboxes = 1;
        }

        if (m->cursor.y == 3 && !(app->debug.collisions))
        {
            app->debug.collisions = 1;
        }
    }
}