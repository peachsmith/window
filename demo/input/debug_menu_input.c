#include "demo/input/input.h"
#include "demo/menu/menu.h"
#include "demo/entities/entity_types.h"

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

    // Locate the debug menu.
    eg_entity *menu_entity = app->menu_entities[app->menu_count - 1];
    if (menu_entity == NULL)
    {
        return;
    }

    if (eg_consume_input(app, EG_KEYCODE_UP))
    {
        if (menu_entity->cursor_y > 0)
        {
            menu_entity->cursor_y--;
        }
    }

    if (eg_consume_input(app, EG_KEYCODE_DOWN))
    {
        if (menu_entity->cursor_y < 4)
        {
            menu_entity->cursor_y++;
        }
    }

    // menu item selection
    if (eg_consume_input(app, EG_KEYCODE_Z))
    {
        switch (menu_entity->cursor_y)
        {
        case 0:
        {
            eg_entity *scene_menu = app->entities;
            while (scene_menu != NULL && scene_menu->type != ENTITY_TYPE_SCENE_MENU)
            {
                scene_menu = scene_menu->next;
            }

            eg_push_input_handler(app, scene_menu_input_handler);

            scene_menu->cursor_x = 0;
            scene_menu->cursor_y = 0;

            app->menu_entities[app->menu_count++] = scene_menu;
        }
        break;

        case 4:
        {
            eg_entity *input_menu = app->entities;
            while (input_menu != NULL && input_menu->type != ENTITY_TYPE_INPUT_MENU)
            {
                input_menu = input_menu->next;
            }

            eg_push_input_handler(app, input_menu_input_handler);

            input_menu->cursor_x = 0;
            input_menu->cursor_y = 0;

            app->menu_entities[app->menu_count++] = input_menu;
        }
        break;

        default:
            break;
        }
    }

    if (eg_consume_input(app, EG_KEYCODE_LEFT))
    {
        if (menu_entity->cursor_y == 1 && app->debug.frame_len > 1)
        {
            app->debug.frame_len /= 2;
            printf("[DEBUG] frame length: %d\n", app->debug.frame_len);
        }

        if (menu_entity->cursor_y == 2 && app->debug.hitboxes)
        {
            app->debug.hitboxes = 0;
        }

        if (menu_entity->cursor_y == 3 && app->debug.collisions)
        {
            app->debug.collisions = 0;
        }
    }

    if (eg_consume_input(app, EG_KEYCODE_RIGHT))
    {
        if (menu_entity->cursor_y == 1 && app->debug.frame_len < 8)
        {
            app->debug.frame_len *= 2;
            printf("[DEBUG] frame length: %d\n", app->debug.frame_len);
        }

        if (menu_entity->cursor_y == 2 && !(app->debug.hitboxes))
        {
            app->debug.hitboxes = 1;
        }

        if (menu_entity->cursor_y == 3 && !(app->debug.collisions))
        {
            app->debug.collisions = 1;
        }
    }
}