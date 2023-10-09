#include "demo/input/input.h"
#include "demo/entities/corgi.h"
#include "demo/entities/note.h"
#include "demo/entities/entity_types.h"
#include "demo/menu/menu.h"
#include "demo/collision/collision.h"
#include "demo/audio/audio.h"
#include "demo/util/util.h"
#include "demo/menu/menu.h"
#include "demo/demo.h"

#include <stdio.h>

// input handlers for Toot n Sploot

void tns_forest_input_handler(eg_app *app)
{
    // Pause the application.
    if (eg_consume_input(app, EG_KEYCODE_Q))
    {
        // Locate the pause menu.
        eg_entity *pause_menu = NULL;
        for (int i = 0; i < app->entity_cap; i++)
        {
            if (app->entities[i].type == ENTITY_TYPE_PAUSE_MENU && app->entities[i].present)
            {
                pause_menu = &(app->entities[i]);
            }
        }

        // Set the pause menu as the active menu.
        app->menus[app->menu_count++] = pause_menu;

        eg_push_input_handler(app, pause_menu_input_handler);

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

    //-------------------------------------------------
    // BEGIN player controls

    // Locate the player entity.
    eg_entity *target = app->primary;
    if (target == NULL)
    {
        return;
    }

    int splooting = app->actuation_counters[EG_KEYCODE_SPACE] >= 20;

    int max_walk = 6;
    int min_walk = 1;
    int walk_acc = 1;

    // left movement
    if (eg_peek_input(app, EG_KEYCODE_LEFT) && !splooting)
    {
        if (app->actuation_counters[EG_KEYCODE_LEFT] < 10)
        {
            app->actuation_counters[EG_KEYCODE_LEFT]++;
        }

        eg_clear_flag(target, ENTITY_FLAG_MIRROR);

        if (target->x_acc > -min_walk)
        {
            if (target->x_acc < min_walk)
            {
                target->x_acc = -min_walk;
            }
        }
        else
        {
            target->x_acc -= walk_acc;
        }

        if (target->x_acc < -max_walk)
        {
            target->x_acc = -max_walk;
        }
    }
    else
    {
        app->actuation_counters[EG_KEYCODE_LEFT] = 0;
    }

    // right movement
    if (eg_peek_input(app, EG_KEYCODE_RIGHT) && !splooting)
    {
        if (app->actuation_counters[EG_KEYCODE_RIGHT] < 10)
        {
            app->actuation_counters[EG_KEYCODE_RIGHT]++;
        }

        eg_set_flag(target, ENTITY_FLAG_MIRROR);

        if (target->x_acc < min_walk)
        {
            if (target->x_acc > -min_walk)
            {
                target->x_acc = min_walk;
            }
        }
        else
        {
            target->x_acc += walk_acc;
        }

        if (target->x_acc > max_walk)
        {
            target->x_acc = max_walk;
        }
    }
    else
    {
        app->actuation_counters[EG_KEYCODE_RIGHT] = 0;
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
    if (eg_peek_input(app, EG_KEYCODE_SPACE))
    {
        if (eg_check_flag(target, ENTITY_FLAG_GROUND) &&
            !eg_check_flag(target, ENTITY_FLAG_JUMP) &&
            app->actuation_counters[EG_KEYCODE_SPACE] == 0)
        {
            eg_clear_flag(target, ENTITY_FLAG_GROUND);
            eg_set_flag(target, ENTITY_FLAG_JUMP);
            eg_clear_flag(target, ENTITY_FLAG_MOVE);

            // Clear the carrier and the correction factor.
            target->carrier = NULL;
            target->y_t = 0;

            // Set the vertical acceleration to some significant negative
            // value to launch the entity upward.
            target->y_acc = -13;

            app->actuation_counters[EG_KEYCODE_SPACE]++;
        }

        // Increase the actuation counter to allow jump height control in the
        // target entity's update function.
        else if (app->actuation_counters[EG_KEYCODE_SPACE] < 20)
        {
            app->actuation_counters[EG_KEYCODE_SPACE]++;
        }
    }
    else
    {
        app->actuation_counters[EG_KEYCODE_SPACE] = 0;
    }

    // play a musical note
    if (eg_consume_input(app, EG_KEYCODE_X) && !splooting)
    {
        // Limit the number of notes to 3 at any given time.
        if (app->counters[DEMO_COUNTER_BREATH] > 0)
        {
            eg_entity *f = tns_create_note(
                app,
                target->x_pos - app->cam.x,
                target->y_pos - app->cam.y + 4);
            if (f != NULL)
            {
                if (eg_check_flag(target, ENTITY_FLAG_MIRROR))
                {
                    f->x_pos += app->entity_types[target->type].width / 2;
                    f->x_acc = 16;
                }
                else
                {
                    f->x_acc = -16;
                    eg_set_flag(f, ENTITY_FLAG_MIRROR);
                }
            }

            app->counters[DEMO_COUNTER_BREATH]--;
        }
    }

    // END player controls
    //-------------------------------------------------

    //-------------------------------------------------
    // BEGIN debug controls

    // If the escape key is pressed, terminate the application.
    if (eg_consume_input(app, EG_KEYCODE_GRAVE))
    {
        if (eg_peek_input(app, EG_KEYCODE_LSHIFT) || eg_peek_input(app, EG_KEYCODE_RSHIFT))
        {
            // Locate the pause menu.
            eg_entity *debug_menu = NULL;
            for (int i = 0; i < app->entity_cap; i++)
            {
                if (app->entities[i].type == ENTITY_TYPE_DEBUG_MENU && app->entities[i].present)
                {
                    debug_menu = &(app->entities[i]);
                }
            }

            // Set the pause menu as the active menu.
            app->menus[app->menu_count++] = debug_menu;

            eg_push_input_handler(app, debug_menu_input_handler);

            app->pause = 1;
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

    if (eg_consume_input(app, EG_KEYCODE_LEFTBRACKET))
    {
        app->debug.hitboxes = app->debug.hitboxes ? 0 : 1;
    }

    // END debug controls
    //-------------------------------------------------
}
