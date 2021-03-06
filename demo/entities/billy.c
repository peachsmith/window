#include "demo/entities/billy.h"
#include "demo/entities/entity_types.h"
#include "demo/texture/texture.h"
#include "demo/util/sprite.h"
#include "demo/dialog/dialog.h"

#include <stdio.h>
#include <stdlib.h>

// This is a tick counter used for moving things around.
// Much like the tmp_counter from player.c, this is temporary
// and will most likely be moved to a better location at some point.
static int action_counter = 0;

static int animation_counter = 0;

static int get_billy_x_vel(eg_entity *billy)
{
    // acceleration to velocity conversion table
    int a_to_v[24] = {
        1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3, 3,
        4, 4, 4, 4, 4, 4};

    int x_vel = 0;
    if (billy->x_acc > 0)
    {
        x_vel = a_to_v[billy->x_acc];
    }
    else if (billy->x_acc < 0)
    {
        x_vel = -(a_to_v[-(billy->x_acc)]);
    }

    if (billy->x_t)
    {
        x_vel += billy->x_t;
    }

    return x_vel;
}

static int get_billy_y_vel(eg_entity *billy)
{
    // acceleration to velocity conversion table
    int a_to_v[24] = {
        1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3, 3,
        4, 4, 4, 4, 4, 4};

    int y_vel = 0;
    if (billy->y_acc > 0)
    {
        y_vel = a_to_v[billy->y_acc];
    }
    else if (billy->y_acc < 0)
    {
        y_vel = -(a_to_v[-(billy->y_acc)]);
    }

    if (billy->y_t)
    {
        y_vel += billy->y_t;
    }

    return y_vel;
}

static void render_billy(eg_app *app, eg_entity *billy)
{
    int tile = 0;
    int grounded = eg_check_flag(billy, ENTITY_FLAG_GROUND);

    // Animation logic for walking to the right
    if ((billy->x_acc) && grounded)
    {
        if (animation_counter < 6)
        {
            tile = 0;
        }
        else if (animation_counter < 16)
        {
            tile = 1;
        }
    }

    if (!grounded)
    {
        tile = 1;
    }

    // Render the billy sprite.
    sprite_draw_billy(
        app,
        billy->x_pos + app->cam.x,
        billy->y_pos + app->cam.y,
        eg_check_flag(billy, ENTITY_FLAG_MIRROR),
        tile);

    // hit box
    if (app->debug.hitboxes)
    {
        eg_rect hit_box;
        hit_box.x = billy->x_pos + app->cam.x;
        hit_box.y = billy->y_pos + app->cam.y;
        hit_box.w = app->registry[billy->type].width;
        hit_box.h = app->registry[billy->type].height;

        // Render the billy hit box.
        eg_set_color(app, EG_COLOR_ORANGE);
        eg_draw_rect(app, &hit_box, 0);
    }
}

static void update_billy(eg_app *app, eg_entity *billy)
{
    // Movement constraints for billy.
    int max_walk = 6;
    // int min_walk = 1;
    int walk_acc = 1;

    // Check for animation logic.
    int walking_left = action_counter < 60;
    int walking_right = action_counter >= 300 && action_counter < 450;

    // Get the width and height of the billy.
    // int w = app->registry[billy->type].width;
    int h = app->registry[billy->type].height;

    // Check the MOVE flag to see if the billy is being carried by a
    // a moving platform.
    int carried = eg_check_flag(billy, ENTITY_FLAG_MOVE);

    int avx = get_billy_x_vel(billy);
    int avy = get_billy_y_vel(billy);

    //--------------------------------------------------------------------
    // Horizontal Movement

    // horizontal velocity applied by a carrier
    if (carried && avx == 0)
    {
        avx += billy->x_vel;
    }

    // horizontal correction factor applied by collision with a solid object
    if (billy->x_t)
    {
        billy->x_acc = 0;
        billy->x_t = 0;
    }

    // Update horizontal position.
    billy->x_pos += avx;

    // Perform horizontal inertia.
    if (billy->x_acc > 0 && !walking_right)
    {
        billy->x_acc--;
    }

    if (billy->x_acc < 0 && !walking_left)
    {
        billy->x_acc++;
    }

    //--------------------------------------------------------------------
    // Vertical Movement

    // If the billy is standing on a moving platform,
    // adjust the y velocity to match the platform.
    if (carried && billy->carrier != NULL)
    {
        int cf = 0;

        // Determine the correction factor.
        // If the platform was already updated, recalculate the correction
        // factor that was done in the collision resolution.
        // Otherwise, we just add the platform's y velocity to the billy's.
        if (eg_check_flag(billy->carrier, ENTITY_FLAG_UPDATE))
        {
            cf = billy->carrier->y_pos - (billy->y_pos + h);
        }
        else
        {
            cf = billy->y_vel + billy->carrier->y_vel;
        }

        avy = cf;
    }

    // vertical correction factor applied by collision with a solid object
    if (billy->y_t && !carried)
    {
        billy->y_acc = 0;
        billy->y_t = 0;
    }

    // Set the link pointer to NULL.
    if (!carried && billy->carrier != NULL)
    {
        billy->carrier = NULL;
    }

    // Clear the carry flag.
    eg_clear_flag(billy, ENTITY_FLAG_MOVE);

    // Update vertical position.
    billy->y_pos += avy;

    // If the billy is on a sloped platform, set the y velocity to some
    // value that causes the billy to collide with the platform before
    // moving into open space.
    if (eg_check_flag(billy, ENTITY_FLAG_SLOPE))
    {
        eg_clear_flag(billy, ENTITY_FLAG_SLOPE);
        billy->y_acc = 23;
    }

    // Apply gravity.
    if (billy->y_acc < 23)
    {
        billy->y_acc++;
    }

    //--------------------------------------------------------------------
    // Animation Logic

    if (action_counter < 450)
    {
        action_counter++;
    }
    else
    {
        // Stop the walking animation.
        animation_counter = 0;
    }

    // Walk to the left for 120 frames.
    if (action_counter < 60)
    {
        // Progress the walking animation.
        animation_counter++;
        if (animation_counter >= 20)
        {
            animation_counter = 0;
        }

        if (billy->x_acc > -max_walk)
        {
            billy->x_acc -= walk_acc;
        }

        if (billy->x_acc < -max_walk)
        {
            billy->x_acc = -max_walk;
        }
    }

    // Stand in once place for 180 frames.
    // Turn to face left and right a few times.
    if (action_counter >= 60 && action_counter < 300)
    {
        // Stop the animation.
        animation_counter = 0;

        // Turn to face right.
        if (action_counter > 180 && action_counter < 210)
        {
            eg_set_flag(billy, ENTITY_FLAG_MIRROR);
        }

        // Turn to face left.
        if (action_counter >= 210 && action_counter < 250)
        {
            eg_clear_flag(billy, ENTITY_FLAG_MIRROR);
        }

        // Turn to face right.
        if (action_counter >= 250)
        {
            eg_set_flag(billy, ENTITY_FLAG_MIRROR);
        }
    }

    // Walk to the right for 120 frames.
    if (action_counter >= 300 && action_counter < 450)
    {
        // Progress the walking animation.
        animation_counter++;
        if (animation_counter >= 20)
        {
            animation_counter = 0;
        }

        if (billy->x_acc < max_walk)
        {
            billy->x_acc += walk_acc;
        }

        if (billy->x_acc > max_walk)
        {
            billy->x_acc = max_walk;
        }
    }
}

void billy_demo_register(eg_entity_type *t)
{
    t->width = 24;
    t->height = 24;
    t->render = render_billy;
    t->update = update_billy;
    t->get_x_vel = get_billy_x_vel;
    t->get_y_vel = get_billy_y_vel;
}

eg_entity *billy_demo_create(int x, int y)
{
    eg_entity *billy = NULL;

    billy = eg_create_entity();
    if (billy == NULL)
    {
        return NULL;
    }

    billy->type = ENTITY_TYPE_BILLY;
    billy->x_pos = x;
    billy->y_pos = y;

    return billy;
}