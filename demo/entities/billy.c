#include "demo/demo.h"
#include "demo/entities/entity_types.h"
#include "demo/entities/billy.h"
#include "demo/util/sprite.h"

#include "common/util.h"
#include "common/dialog.h"

static int get_billy_x_vel(cr_entity *billy)
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

static int get_billy_y_vel(cr_entity *billy)
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

static void render_billy(cr_app *app, cr_entity *billy)
{
    if (!util_is_on_screen(app, billy))
    {
        return;
    }

    int tile = 0;
    int grounded = cr_check_flag(billy, ENTITY_FLAG_GROUND);

    // Animation logic for walking to the right
    if ((billy->x_acc) && grounded)
    {
        if (billy->animation_ticks < 6)
        {
            tile = 0;
        }
        else if (billy->animation_ticks < 16)
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
        cr_check_flag(billy, ENTITY_FLAG_MIRROR),
        tile);

    // hit box
    if (app->debug.hitboxes)
    {
        cr_rect hit_box;
        hit_box.x = billy->x_pos + app->cam.x;
        hit_box.y = billy->y_pos + app->cam.y;
        hit_box.w = app->entity_types[billy->type].width;
        hit_box.h = app->entity_types[billy->type].height;

        // Render the billy hit box.
        cr_set_color(app, CR_COLOR_VINIK_ORANGE);
        cr_draw_rect(app, &hit_box, 0);
    }
}

static void update_billy(cr_app *app, cr_entity *billy)
{
    // Movement constraints for billy.
    int max_walk = 6;
    // int min_walk = 1;
    int walk_acc = 1;

    // Check for animation logic.
    int walking_left = billy->ticks < 60;
    int walking_right = billy->ticks >= 300 && billy->ticks < 450;

    // Get the width and height of the billy.
    // int w = app->entity_types[billy->type].width;
    int h = app->entity_types[billy->type].height;

    // Check the MOVE flag to see if the billy is being carried by a
    // a moving platform.
    int carried = cr_check_flag(billy, ENTITY_FLAG_MOVE);

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
        if (cr_check_flag(billy->carrier, ENTITY_FLAG_UPDATE))
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
    cr_clear_flag(billy, ENTITY_FLAG_MOVE);

    // Update vertical position.
    billy->y_pos += avy;

    // If the billy is on a sloped platform, set the y velocity to some
    // value that causes the billy to collide with the platform before
    // moving into open space.
    if (cr_check_flag(billy, ENTITY_FLAG_SLOPE))
    {
        cr_clear_flag(billy, ENTITY_FLAG_SLOPE);
        billy->y_acc = 23;
    }

    // Apply gravity.
    if (billy->y_acc < 23)
    {
        billy->y_acc++;
    }

    //--------------------------------------------------------------------
    // Animation Logic

    // if (billy->ticks < 450)
    if (billy->ticks < 450)
    {
        billy->ticks++;
    }
    else
    {
        // Stop the walking animation.
        billy->animation_ticks = 0;
    }

    // Walk to the left for 60 frames.
    if (billy->ticks < 60)
    {
        // Progress the walking animation.
        billy->animation_ticks++;
        if (billy->animation_ticks >= 20)
        {
            billy->animation_ticks = 0;
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

    // Stand in once place for 240 frames.
    // Turn to face left and right a few times.
    if (billy->ticks >= 60 && billy->ticks < 300)
    {
        // Stop the animation.
        billy->animation_ticks = 0;

        // Turn to face right.
        if (billy->ticks > 180 && billy->ticks < 210)
        {
            cr_set_flag(billy, ENTITY_FLAG_MIRROR);
        }

        // Turn to face left.
        if (billy->ticks >= 210 && billy->ticks < 250)
        {
            cr_clear_flag(billy, ENTITY_FLAG_MIRROR);
        }

        // Turn to face right.
        if (billy->ticks >= 250)
        {
            cr_set_flag(billy, ENTITY_FLAG_MIRROR);
        }
    }

    // Walk to the right for 150 frames.
    if (billy->ticks >= 300 && billy->ticks < 450)
    {
        // Progress the walking animation.
        billy->animation_ticks++;
        if (billy->animation_ticks >= 20)
        {
            billy->animation_ticks = 0;
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

void demo_register_billy(cr_entity_type *t)
{
    t->width = 24;
    t->height = 24;
    t->render = render_billy;
    t->update = update_billy;
    t->get_x_vel = get_billy_x_vel;
    t->get_y_vel = get_billy_y_vel;
}

cr_entity *demo_create_billy(cr_app *app, int x, int y)
{
    cr_entity *billy = NULL;

    billy = cr_create_entity(app);
    if (billy == NULL)
    {
        return NULL;
    }

    billy->type = ENTITY_TYPE_BILLY;
    billy->x_pos = x;
    billy->y_pos = y;

    return billy;
}