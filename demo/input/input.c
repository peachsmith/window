#include "demo/input/input.h"
#include "demo/entities/player.h"
#include "demo/entities/entity_types.h"
#include "demo/menu/menu.h"

#include <stdio.h>

void demo_dialog_input_callback(eg_app *app, eg_entity *target)
{
    eg_dialog *d = app->dialogs[app->dialog_count - 1];

    if (eg_consume_input(app, EG_KEYCODE_Z))
    {
        if (d->ticks >= 68)
        {
            app->dialog_count--;
            eg_input_handler *handler = eg_pop_input_handler(app);
            eg_destroy_input_handler(handler);
            return;
        }
    }

    if (eg_consume_input(app, EG_KEYCODE_X))
    {
        if (d->ticks < 68)
        {
            d->ticks = 68;
        }
        else
        {
            app->dialog_count--;
            eg_input_handler *handler = eg_pop_input_handler(app);
            eg_destroy_input_handler(handler);
            return;
        }
    }
}

void fish_input_callback(eg_app *app, eg_entity *target)
{
    if (eg_consume_input(app, EG_KEYCODE_X))
    {
        app->menu_count--;
        eg_input_handler *handler = eg_pop_input_handler(app);
        eg_destroy_input_handler(handler);
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

        item->callback(app, NULL);
    }
}

void pause_input_callback(eg_app *app, eg_entity *target)
{
    if (eg_consume_input(app, EG_KEYCODE_X))
    {
        printf("[DEBUG] resumed\n");
        app->menu_count--;
        app->pause = 0;
        eg_input_handler *handler = eg_pop_input_handler(app);
        eg_destroy_input_handler(handler);
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

void root_input_callback(eg_app *app, eg_entity *target)
{
    if (eg_consume_input(app, EG_KEYCODE_Q))
    {
        printf("[DEBUG] paused\n");

        demo_open_pause_menu(app);

        app->pause = 1;
        eg_input_handler *handler = eg_create_input_handler(pause_input_callback, NULL);
        eg_push_input_handler(app, handler);
        return;
    }

    // left movement
    if (eg_peek_input(app, EG_KEYCODE_LEFT))
    {
        if (target->x_vel >= -2)
        {
            target->x_vel -= 2;
        }
    }

    // right movement
    if (eg_peek_input(app, EG_KEYCODE_RIGHT))
    {
        if (target->x_vel <= 2)
        {
            target->x_vel += 2;
        }
    }

    // Print some empty space for debugging.
    if (eg_consume_input(app, EG_KEYCODE_M))
    {
        puts("");
    }

    // down arrow key
    if (eg_peek_input(app, EG_KEYCODE_DOWN))
    {
        if (!eg_check_flag(target, ENTITY_FLAG_DOWN))
        {
            eg_set_flag(target, ENTITY_FLAG_DOWN);
        }
    }
    else if (eg_check_flag(target, ENTITY_FLAG_DOWN))
    {
        eg_clear_flag(target, ENTITY_FLAG_DOWN);
    }

    // jumping
    if (eg_consume_input(app, EG_KEYCODE_SPACE))
    {
        if (eg_check_flag(target, ENTITY_FLAG_GROUND) &&
            !eg_check_flag(target, ENTITY_FLAG_JUMP))
        {
            eg_clear_flag(target, ENTITY_FLAG_GROUND);
            eg_set_flag(target, ENTITY_FLAG_JUMP);
            eg_clear_flag(target, ENTITY_FLAG_MOVE);
            target->carrier = NULL;
            target->y_vel = 0;
            target->y_vel -= 12;
        }
    }

    // TEMP: Reset the state of various things.
    if (eg_consume_input(app, EG_KEYCODE_R))
    {
        // Reset the target entity (the player).
        target->x_pos = 80;
        target->y_pos = 10;
        target->x_vel = 0;
        target->y_vel = 0;
        target->flags = 0;

        // Reset the camera.
        app->cam.x = 0;
        app->cam.y = 0;
    }

    // If the escape key is pressed, terminate the application.
    if (eg_consume_input(app, EG_KEYCODE_ESCAPE))
    {
        printf("The escape key was pressed. "
               "The application should now terminate.\n");
        app->done = 1;
    }
}
