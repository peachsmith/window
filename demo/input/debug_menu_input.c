#include "demo/demo.h"
#include "demo/input/input.h"
#include "demo/entities/entity_types.h"

void demo_debug_menu_input(cr_app *app)
{
    if (cr_consume_input(app, CR_KEYCODE_X) ||
        cr_consume_input(app, CR_KEYCODE_Q) ||
        cr_consume_input(app, CR_KEYCODE_ESCAPE))
    {
        app->menu_count--;
        app->pause = 0;
        cr_pop_input_handler(app);
        return;
    }

    // Locate the debug menu.
    cr_entity *menu_entity = app->menus[app->menu_count - 1];
    if (menu_entity == NULL)
    {
        return;
    }

    if (cr_consume_input(app, CR_KEYCODE_UP))
    {
        if (menu_entity->cursor_y > 0)
        {
            menu_entity->cursor_y--;
        }
    }

    if (cr_consume_input(app, CR_KEYCODE_DOWN))
    {
        if (menu_entity->cursor_y < 4)
        {
            menu_entity->cursor_y++;
        }
    }

    // menu item selection
    if (cr_consume_input(app, CR_KEYCODE_Z))
    {
        switch (menu_entity->cursor_y)
        {
        case 0:
        {
            cr_entity *scene_menu = NULL;
            for (int i = 0; i < app->entity_cap; i++)
            {
                if (app->entities[i].type == ENTITY_TYPE_SCENE_MENU && app->entities[i].present)
                {
                    scene_menu = &(app->entities[i]);
                }
            }

            cr_push_input_handler(app, demo_scene_menu_input);

            scene_menu->cursor_x = 0;
            scene_menu->cursor_y = 0;

            app->menus[app->menu_count++] = scene_menu;
        }
        break;

        case 4:
        {
            cr_entity *input_menu = NULL;
            for (int i = 0; i < app->entity_cap; i++)
            {
                if (app->entities[i].type == ENTITY_TYPE_INPUT_MENU && app->entities[i].present)
                {
                    input_menu = &(app->entities[i]);
                }
            }

            cr_push_input_handler(app, demo_input_menu_input);

            input_menu->cursor_x = 0;
            input_menu->cursor_y = 0;

            app->menus[app->menu_count++] = input_menu;
        }
        break;

        default:
            break;
        }
    }

    if (cr_consume_input(app, CR_KEYCODE_LEFT))
    {
        if (menu_entity->cursor_y == 1 && app->debug.frame_len > 1)
        {
            app->debug.frame_len /= 2;
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

    if (cr_consume_input(app, CR_KEYCODE_RIGHT))
    {
        if (menu_entity->cursor_y == 1 && app->debug.frame_len < 8)
        {
            app->debug.frame_len *= 2;
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