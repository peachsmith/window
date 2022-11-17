#include "demo/input/input.h"
#include "demo/entities/player.h"
#include "demo/entities/entity_types.h"
#include "demo/entities/jimbo.h"
#include "demo/entities/billy.h"
#include "demo/menu/menu.h"
#include "demo/collision/collision.h"
#include "demo/audio/audio.h"

#include <stdio.h>

static eg_callback input_handlers[20];

void demo_init_input(eg_app *app)
{
    app->input = &(input_handlers[0]);
}

void common_dialog_input_handler(eg_app *app)
{
    // Locate the active dialog.
    eg_entity *dialog = app->dialogs[app->dialog_count - 1];
    if (dialog == NULL)
    {
        return;
    }

    if (eg_consume_input(app, EG_KEYCODE_Z))
    {
        if (dialog->ticks >= dialog->tick_limit)
        {
            app->registry[dialog->type].advance(app, dialog);
            return;
        }
    }

    if (eg_consume_input(app, EG_KEYCODE_X))
    {
        if (dialog->ticks < dialog->tick_limit)
        {
            dialog->ticks = dialog->tick_limit;
        }
        else
        {
            app->registry[dialog->type].advance(app, dialog);
        }
    }
}

void empty_input_handler(eg_app *app)
{
}

void root_input_handler(eg_app *app)
{
    // Pause the application.
    if (eg_consume_input(app, EG_KEYCODE_Q))
    {
        // Locate the pause menu.
        eg_entity *pause_menu = app->entities;
        while (pause_menu != NULL && pause_menu->type != ENTITY_TYPE_PAUSE_MENU)
        {
            pause_menu = pause_menu->next;
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

    // If the escape key is pressed, terminate the application.
    if (eg_consume_input(app, EG_KEYCODE_GRAVE))
    {
        if (eg_peek_input(app, EG_KEYCODE_LSHIFT) || eg_peek_input(app, EG_KEYCODE_RSHIFT))
        {
            // Locate the pause menu.
            eg_entity *debug_menu = app->entities;
            while (debug_menu != NULL && debug_menu->type != ENTITY_TYPE_DEBUG_MENU)
            {
                debug_menu = debug_menu->next;
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

    //-------------------------------------------------
    // BEGIN player controls

    // TODO: remove or replace this once searchable entities have been implemented.
    eg_entity *target = app->player;

    // TODO: implement a way to locate target entities during input handling.

    int max_walk = 6;
    int min_walk = 1;
    int walk_acc = 1;

    // left movement
    if (eg_peek_input(app, EG_KEYCODE_LEFT))
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
    if (eg_peek_input(app, EG_KEYCODE_RIGHT))
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

            // Clear the carrier and the correction factor.
            target->carrier = NULL;
            target->y_t = 0;

            // Set the vertical accleration to some significant negative
            // value to launch the entity upward.
            target->y_acc = -18;
        }
    }

    // interaction with interactable entities
    if (eg_consume_input(app, EG_KEYCODE_Z))
    {
        // Scan for interactable entities.
        eg_entity *interactable = NULL;
        eg_entity *source = app->entities;
        while (source != NULL && interactable == NULL)
        {
            if (app->registry[source->type].interactable)
            {
                // Check for overlap with the interactable entity.
                eg_rect a = {.x = target->x_pos,
                             .y = target->y_pos,
                             .w = app->registry[target->type].width,
                             .h = app->registry[target->type].height};

                eg_rect b = {.x = source->x_pos + app->cam.x,
                             .y = source->y_pos + app->cam.y,
                             .w = app->registry[source->type].width,
                             .h = app->registry[source->type].height};
                eg_overlap o;

                if (demo_is_overlapped(&a, &b, &o))
                {
                    interactable = source;
                }
            }

            source = source->next;
        }

        if (interactable != NULL)
        {
            app->registry[interactable->type].interact(app, interactable, target);
        }
    }

    // TEMP: add Billy.
    if (eg_consume_input(app, EG_KEYCODE_J))
    {
        eg_add_entity(app, billy_demo_create(
                               target->x_pos + 24 - app->cam.x,
                               target->y_pos - 24 - app->cam.y));
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

    // TEMP: Play audio
    if (eg_consume_input(app, EG_KEYCODE_F))
    {
        eg_play_sound(app, app->sounds[DEMO_SOUND_EFFECT_CONFIRMATION]);
    }
    if (eg_consume_input(app, EG_KEYCODE_G))
    {
        eg_play_sound(app, app->sounds[DEMO_SOUND_EFFECT_DROP]);
    }
    if (eg_consume_input(app, EG_KEYCODE_H))
    {
        eg_play_sound(app, app->sounds[DEMO_SOUND_EFFECT_TOGGLE]);
    }

    // END player controls
    //-------------------------------------------------
}
