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

    // Get the menu at the top of the stack.
    // We assume this will be the pause menu in this context.
    eg_menu *m = app->menus[app->menu_count - 1];
    eg_entity *menu_entity = app->entities;
    while (menu_entity != NULL && menu_entity->type != ENTITY_TYPE_MENU)
    {
        menu_entity = menu_entity->next;
    }

    // The data field will temporarily be used to represent the cursor
    // position in the pause menu entity.
    int data = 0;
    if (menu_entity != NULL)
    {
        data = menu_entity->data;
    }

    if (eg_consume_input(app, EG_KEYCODE_UP))
    {
        if (m->cursor.y > 0)
        {
            m->cursor.y--;
        }

        if (data > 2)
        {
            menu_entity->data -= 2;
        }
    }

    if (eg_consume_input(app, EG_KEYCODE_DOWN))
    {
        if (m->cursor.y < 1)
        {
            m->cursor.y++;
        }

        if (data > 0 && data < 3)
        {
            menu_entity->data += 2;
        }
    }

    if (eg_consume_input(app, EG_KEYCODE_LEFT))
    {
        if (m->cursor.x > 0)
        {
            m->cursor.x--;
        }

        if (data == 2 || data == 4)
        {
            menu_entity->data--;
        }
    }

    if (eg_consume_input(app, EG_KEYCODE_RIGHT))
    {
        if (m->cursor.x < 1)
        {
            m->cursor.x++;
        }

        if (data == 1 || data == 3)
        {
            menu_entity->data++;
        }
    }

    // menu item selection
    if (eg_consume_input(app, EG_KEYCODE_Z))
    {
        // Determine which menu item has been selected.
        // Layout:
        // items[0]  items[2]
        // items[1]  items[3]
        eg_menu_item *item = m->items[m->cursor.y + 2 * m->cursor.x];

        item->callback(app, NULL);
    }
}