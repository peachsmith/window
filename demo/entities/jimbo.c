#include "demo/entities/jimbo.h"
#include "demo/entities/entity_types.h"
#include "demo/texture/texture.h"
#include "demo/util/sprite.h"
#include "demo/dialog/dialog.h"
#include "demo/input/input.h"
#include "demo/entities/jimbo_dialog.h"

#include <stdio.h>
#include <stdlib.h>

static int get_jimbo_x_vel(eg_entity *jimbo)
{
    // acceleration to velocity conversion table
    int a_to_v[24] = {
        1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3, 3,
        4, 4, 4, 4, 4, 4};

    int x_vel = 0;
    if (jimbo->x_acc > 0)
    {
        x_vel = a_to_v[jimbo->x_acc];
    }
    else if (jimbo->x_acc < 0)
    {
        x_vel = -(a_to_v[-(jimbo->x_acc)]);
    }

    if (jimbo->x_t)
    {
        x_vel += jimbo->x_t;
    }

    return x_vel;
}

static int get_jimbo_y_vel(eg_entity *jimbo)
{
    // acceleration to velocity conversion table
    int a_to_v[24] = {
        1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3, 3,
        4, 4, 4, 4, 4, 4};

    int y_vel = 0;
    if (jimbo->y_acc > 0)
    {
        y_vel = a_to_v[jimbo->y_acc];
    }
    else if (jimbo->y_acc < 0)
    {
        y_vel = -(a_to_v[-(jimbo->y_acc)]);
    }

    if (jimbo->y_t)
    {
        y_vel += jimbo->y_t;
    }

    return y_vel;
}

static void render_jimbo(eg_app *app, eg_entity *jimbo)
{
    int tile = 0;
    int grounded = eg_check_flag(jimbo, ENTITY_FLAG_GROUND);

    if (!grounded)
    {
        tile = 1;
    }

    // Render the jimbo sprite.
    sprite_draw_jimbo(
        app,
        jimbo->x_pos + app->cam.x,
        jimbo->y_pos + app->cam.y,
        eg_check_flag(jimbo, ENTITY_FLAG_MIRROR),
        tile);

    // hit box
    if (app->debug.hitboxes)
    {
        eg_rect hit_box;
        hit_box.x = jimbo->x_pos + app->cam.x;
        hit_box.y = jimbo->y_pos + app->cam.y;
        hit_box.w = app->entity_types[jimbo->type].width;
        hit_box.h = app->entity_types[jimbo->type].height;

        // Render the jimbo hit box.
        eg_set_color(app, EG_COLOR_ORANGE);
        eg_draw_rect(app, &hit_box, 0);
    }
}

static void update_jimbo(eg_app *app, eg_entity *jimbo)
{
    // Get the width and height of the jimbo.
    // int w = app->entity_types[jimbo->type].width;
    int h = app->entity_types[jimbo->type].height;

    // Check the MOVE flag to see if the jimbo is being carried by a
    // a moving platform.
    int carried = eg_check_flag(jimbo, ENTITY_FLAG_MOVE);

    int avx = get_jimbo_x_vel(jimbo);
    int avy = get_jimbo_y_vel(jimbo);

    //--------------------------------------------------------------------
    // Horizontal Movement

    // horizontal velocity applied by a carrier
    if (carried && avx == 0)
    {
        avx += jimbo->x_vel;
    }

    // horizontal correction factor applied by collision with a solid object
    if (jimbo->x_t)
    {
        jimbo->x_acc = 0;
        jimbo->x_t = 0;
    }

    // Update horizontal position.
    jimbo->x_pos += avx;

    // Perform horizontal inertia.
    if (jimbo->x_acc > 0)
    {
        jimbo->x_acc--;
    }

    if (jimbo->x_acc < 0)
    {
        jimbo->x_acc++;
    }

    //--------------------------------------------------------------------
    // Vertical Movement

    // If the jimbo is standing on a moving platform,
    // adjust the y velocity to match the platform.
    if (carried && jimbo->carrier != NULL)
    {
        int cf = 0;

        // Determine the correction factor.
        // If the platform was already updated, recalculate the correction
        // factor that was done in the collision resolution.
        // Otherwise, we just add the platform's y velocity to the jimbo's.
        if (eg_check_flag(jimbo->carrier, ENTITY_FLAG_UPDATE))
        {
            cf = jimbo->carrier->y_pos - (jimbo->y_pos + h);
        }
        else
        {
            cf = jimbo->y_vel + jimbo->carrier->y_vel;
        }

        avy = cf;
    }

    // vertical correction factor applied by collision with a solid object
    if (jimbo->y_t && !carried)
    {
        jimbo->y_acc = 0;
        jimbo->y_t = 0;
    }

    // Set the link pointer to NULL.
    if (!carried && jimbo->carrier != NULL)
    {
        jimbo->carrier = NULL;
    }

    // Clear the carry flag.
    eg_clear_flag(jimbo, ENTITY_FLAG_MOVE);

    // Update vertical position.
    jimbo->y_pos += avy;

    // If the jimbo is on a sloped platform, set the y velocity to some
    // value that causes the jimbo to collide with the platform before
    // moving into open space.
    if (eg_check_flag(jimbo, ENTITY_FLAG_SLOPE))
    {
        eg_clear_flag(jimbo, ENTITY_FLAG_SLOPE);
        jimbo->y_acc = 23;
    }

    // Apply gravity.
    if (jimbo->y_acc < 23)
    {
        jimbo->y_acc++;
    }
}

static int interact_with_jimbo(eg_app *app, eg_entity *jimbo, eg_entity *actor)
{
    app->pause = 1;

    // Locate jimbo's dialog.
    eg_entity *jimbo_dialog = NULL;
    for (int i = 0; i < app->entity_cap; i++)
    {
        if (app->entities[i].type == ENTITY_TYPE_JIMBO_DIALOG && app->entities[i].present)
        {
            jimbo_dialog = &(app->entities[i]);
        }
    }

    jimbo_dialog_demo_open(app, jimbo_dialog);

    eg_push_input_handler(app, common_dialog_input_handler);

    return 0;
}

void jimbo_demo_register(eg_entity_type *t)
{
    t->width = 24;
    t->height = 24;
    t->render = render_jimbo;
    t->update = update_jimbo;
    t->get_x_vel = get_jimbo_x_vel;
    t->get_y_vel = get_jimbo_y_vel;
    t->interactable = 1;
    t->interact = interact_with_jimbo;
}

eg_entity *jimbo_demo_create(int x, int y)
{
    eg_entity *jimbo = NULL;

    jimbo = eg_create_entity();
    if (jimbo == NULL)
    {
        return NULL;
    }

    jimbo->type = ENTITY_TYPE_JIMBO;
    jimbo->x_pos = x;
    jimbo->y_pos = y;

    return jimbo;
}