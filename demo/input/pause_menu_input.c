#include "demo/input/input.h"
#include "demo/entities/player.h"
#include "demo/entities/entity_types.h"
#include "demo/entities/info_dialog.h"
#include "demo/entities/demo_dialog.h"
#include "demo/menu/menu.h"

#include <stdlib.h>
#include <stdio.h>

void pause_menu_input_handler(eg_app *app)
{
    if (eg_consume_input(app, EG_KEYCODE_X) || eg_consume_input(app, EG_KEYCODE_Q))
    {
        app->menu_count--;
        app->pause = 0;
        eg_pop_input_handler(app);
        return;
    }

    // Locate the pause menu.
    eg_entity *menu_entity = app->entities;
    while (menu_entity != NULL && menu_entity->type != ENTITY_TYPE_PAUSE_MENU)
    {
        menu_entity = menu_entity->next;
    }

    if (menu_entity == NULL)
    {
        return;
    }

    if (eg_consume_input(app, EG_KEYCODE_UP))
    {
        if (menu_entity->data > 2)
        {
            menu_entity->data -= 2;
        }
    }

    if (eg_consume_input(app, EG_KEYCODE_DOWN))
    {
        if (menu_entity->data > 0 && menu_entity->data < 3)
        {
            menu_entity->data += 2;
        }
    }

    if (eg_consume_input(app, EG_KEYCODE_LEFT))
    {

        if (menu_entity->data == 2 || menu_entity->data == 4)
        {
            menu_entity->data--;
        }
    }

    if (eg_consume_input(app, EG_KEYCODE_RIGHT))
    {
        if (menu_entity->data == 1 || menu_entity->data == 3)
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
        {
            // Locate the info dialog.
            eg_entity *info_dialog = app->entities;
            while (info_dialog != NULL && info_dialog->type != ENTITY_TYPE_INFO_DIALOG)
            {
                info_dialog = info_dialog->next;
            }

            info_dialog_demo_open(app, info_dialog);

            eg_push_input_handler(app, common_dialog_input_handler);
        }
        break;

        case 2:
        {
            // Locate the fish menu.
            eg_entity *fish_menu = app->entities;
            while (fish_menu != NULL && fish_menu->type != ENTITY_TYPE_FISH_MENU)
            {
                fish_menu = fish_menu->next;
            }

            // Set the pause menu as the active menu.
            app->menu_entities[app->menu_count++] = fish_menu;

            eg_push_input_handler(app, fish_menu_input_handler);
        }
        break;

        case 3:
            app->done = 1;
            break;

        case 4:
        {
            // Locate the demo dialog.
            eg_entity *demo_dialog = app->entities;
            while (demo_dialog != NULL && demo_dialog->type != ENTITY_TYPE_DEMO_DIALOG)
            {
                demo_dialog = demo_dialog->next;
            }

            demo_dialog_demo_open(app, demo_dialog);

            eg_push_input_handler(app, common_dialog_input_handler);
        }
        break;

        default:
            break;
        }
    }
}