#include "demo/input/input.h"
#include "demo/entities/player.h"
#include "demo/entities/entity_types.h"
#include "demo/menu/menu.h"

#include <stdio.h>

static eg_callback input_handlers[20];

void demo_init_input(eg_app *app)
{
    app->input = &(input_handlers[0]);
}

void demo_dialog_input_handler(eg_app *app)
{
    eg_dialog *d = app->dialogs[app->dialog_count - 1];

    if (eg_consume_input(app, EG_KEYCODE_Z))
    {
        if (d->ticks >= d->tick_limit)
        {
            d->advance(app, d);
            return;
        }
    }

    if (eg_consume_input(app, EG_KEYCODE_X))
    {
        if (d->ticks < d->tick_limit)
        {
            d->ticks = d->tick_limit;
        }
        else
        {
            d->advance(app, d);
        }
    }
}

void fish_menu_input_handler(eg_app *app)
{
    if (eg_consume_input(app, EG_KEYCODE_X))
    {
        app->menu_count--;
        eg_pop_input_handler(app);
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

void pause_menu_input_handler(eg_app *app)
{
    if (eg_consume_input(app, EG_KEYCODE_X) || eg_consume_input(app, EG_KEYCODE_Q))
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

void root_input_handler(eg_app *app)
{

    // Pause the application.
    if (eg_consume_input(app, EG_KEYCODE_Q))
    {
        demo_open_pause_menu(app);

        app->pause = 1;
        return;
    }

    // Print some empty space for debugging.
    if (eg_consume_input(app, EG_KEYCODE_M))
    {
        puts("");
    }

    // If the escape key is pressed, terminate the application.
    if (eg_consume_input(app, EG_KEYCODE_ESCAPE))
    {
        printf("The escape key was pressed. "
               "The application should now terminate.\n");
        app->done = 1;
    }

    // If the escape key is pressed, terminate the application.
    if (eg_consume_input(app, EG_KEYCODE_GRAVE))
    {
        if (eg_peek_input(app, EG_KEYCODE_LSHIFT) || eg_peek_input(app, EG_KEYCODE_RSHIFT))
        {
            demo_open_debug_menu(app);
            app->pause = 1;
            return;
        }
        else
        {
            if (app->debug.overlay)
            {
                app->debug.overlay = 0;
            }
            else
            {
                app->debug.overlay = 1;
            }
        }
    }

    //-------------------------------------------------
    // BEGIN player controls

    // TODO: remove or replace this once searchable entities have been implemented.
    eg_entity *target = app->player;

    // TODO: implement a way to locate target entities during input handling.

    // left movement
    if (eg_peek_input(app, EG_KEYCODE_LEFT))
    {
        eg_clear_flag(target, ENTITY_FLAG_MIRROR);
        if (target->x_vel >= -2)
        {
            target->x_vel -= 2;
        }
    }

    // right movement
    if (eg_peek_input(app, EG_KEYCODE_RIGHT))
    {
        eg_set_flag(target, ENTITY_FLAG_MIRROR);
        if (target->x_vel <= 2)
        {
            target->x_vel += 2;
        }
    }

    // if (eg_peek_input(app, EG_KEYCODE_UP))
    // {
    //     if (target->y_vel >= -2)
    //     {
    //         target->y_vel -= 2;
    //     }
    // }

    // if (eg_peek_input(app, EG_KEYCODE_DOWN))
    // {
    //     if (target->y_vel <= 2)
    //     {
    //         target->y_vel += 2;
    //     }
    // }

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

    // TEMP
    // The following W, A, S, D controls are used for debugging
    // sprite sheets.
    // if (eg_consume_input(app, EG_KEYCODE_A)) // && target->sprite_x > 0)
    // {
    //     // target->sprite_x--;
    //     app->registry[ENTITY_TYPE_PLAYER].width--;
    // }

    // if (eg_consume_input(app, EG_KEYCODE_D)) // && target->sprite_x < 19)
    // {
    //     // target->sprite_x++;
    //     app->registry[ENTITY_TYPE_PLAYER].width++;
    // }

    // if (eg_consume_input(app, EG_KEYCODE_W)) // && target->sprite_y > 0)
    // {
    //     // target->sprite_y--;
    //     app->registry[ENTITY_TYPE_PLAYER].height++;
    // }

    // if (eg_consume_input(app, EG_KEYCODE_S)) // && target->sprite_y < 8)
    // {
    //     // target->sprite_y++;
    //     app->registry[ENTITY_TYPE_PLAYER].height--;
    // }

    // END player controls
    //-------------------------------------------------
}
