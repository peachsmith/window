#include "demo/input/input.h"
#include "demo/entities/player.h"
#include "demo/entities/entity_types.h"
#include "demo/menu/menu.h"

#include <stdio.h>

void fish_menu_input_handler(eg_app *app)
{
    if (eg_consume_input(app, EG_KEYCODE_X))
    {
        app->menu_count--;
        eg_pop_input_handler(app);
        return;
    }

    // Locate the fish menu.
    eg_entity *menu_entity = app->menus[app->menu_count - 1];
    if (menu_entity == NULL)
    {
        return;
    }

    if (eg_consume_input(app, EG_KEYCODE_UP))
    {
        if (menu_entity->data > 1)
        {
            menu_entity->data--;
        }
    }

    if (eg_consume_input(app, EG_KEYCODE_DOWN))
    {
        if (menu_entity->data < 2)
        {
            menu_entity->data++;
        }
    }

    // menu item selection
    if (eg_consume_input(app, EG_KEYCODE_Z))
    {
        switch (menu_entity->data)
        {
        case 1:
            printf("trout has been selected\n");
            break;

        case 2:
            printf("salmon has been selected\n");
            break;

        default:
            break;
        }
    }
}
