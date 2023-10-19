#include "demo/demo.h"
#include "demo/assets.h"
#include "demo/input/input.h"
#include "demo/entities/entity_types.h"
#include "demo/entities/player.h"
#include "demo/entities/jimbo.h"
#include "demo/entities/billy.h"
#include "demo/entities/henry.h"
#include "demo/entities/fireball.h"

#include "common/util.h"
#include "common/collision.h"

void demo_root_input(cr_app *app)
{
    // Pause the application.
    if (cr_consume_input(app, CR_KEYCODE_Q))
    {
        // Locate the pause menu.
        cr_entity *pause_menu = NULL;
        for (int i = 0; i < app->entity_cap; i++)
        {
            if (app->entities[i].type == ENTITY_TYPE_PAUSE_MENU && app->entities[i].present)
            {
                pause_menu = &(app->entities[i]);
            }
        }

        // Set the pause menu as the active menu.
        app->menus[app->menu_count++] = pause_menu;

        cr_push_input_handler(app, demo_pause_menu_input);

        app->pause = 1;
        return;
    }

    // Print some empty space for debugging.
    if (cr_consume_input(app, CR_KEYCODE_M))
    {
        puts("");
    }

    // If the escape key is pressed, terminate the application.
    if (cr_consume_input(app, CR_KEYCODE_ESCAPE))
    {
        app->done = 1;
    }

    // If the escape key is pressed, terminate the application.
    if (cr_consume_input(app, CR_KEYCODE_GRAVE))
    {
        if (cr_peek_input(app, CR_KEYCODE_LSHIFT) || cr_peek_input(app, CR_KEYCODE_RSHIFT))
        {
            // Locate the pause menu.
            cr_entity *debug_menu = NULL;
            for (int i = 0; i < app->entity_cap; i++)
            {
                if (app->entities[i].type == ENTITY_TYPE_DEBUG_MENU && app->entities[i].present)
                {
                    debug_menu = &(app->entities[i]);
                }
            }

            // Set the pause menu as the active menu.
            app->menus[app->menu_count++] = debug_menu;

            cr_push_input_handler(app, demo_debug_menu_input);

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
    cr_entity *target = app->extension->entity_handles[DEMO_HANDLE_PLAYER];

    int max_walk = 6;
    int min_walk = 1;
    int walk_acc = 1;

    // left movement
    if (cr_peek_input(app, CR_KEYCODE_LEFT))
    {
        if (app->actuation_counters[CR_KEYCODE_LEFT] < 10)
        {
            app->actuation_counters[CR_KEYCODE_LEFT]++;
        }

        cr_clear_flag(target, ENTITY_FLAG_MIRROR);

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
        app->actuation_counters[CR_KEYCODE_LEFT] = 0;
    }

    // right movement
    if (cr_peek_input(app, CR_KEYCODE_RIGHT))
    {
        if (app->actuation_counters[CR_KEYCODE_RIGHT] < 10)
        {
            app->actuation_counters[CR_KEYCODE_RIGHT]++;
        }

        cr_set_flag(target, ENTITY_FLAG_MIRROR);

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
        app->actuation_counters[CR_KEYCODE_RIGHT] = 0;
    }

    // down arrow key
    if (cr_peek_input(app, CR_KEYCODE_DOWN))
    {
        if (!cr_check_flag(target, ENTITY_FLAG_DOWN))
        {
            cr_set_flag(target, ENTITY_FLAG_DOWN);
        }
    }
    else if (cr_check_flag(target, ENTITY_FLAG_DOWN))
    {
        cr_clear_flag(target, ENTITY_FLAG_DOWN);
    }

    // jumping
    if (cr_peek_input(app, CR_KEYCODE_SPACE))
    {
        if (cr_check_flag(target, ENTITY_FLAG_GROUND) &&
            !cr_check_flag(target, ENTITY_FLAG_JUMP) &&
            app->actuation_counters[CR_KEYCODE_SPACE] == 0)
        {
            cr_clear_flag(target, ENTITY_FLAG_GROUND);
            cr_set_flag(target, ENTITY_FLAG_JUMP);
            cr_clear_flag(target, ENTITY_FLAG_MOVE);

            // Clear the carrier and the correction factor.
            target->carrier = NULL;
            target->y_t = 0;

            // Set the vertical acceleration to some significant negative
            // value to launch the entity upward.
            target->y_acc = -13;

            app->actuation_counters[CR_KEYCODE_SPACE]++;
        }

        // Increase the actuation counter to allow jump height control in the
        // target entity's update function.
        else if (app->actuation_counters[CR_KEYCODE_SPACE] > 0 && app->actuation_counters[CR_KEYCODE_SPACE] < 15)
        {
            app->actuation_counters[CR_KEYCODE_SPACE]++;
        }
        else
        {
            // Prevent automatic jumping when holding the spacebar while
            // landing on the ground. This requires that the player's update
            // function not perform any logic for actuation counters above 15.
            app->actuation_counters[CR_KEYCODE_SPACE] = 16;
        }
    }
    else
    {
        app->actuation_counters[CR_KEYCODE_SPACE] = 0;
    }

    // interaction with interactable entities
    if (cr_consume_input(app, CR_KEYCODE_Z))
    {
        // Scan for interactable entities.
        cr_entity *interactable = NULL;
        for (int i = 0; i < app->entity_cap && interactable == NULL; i++)
        {
            if (app->entity_types[app->entities[i].type].interactable && app->entities[i].present)
            {
                // Check for overlap with the interactable entity.
                cr_rect a = {.x = target->x_pos,
                             .y = target->y_pos,
                             .w = app->entity_types[target->type].width,
                             .h = app->entity_types[target->type].height};

                cr_rect b = {.x = app->entities[i].x_pos + app->cam.x,
                             .y = app->entities[i].y_pos + app->cam.y,
                             .w = app->entity_types[app->entities[i].type].width,
                             .h = app->entity_types[app->entities[i].type].height};
                cr_overlap o;

                if (common_is_overlapped(&a, &b, &o))
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
    if (cr_consume_input(app, CR_KEYCODE_J))
    {
        demo_create_billy(app,
                          target->x_pos + 24 - app->cam.x,
                          target->y_pos - 24 - app->cam.y);
    }

    // TEMP: remove Billy.
    if (cr_consume_input(app, CR_KEYCODE_K))
    {
        cr_entity *b = NULL;
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
            cr_remove_entity(app, b);
        }
    }

    // TEMP: add jimbo
    if (cr_consume_input(app, CR_KEYCODE_U))
    {
        demo_create_jimbo(app,
                          target->x_pos + 24 - app->cam.x,
                          target->y_pos - 24 - app->cam.y);
    }

    // TEMP: remove jimbo
    if (cr_consume_input(app, CR_KEYCODE_I))
    {
        cr_entity *b = NULL;
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
            cr_remove_entity(app, b);
        }
    }

    // TEMP: add henry
    if (cr_consume_input(app, CR_KEYCODE_7))
    {
        demo_create_henry(app,
                          target->x_pos + 24 - app->cam.x,
                          target->y_pos - 24 - app->cam.y);
    }

    // TEMP: remove henry
    if (cr_consume_input(app, CR_KEYCODE_8))
    {
        cr_entity *b = NULL;
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
            cr_remove_entity(app, b);
        }
    }

    // lanuch projectile
    if (cr_consume_input(app, CR_KEYCODE_X))
    {
        cr_entity *f = demo_create_fireball(
            app,
            target->x_pos - app->cam.x,
            target->y_pos - app->cam.y + 4);
        if (f != NULL)
        {
            if (cr_check_flag(target, ENTITY_FLAG_MIRROR))
            {
                f->x_acc = 1;
            }
            else
            {
                f->x_acc = -1;
                cr_set_flag(f, ENTITY_FLAG_MIRROR);
            }
        }
    }

    // TEMP: Reset the state of various things.
    if (cr_consume_input(app, CR_KEYCODE_R))
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
    if (cr_consume_input(app, CR_KEYCODE_F))
    {
        cr_play_sound(app, app->sounds[DEMO_SOUND_EFFECT_CONFIRMATION]);
    }
    if (cr_consume_input(app, CR_KEYCODE_G))
    {
        cr_play_sound(app, app->sounds[DEMO_SOUND_EFFECT_DROP]);
    }
    if (cr_consume_input(app, CR_KEYCODE_H))
    {
        cr_play_sound(app, app->sounds[DEMO_SOUND_EFFECT_TOGGLE]);
    }

    // The V and B keys will move the player left or right.
    if (cr_consume_input(app, CR_KEYCODE_V))
    {
        target->x_pos -= 1;
    }
    if (cr_consume_input(app, CR_KEYCODE_B))
    {
        target->x_pos += 1;
    }

    // The [ and ] keys will toggle the rendering of hitboxes and camera
    // boundaries respectively
    if (cr_consume_input(app, CR_KEYCODE_LEFTBRACKET))
    {
        app->debug.hitboxes = app->debug.hitboxes ? 0 : 1;
    }
    if (cr_consume_input(app, CR_KEYCODE_RIGHTBRACKET))
    {
        app->debug.camera = app->debug.camera ? 0 : 1;
    }

    // The ' key will toggle the camera configuration
    if (cr_consume_input(app, CR_KEYCODE_APOSTROPHE))
    {
        app->cam.config = app->cam.config == CR_CAMERA_ALL ? CR_CAMERA_NONE : CR_CAMERA_ALL;
    }

    // END player controls
    //-------------------------------------------------
}