#include "demo/entities/fireball.h"
#include "demo/entities/entity_types.h"
#include "demo/texture/texture.h"
#include "demo/util/sprite.h"
#include "demo/dialog/dialog.h"

#include <stdio.h>
#include <stdlib.h>

static int get_fireball_x_vel(eg_entity *fireball)
{
    // acceleration to velocity conversion table
    int a_to_v[24] = {
        1, 4, 1, 1, 1, 1,
        1, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3, 3,
        4, 4, 4, 4, 4, 4};

    int x_vel = 0;
    if (fireball->x_acc > 0)
    {
        x_vel = a_to_v[fireball->x_acc];
    }
    else if (fireball->x_acc < 0)
    {
        x_vel = -(a_to_v[-(fireball->x_acc)]);
    }

    if (fireball->x_t)
    {
        x_vel += fireball->x_t;
    }

    return x_vel;
}

static int get_fireball_y_vel(eg_entity *fireball)
{
    // acceleration to velocity conversion table
    int a_to_v[24] = {
        1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3, 3,
        4, 4, 4, 4, 4, 4};

    int y_vel = 0;
    if (fireball->y_acc > 0)
    {
        y_vel = a_to_v[fireball->y_acc];
    }
    else if (fireball->y_acc < 0)
    {
        y_vel = -(a_to_v[-(fireball->y_acc)]);
    }

    if (fireball->y_t)
    {
        y_vel += fireball->y_t;
    }

    return y_vel;
}

static void render_fireball(eg_app *app, eg_entity *fireball)
{
    int tile = 0;

    // Animation logic for walking to the right
    if (fireball->animation_ticks < 5)
    {
        tile = 9;
    }
    else if (fireball->animation_ticks < 10)
    {
        tile = 10;
    }
    else if (fireball->animation_ticks < 15)
    {
        tile = 11;
    }

    // Render the fireball sprite.
    sprite_draw_fireball(
        app,
        fireball->x_pos + app->cam.x - 3,
        fireball->y_pos + app->cam.y - 4,
        eg_check_flag(fireball, ENTITY_FLAG_MIRROR),
        tile);

    // hit box
    if (app->debug.hitboxes)
    {
        eg_rect hit_box;
        hit_box.x = fireball->x_pos + app->cam.x;
        hit_box.y = fireball->y_pos + app->cam.y;
        hit_box.w = app->entity_types[fireball->type].width;
        hit_box.h = app->entity_types[fireball->type].height;

        // Render the fireball hit box.
        eg_set_color(app, EG_COLOR_ORANGE);
        eg_draw_rect(app, &hit_box, 0);
    }
}

static void update_fireball(eg_app *app, eg_entity *fireball)
{
    int avx = get_fireball_x_vel(fireball);
    int avy = get_fireball_y_vel(fireball);

    //--------------------------------------------------------------------
    // Horizontal Movement

    // Update horizontal position.
    fireball->x_pos += avx;

    //--------------------------------------------------------------------
    // Vertical Movement

    // Update vertical position.
    fireball->y_pos += avy;

    //--------------------------------------------------------------------
    // Behavior Logic

    fireball->animation_ticks++;
    if (fireball->animation_ticks >= 15)
    {
        fireball->animation_ticks = 0;
    }

    fireball->ticks++;

    if (fireball->ticks >= 60)
    {
        fireball->present = 0;
    }
}

static void collide_fireball(
    eg_app *app,
    eg_entity *fireball,
    eg_entity *other,
    eg_collision *t_res,
    int is_b)
{
    if (other->type == ENTITY_TYPE_BLOCK ||
        other->type == ENTITY_TYPE_HENRY ||
        other->type == ENTITY_TYPE_BLOCK_SLOPE ||
        other->type == ENTITY_TYPE_BLOCK_MOVING)
    {
        fireball->present = 0;

        if (other->type == ENTITY_TYPE_HENRY)
        {
            other->present = 0;
        }
    }
}

void fireball_demo_register(eg_entity_type *t)
{
    t->width = 11;
    t->height = 11;
    t->render = render_fireball;
    t->update = update_fireball;
    t->get_x_vel = get_fireball_x_vel;
    t->get_y_vel = get_fireball_y_vel;
    t->collide = collide_fireball;
}

eg_entity *fireball_demo_create(eg_app *app, int x, int y)
{
    eg_entity *fireball = NULL;

    fireball = eg_create_entity(app);
    if (fireball == NULL)
    {
        return NULL;
    }

    fireball->type = ENTITY_TYPE_FIREBALL;
    fireball->x_pos = x;
    fireball->y_pos = y;

    return fireball;
}