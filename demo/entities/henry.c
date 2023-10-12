#include "demo/demo.h"
#include "demo/entities/entity_types.h"
#include "demo/entities/henry.h"
#include "demo/util/sprite.h"

#include "common/util.h"
#include "common/texture.h"
#include "common/dialog.h"


static int get_henry_x_vel(eg_entity *henry)
{
    // acceleration to velocity conversion table
    int a_to_v[24] = {
        1, 1, 1, 1, 1, 1,
        1, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3, 3,
        4, 4, 4, 4, 4, 4};

    int x_vel = 0;
    if (henry->x_acc > 0)
    {
        x_vel = a_to_v[henry->x_acc];
    }
    else if (henry->x_acc < 0)
    {
        x_vel = -(a_to_v[-(henry->x_acc)]);
    }

    if (henry->x_t)
    {
        x_vel += henry->x_t;
    }

    return x_vel;
}

static int get_henry_y_vel(eg_entity *henry)
{
    // acceleration to velocity conversion table
    int a_to_v[24] = {
        1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3, 3,
        4, 4, 4, 4, 4, 4};

    int y_vel = 0;
    if (henry->y_acc > 0)
    {
        y_vel = a_to_v[henry->y_acc];
    }
    else if (henry->y_acc < 0)
    {
        y_vel = -(a_to_v[-(henry->y_acc)]);
    }

    if (henry->y_t)
    {
        y_vel += henry->y_t;
    }

    return y_vel;
}

static void render_henry(eg_app *app, eg_entity *henry)
{
    int tile = 0;
    int grounded = eg_check_flag(henry, ENTITY_FLAG_GROUND);

    // Animation logic for walking to the right
    if ((henry->x_acc) && grounded)
    {
        if (henry->animation_ticks < 6)
        {
            tile = 0;
        }
        else if (henry->animation_ticks < 16)
        {
            tile = 1;
        }
    }

    if (!grounded)
    {
        tile = 1;
    }

    // Render the henry sprite.
    sprite_draw_henry(
        app,
        henry->x_pos + app->cam.x - 3,
        henry->y_pos + app->cam.y - 6,
        eg_check_flag(henry, ENTITY_FLAG_MIRROR),
        tile);

    // hit box
    if (app->debug.hitboxes)
    {
        eg_rect hit_box;
        hit_box.x = henry->x_pos + app->cam.x;
        hit_box.y = henry->y_pos + app->cam.y;
        hit_box.w = app->entity_types[henry->type].width;
        hit_box.h = app->entity_types[henry->type].height;

        // Render the henry hit box.
        eg_set_color(app, EG_COLOR_VINIK_ORANGE);
        eg_draw_rect(app, &hit_box, 0);
    }
}

static void update_henry(eg_app *app, eg_entity *henry)
{
    // Movement constraints for henry.
    int max_walk = 6;
    // int min_walk = 1;
    int walk_acc = 1;

    // Check for animation logic.
    int walking_left = henry->ticks < 60;
    int walking_right = henry->ticks >= 60 && henry->ticks < 120;

    // Get the width and height of the henry.
    // int w = app->entity_types[henry->type].width;
    int h = app->entity_types[henry->type].height;

    // Check the MOVE flag to see if the henry is being carried by a
    // a moving platform.
    int carried = eg_check_flag(henry, ENTITY_FLAG_MOVE);

    int avx = get_henry_x_vel(henry);
    int avy = get_henry_y_vel(henry);

    //--------------------------------------------------------------------
    // Horizontal Movement

    // horizontal velocity applied by a carrier
    if (carried && avx == 0)
    {
        avx += henry->x_vel;
    }

    // horizontal correction factor applied by collision with a solid object
    if (henry->x_t)
    {
        henry->x_acc = 0;
        henry->x_t = 0;
    }

    // Update horizontal position.
    henry->x_pos += avx;

    // Perform horizontal inertia.
    if (henry->x_acc > 0 && !walking_right)
    {
        henry->x_acc--;
    }

    if (henry->x_acc < 0 && !walking_left)
    {
        henry->x_acc++;
    }

    //--------------------------------------------------------------------
    // Vertical Movement

    // If the henry is standing on a moving platform,
    // adjust the y velocity to match the platform.
    if (carried && henry->carrier != NULL)
    {
        int cf = 0;

        // Determine the correction factor.
        // If the platform was already updated, recalculate the correction
        // factor that was done in the collision resolution.
        // Otherwise, we just add the platform's y velocity to the henry's.
        if (eg_check_flag(henry->carrier, ENTITY_FLAG_UPDATE))
        {
            cf = henry->carrier->y_pos - (henry->y_pos + h);
        }
        else
        {
            cf = henry->y_vel + henry->carrier->y_vel;
        }

        avy = cf;
    }

    // vertical correction factor applied by collision with a solid object
    if (henry->y_t && !carried)
    {
        henry->y_acc = 0;
        henry->y_t = 0;
    }

    // Set the link pointer to NULL.
    if (!carried && henry->carrier != NULL)
    {
        henry->carrier = NULL;
    }

    // Clear the carry flag.
    eg_clear_flag(henry, ENTITY_FLAG_MOVE);

    // Update vertical position.
    henry->y_pos += avy;

    // If the henry is on a sloped platform, set the y velocity to some
    // value that causes the henry to collide with the platform before
    // moving into open space.
    if (eg_check_flag(henry, ENTITY_FLAG_SLOPE))
    {
        eg_clear_flag(henry, ENTITY_FLAG_SLOPE);
        henry->y_acc = 23;
    }

    // Apply gravity.
    if (henry->y_acc < 23)
    {
        henry->y_acc++;
    }

    //--------------------------------------------------------------------
    // Behavior Logic
    //
    // Henry will continually walk left and right.

    henry->ticks++;

    // Walk to the left for 60 frames.
    if (henry->ticks < 61)
    {
        // Progress the walking animation.
        henry->animation_ticks++;
        if (henry->animation_ticks >= 20)
        {
            henry->animation_ticks = 0;
        }

        if (henry->x_acc > -max_walk)
        {
            henry->x_acc -= walk_acc;
        }

        if (henry->x_acc < -max_walk)
        {
            henry->x_acc = -max_walk;
        }
    }

    if (henry->ticks == 61)
    {
        eg_set_flag(henry, ENTITY_FLAG_MIRROR);
    }

    // Walk to the right for 60 frames.
    if (henry->ticks >= 61 && henry->ticks < 121)
    {
        // Progress the walking animation.
        henry->animation_ticks++;
        if (henry->animation_ticks >= 20)
        {
            henry->animation_ticks = 0;
        }

        if (henry->x_acc < max_walk)
        {
            henry->x_acc += walk_acc;
        }

        if (henry->x_acc > max_walk)
        {
            henry->x_acc = max_walk;
        }
    }

    // Reset Henry's walking.
    if (henry->ticks >= 121)
    {
        eg_clear_flag(henry, ENTITY_FLAG_MIRROR);
        henry->ticks = 0;
    }
}

static void collide_henry(
    eg_app *app,
    eg_entity *henry,
    eg_entity *other,
    eg_collision *t_res)
{
    if (other->type == ENTITY_TYPE_PLAYER)
    {
        if (eg_check_flag(other, ENTITY_FLAG_INVINCIBLE))
        {
            return;
        }

        eg_set_flag(other, ENTITY_FLAG_INVINCIBLE);

        int avx = app->entity_types[other->type].get_x_vel(other);
        if (avx >= 0)
        {
            other->x_acc = -4;
        }
        else
        {
            other->x_acc = 4;
        }

        other->y_acc = -8;
    }
}

void henry_demo_register(eg_entity_type *t)
{
    t->width = 18;
    t->height = 18;
    t->render = render_henry;
    t->update = update_henry;
    t->get_x_vel = get_henry_x_vel;
    t->get_y_vel = get_henry_y_vel;
    t->collide = collide_henry;
}

eg_entity *henry_demo_create(eg_app *app, int x, int y)
{
    eg_entity *henry = NULL;

    henry = eg_create_entity(app);
    if (henry == NULL)
    {
        return NULL;
    }

    henry->type = ENTITY_TYPE_HENRY;
    henry->x_pos = x;
    henry->y_pos = y;

    return henry;
}