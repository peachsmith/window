#include "demo/input/input.h"
#include "demo/entities/entity_types.h"
#include "demo/menu/menu.h"

void info_menu_input_handler(eg_app *app)
{
    if (eg_consume_input(app, EG_KEYCODE_X))
    {
        app->menu_count--;
        eg_pop_input_handler(app);

        if (app->dialog_count > 0)
        {
            eg_dialog *d = app->dialogs[app->dialog_count - 1];
            d->result = 0;
            d->advance(app, d);
        }

        return;
    }

    if (eg_consume_input(app, EG_KEYCODE_UP))
    {
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

    // menu item selection
    if (eg_consume_input(app, EG_KEYCODE_Z))
    {
        eg_menu *m = app->menus[app->menu_count - 1];

        // Determine which menu item has been selected.
        // Layout:
        // items[0]
        // items[1]
        eg_menu_item *item = m->items[m->cursor.y];

        item->callback(app, m);
    }
}