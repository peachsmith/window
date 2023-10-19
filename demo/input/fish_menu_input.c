#include "demo/demo.h"
#include "demo/input/input.h"
#include "demo/entities/entity_types.h"
#include "demo/entities/player.h"

void demo_fish_menu_input(cr_app *app)
{
    if (cr_consume_input(app, CR_KEYCODE_X))
    {
        app->menu_count--;
        cr_pop_input_handler(app);
        return;
    }

    // Locate the fish menu.
    cr_entity *menu_entity = app->menus[app->menu_count - 1];
    if (menu_entity == NULL)
    {
        return;
    }

    if (cr_consume_input(app, CR_KEYCODE_UP))
    {
        if (menu_entity->data > 1)
        {
            menu_entity->data--;
        }
    }

    if (cr_consume_input(app, CR_KEYCODE_DOWN))
    {
        if (menu_entity->data < 2)
        {
            menu_entity->data++;
        }
    }

    // menu item selection
    if (cr_consume_input(app, CR_KEYCODE_Z))
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
