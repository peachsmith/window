#include "demo/input/input.h"
#include "demo/entities/player.h"
#include "demo/entities/entity_types.h"
#include "demo/entities/jimbo.h"
#include "demo/entities/billy.h"
#include "demo/entities/henry.h"
#include "demo/entities/fireball.h"
#include "demo/menu/menu.h"
#include "demo/collision/collision.h"
#include "demo/audio/audio.h"
#include "demo/util/util.h"

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
            app->entity_types[dialog->type].advance(app, dialog);
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
            app->entity_types[dialog->type].advance(app, dialog);
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

    //-------------------------------------------------
    // BEGIN player controls

    // Locate the player entity.
    eg_entity *target = app->primary;

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
        else if (app->actuation_counters[EG_KEYCODE_SPACE] > 0 && app->actuation_counters[EG_KEYCODE_SPACE] < 15)
        {
            app->actuation_counters[EG_KEYCODE_SPACE]++;
        }
        else
        {
            // Prevent automatic jumping when holding the spacebar while
            // landing on the ground. This requires that the player's update
            // function not perform any logic for actuation counters above 15.
            app->actuation_counters[EG_KEYCODE_SPACE] = 16;
        }
    }
    else
    {
        app->actuation_counters[EG_KEYCODE_SPACE] = 0;
    }

    // interaction with interactable entities
    if (eg_consume_input(app, EG_KEYCODE_Z))
    {
        // Scan for interactable entities.
        eg_entity *interactable = NULL;
        for (int i = 0; i < app->entity_cap && interactable == NULL; i++)
        {
            if (app->entity_types[app->entities[i].type].interactable && app->entities[i].present)
            {
                // Check for overlap with the interactable entity.
                eg_rect a = {.x = target->x_pos,
                             .y = target->y_pos,
                             .w = app->entity_types[target->type].width,
                             .h = app->entity_types[target->type].height};

                eg_rect b = {.x = app->entities[i].x_pos + app->cam.x,
                             .y = app->entities[i].y_pos + app->cam.y,
                             .w = app->entity_types[app->entities[i].type].width,
                             .h = app->entity_types[app->entities[i].type].height};
                eg_overlap o;

                if (demo_is_overlapped(&a, &b, &o))
                {
                    interactable = &(app->entities[i]);
                }
            }
        }

        if (interactable != NULL)
        {
            app->entity_types[interactable->type].interact(app, interactable, target);
        }
    }

    // TEMP: add Billy.
    if (eg_consume_input(app, EG_KEYCODE_J))
    {
        billy_demo_create(app,
                          target->x_pos + 24 - app->cam.x,
                          target->y_pos - 24 - app->cam.y);
    }

    // TEMP: remove Billy.
    if (eg_consume_input(app, EG_KEYCODE_K))
    {
        eg_entity *b = NULL;
        int offset = 0;
        for (int i = 0; i + offset < app->entity_cap && b == NULL; i++)
        {
            if (app->entities[i].type == ENTITY_TYPE_BILLY && app->entities[i].present)
            {
                b = &(app->entities[i]);
            }
        }

        if (b != NULL)
        {
            eg_remove_entity(app, b);
        }
    }

    // TEMP: add jimbo
    if (eg_consume_input(app, EG_KEYCODE_U))
    {
        jimbo_demo_create(app,
                          target->x_pos + 24 - app->cam.x,
                          target->y_pos - 24 - app->cam.y);
    }

    // TEMP: remove jimbo
    if (eg_consume_input(app, EG_KEYCODE_I))
    {
        eg_entity *b = NULL;
        int offset = 0;
        for (int i = 0; i + offset < app->entity_cap && b == NULL; i++)
        {
            if (app->entities[i].type == ENTITY_TYPE_JIMBO && app->entities[i].present)
            {
                b = &(app->entities[i]);
            }
        }

        if (b != NULL)
        {
            eg_remove_entity(app, b);
        }
    }

    // TEMP: add henry
    if (eg_consume_input(app, EG_KEYCODE_7))
    {
        henry_demo_create(app,
                          target->x_pos + 24 - app->cam.x,
                          target->y_pos - 24 - app->cam.y);
    }

    // TEMP: remove henry
    if (eg_consume_input(app, EG_KEYCODE_8))
    {
        eg_entity *b = NULL;
        int offset = 0;
        for (int i = 0; i + offset < app->entity_cap && b == NULL; i++)
        {
            if (app->entities[i].type == ENTITY_TYPE_HENRY && app->entities[i].present)
            {
                b = &(app->entities[i]);
            }
        }

        if (b != NULL)
        {
            eg_remove_entity(app, b);
        }
    }

    // lanuch projectile
    if (eg_consume_input(app, EG_KEYCODE_X))
    {
        eg_entity *f = fireball_demo_create(
            app,
            target->x_pos - app->cam.x,
            target->y_pos - app->cam.y + 4);
        if (f != NULL)
        {
            if (eg_check_flag(target, ENTITY_FLAG_MIRROR))
            {
                f->x_acc = 1;
            }
            else
            {
                f->x_acc = -1;
                eg_set_flag(f, ENTITY_FLAG_MIRROR);
            }
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

    // The V and B keys will move the player left or right.
    if (eg_consume_input(app, EG_KEYCODE_V))
    {
        target->x_pos -= 1;
    }
    if (eg_consume_input(app, EG_KEYCODE_B))
    {
        target->x_pos += 1;
    }

    // The [ and ] keys will toggle the rendering of hitboxes and camera
    // boundaries respectively
    if (eg_consume_input(app, EG_KEYCODE_LEFTBRACKET))
    {
        app->debug.hitboxes = app->debug.hitboxes ? 0 : 1;
    }
    if (eg_consume_input(app, EG_KEYCODE_RIGHTBRACKET))
    {
        app->debug.camera = app->debug.camera ? 0 : 1;
    }

    // The ' key will toggle the camera configuration
    if (eg_consume_input(app, EG_KEYCODE_APOSTROPHE))
    {
        app->cam.config = app->cam.config == EG_CAMERA_ALL ? EG_CAMERA_NONE : EG_CAMERA_ALL;
    }

    // END player controls
    //-------------------------------------------------
}
