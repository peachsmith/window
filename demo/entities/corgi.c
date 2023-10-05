#include "demo/entities/corgi.h"
#include "demo/entities/entity_types.h"
#include "demo/texture/texture.h"
#include "demo/util/sprite.h"
#include "demo/util/util.h"

#include <stdlib.h>

#define CORGI_X_ACC_LIMIT 24
#define CORGI_Y_ACC_LIMIT 24
#define CORGI_IFRAME_LIMIT 20

static int get_corgi_x_vel(eg_entity *corgi)
{
    // acceleration to velocity conversion table
    int a_to_v[CORGI_X_ACC_LIMIT] = {
        1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3, 3,
        4, 4, 4, 4, 4, 4};

    // Make sure the acceleration is within the limit.
    // Any acceleration outside of the limits will result in
    // maximum velocity.
    int acc = corgi->x_acc;
    if (acc >= CORGI_X_ACC_LIMIT)
    {
        acc = CORGI_X_ACC_LIMIT - 1;
    }
    if (acc <= -CORGI_X_ACC_LIMIT)
    {
        acc = -CORGI_X_ACC_LIMIT + 1;
    }

    int x_vel = 0;
    if (acc > 0)
    {
        x_vel = a_to_v[acc];
    }
    else if (acc < 0)
    {
        x_vel = -(a_to_v[-acc]);
    }

    if (corgi->x_t)
    {
        x_vel += corgi->x_t;
    }

    // effectively halve the velocity
    if (corgi->ticks % 2)
    {
        x_vel = 0;
    }

    return x_vel;
}

static int get_corgi_y_vel(eg_entity *corgi)
{
    // acceleration to velocity conversion table
    int a_to_v[CORGI_Y_ACC_LIMIT] = {
        1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3, 3,
        4, 4, 4, 4, 4, 4};

    // Make sure the acceleration is within the limit.
    // Any acceleration outside of the limits will result in
    // maximum velocity.
    int acc = corgi->y_acc;
    if (acc >= CORGI_Y_ACC_LIMIT)
    {
        acc = CORGI_Y_ACC_LIMIT - 1;
    }
    if (acc <= -CORGI_Y_ACC_LIMIT)
    {
        acc = -CORGI_Y_ACC_LIMIT + 1;
    }

    int y_vel = 0;
    if (acc > 0)
    {
        y_vel = a_to_v[acc];
    }
    else if (acc < 0)
    {
        y_vel = -(a_to_v[-acc]);
    }

    if (corgi->y_t)
    {
        y_vel += corgi->y_t;
    }

    // effectively halve the velocity
    if (corgi->ticks % 2)
    {
        y_vel = 0;
    }

    return y_vel;
}

static void render_corgi(eg_app *app, eg_entity *corgi)
{
    // int left_pressed = eg_peek_input(app, EG_KEYCODE_LEFT);
    // int right_pressed = eg_peek_input(app, EG_KEYCODE_RIGHT);
    // int grounded = eg_check_flag(corgi, ENTITY_FLAG_GROUND);

    // Animation logic for walking to the right
    // if ((left_pressed || right_pressed) && grounded)
    // {
    //     if (corgi->animation_ticks < 6)
    //     {
    //         tile = 0;
    //     }
    //     else if (corgi->animation_ticks < 16)
    //     {
    //         tile = 1;
    //     }
    // }

    // if (!grounded)
    // {
    //     tile = 1;
    // }

    // Render the corgi sprite.
    // sprite_draw_character(
    //     app,
    //     corgi->x_pos,
    //     corgi->y_pos,
    //     eg_check_flag(corgi, ENTITY_FLAG_MIRROR), tile);
    eg_rect r = {
        .x = corgi->x_pos,
        .y = corgi->y_pos,
        .w = app->entity_types[corgi->type].width,
        .h = app->entity_types[corgi->type].height};

    eg_set_color(app, EG_COLOR_ORANGE);
    eg_draw_rect(app, &r, 1);

    // hit box
    if (app->debug.hitboxes)
    {
        eg_rect hit_box;
        hit_box.x = corgi->x_pos;
        hit_box.y = corgi->y_pos;
        hit_box.w = app->entity_types[corgi->type].width;
        hit_box.h = app->entity_types[corgi->type].height;

        // Render the corgi hit box.
        eg_set_color(app, EG_COLOR_PURPLE);
        eg_draw_rect(app, &hit_box, 0);
    }
}

static void update_corgi(eg_app *app, eg_entity *corgi)
{
    // Get the width and height of the corgi.
    int w = app->entity_types[corgi->type].width;
    int h = app->entity_types[corgi->type].height;

    // Check the MOVE flag to see if the corgi is being carried by a
    // a moving platform.
    int carried = eg_check_flag(corgi, ENTITY_FLAG_MOVE);
    int grounded = eg_check_flag(corgi, ENTITY_FLAG_GROUND);
    int left_pressed = eg_peek_input(app, EG_KEYCODE_LEFT);
    int right_pressed = eg_peek_input(app, EG_KEYCODE_RIGHT);

    int avx = get_corgi_x_vel(corgi);
    int avy = get_corgi_y_vel(corgi);

    //--------------------------------------------------------------------
    // Horizontal Movement

    // horizontal velocity applied by a carrier
    if (carried && avx == 0)
    {
        avx += corgi->x_vel;
    }

    // horizontal correction factor applied by collision with a solid object
    if (corgi->x_t)
    {
        corgi->x_acc = 0;
        corgi->x_t = 0;
    }

    // Update horizontal position.
    if (app->cam.config == EG_CAMERA_ALL && corgi->x_pos + w >= app->cam.cr && avx > 0)
    {
        int dcam = (corgi->x_pos + w) - app->cam.cr;
        corgi->x_pos = app->cam.cr - w;
        app->cam.x -= avx;
        app->cam.x -= dcam;
    }
    else if (app->cam.config == EG_CAMERA_ALL && corgi->x_pos <= app->cam.cl + 1 && avx < 0)
    {
        int dcam = corgi->x_pos - (app->cam.cl + 1);
        corgi->x_pos = app->cam.cl + 1;
        app->cam.x -= avx;
        app->cam.x -= dcam;
    }
    else
    {
        corgi->x_pos += avx;
    }

    // Perform horizontal inertia.
    if (corgi->x_acc > 0 && !right_pressed)
    {
        corgi->x_acc--;
    }

    if (corgi->x_acc < 0 && !left_pressed)
    {
        corgi->x_acc++;
    }

    //--------------------------------------------------------------------
    // Vertical Movement

    // If the corgi is standing on a moving platform,
    // adjust the y velocity to match the platform.
    if (carried && corgi->carrier != NULL)
    {
        int cf = 0;

        // Determine the correction factor.
        // If the platform was already updated, recalculate the correction
        // factor that was done in the collision resolution.
        // Otherwise, we just add the platform's y velocity to the corgi's.
        if (eg_check_flag(corgi->carrier, ENTITY_FLAG_UPDATE))
        {
            cf = corgi->carrier->y_pos + app->cam.y - (corgi->y_pos + h);
        }
        else
        {
            cf = corgi->y_vel + corgi->carrier->y_vel;
        }

        avy = cf;
    }

    // vertical correction factor applied by collision with a solid object
    if (corgi->y_t && !carried)
    {
        corgi->y_acc = 0;
        corgi->y_t = 0;
    }

    // Set the link pointer to NULL.
    if (!carried && corgi->carrier != NULL)
    {
        corgi->carrier = NULL;
    }

    // Clear the carry flag.
    eg_clear_flag(corgi, ENTITY_FLAG_MOVE);

    // Update vertical position.
    if (app->cam.config == EG_CAMERA_ALL && corgi->y_pos + h >= app->cam.cb && avy > 0)
    {
        int dcam = (corgi->y_pos + h) - app->cam.cb;
        corgi->y_pos = app->cam.cb - h;
        app->cam.y -= avy;
        app->cam.y -= dcam;
    }
    else if (app->cam.config == EG_CAMERA_ALL && corgi->y_pos <= app->cam.ct + 1 && avy < 0)
    {
        int dcam = corgi->y_pos - (app->cam.ct + 1);
        corgi->y_pos = app->cam.ct + 1;
        app->cam.y -= avy;
        app->cam.y -= dcam;
    }
    else
    {
        corgi->y_pos += avy;
    }

    // If the corgi is on a sloped platform, set the y velocity to some
    // value that causes the corgi to collide with the platform before
    // moving into open space.
    if (eg_check_flag(corgi, ENTITY_FLAG_SLOPE))
    {
        eg_clear_flag(corgi, ENTITY_FLAG_SLOPE);
        corgi->y_acc = CORGI_Y_ACC_LIMIT - 1;
    }

    // Apply gravity.
    if (corgi->y_acc < CORGI_Y_ACC_LIMIT - 1)
    {
        // jump height control
        int act = app->actuation_counters[EG_KEYCODE_SPACE];
        int delay_acceleration = 0;

        if (act)
        {
            if (act < 5)
            {
                delay_acceleration = 1;
            }
            else if (act < 12)
            {
                delay_acceleration = 1;
            }
        }

        if (!delay_acceleration)
        {
            corgi->y_acc++;
        }
    }

    //--------------------------------------------------------------------
    // Behavior Logic

    corgi->ticks++;

    // Advance the counter for walking to the right
    if ((eg_peek_input(app, EG_KEYCODE_LEFT) || eg_peek_input(app, EG_KEYCODE_RIGHT)) && grounded)
    {
        corgi->animation_ticks++;
        if (corgi->animation_ticks >= 20)
        {
            corgi->animation_ticks = 0;
        }
    }
    else
    {
        corgi->animation_ticks = 0;
    }
}

void corgi_demo_register(eg_entity_type *t)
{
    t->width = 24;
    t->height = 24;
    t->render = render_corgi;
    t->update = update_corgi;
    t->get_x_vel = get_corgi_x_vel;
    t->get_y_vel = get_corgi_y_vel;
}

eg_entity *corgi_demo_create(eg_app *app, int x, int y)
{
    eg_entity *corgi = NULL;

    corgi = eg_create_entity(app);
    if (corgi == NULL)
    {
        return NULL;
    }

    corgi->type = ENTITY_TYPE_CORGI;
    corgi->x_pos = x;
    corgi->y_pos = y;

    return corgi;
}