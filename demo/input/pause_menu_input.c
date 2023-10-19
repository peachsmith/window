#include "demo/demo.h"
#include "demo/input/input.h"
#include "demo/entities/entity_types.h"
#include "demo/entities/player.h"
#include "demo/entities/info_dialog.h"
#include "demo/entities/demo_dialog.h"

void demo_pause_menu_input(cr_app *app)
{
    if (cr_consume_input(app, CR_KEYCODE_X) || cr_consume_input(app, CR_KEYCODE_Q))
    {
        app->menu_count--;
        app->pause = 0;
        cr_pop_input_handler(app);
        return;
    }

    // Locate the pause menu.
    cr_entity *menu_entity = app->menus[app->menu_count - 1];
    if (menu_entity == NULL)
    {
        return;
    }

    if (cr_consume_input(app, CR_KEYCODE_UP))
    {
        if (menu_entity->data > 2)
        {
            menu_entity->data -= 2;
        }
    }

    if (cr_consume_input(app, CR_KEYCODE_DOWN))
    {
        if (menu_entity->data > 0 && menu_entity->data < 3)
        {
            menu_entity->data += 2;
        }
    }

    if (cr_consume_input(app, CR_KEYCODE_LEFT))
    {

        if (menu_entity->data == 2 || menu_entity->data == 4)
        {
            menu_entity->data--;
        }
    }

    if (cr_consume_input(app, CR_KEYCODE_RIGHT))
    {
        if (menu_entity->data == 1 || menu_entity->data == 3)
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
        {
            // Locate the info dialog.
            cr_entity *info_dialog = NULL;
            for (int i = 0; i < app->entity_cap; i++)
            {
                if (app->entities[i].type == ENTITY_TYPE_INFO_DIALOG && app->entities[i].present)
                {
                    info_dialog = &(app->entities[i]);
                }
            }

            demo_open_info_dialog(app, info_dialog);

            cr_push_input_handler(app, demo_common_dialog_input);
        }
        break;

        case 2:
        {
            // Locate the fish menu.
            cr_entity *fish_menu = NULL;
            for (int i = 0; i < app->entity_cap; i++)
            {
                if (app->entities[i].type == ENTITY_TYPE_FISH_MENU && app->entities[i].present)
                {
                    fish_menu = &(app->entities[i]);
                }
            }

            // Set the pause menu as the active menu.
            app->menus[app->menu_count++] = fish_menu;

            cr_push_input_handler(app, demo_fish_menu_input);
        }
        break;

        case 3:
            app->done = 1;
            break;

        case 4:
        {
            // Locate the demo dialog.
            cr_entity *demo_dialog = NULL;
            for (int i = 0; i < app->entity_cap; i++)
            {
                if (app->entities[i].type == ENTITY_TYPE_DEMO_DIALOG && app->entities[i].present)
                {
                    demo_dialog = &(app->entities[i]);
                }
            }

            demo_open_demo_dialog(app, demo_dialog);

            cr_push_input_handler(app, demo_common_dialog_input);
        }
        break;

        default:
            break;
        }
    }
}