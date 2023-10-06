#include "demo/entities/critter.h"
#include "demo/entities/entity_types.h"
#include "demo/texture/texture.h"
#include "demo/util/sprite.h"
#include "demo/dialog/dialog.h"
#include "demo/demo.h"

#include <stdio.h>
#include <stdlib.h>

static int get_critter_x_vel(eg_entity *critter)
{
    // acceleration to velocity conversion table
    int a_to_v[24] = {
        1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2,
        4, 4, 4, 4, 4, 4};

    int x_vel = 0;
    // int adjusted = 0;

    // Convert acceleration to velocity
    if (critter->x_acc > 0)
    {
        x_vel = a_to_v[critter->x_acc];
    }
    else if (critter->x_acc < 0)
    {
        x_vel = -(a_to_v[-(critter->x_acc)]);
    }

    if (critter->x_t)
    {
        x_vel += critter->x_t;
    }

    return x_vel; // adjusted;
}

static int get_critter_y_vel(eg_entity *critter)
{
    // acceleration to velocity conversion table
    int a_to_v[24] = {
        1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1};

    int y_vel = 0;
    if (critter->y_acc > 0)
    {
        y_vel = a_to_v[critter->y_acc];
    }
    else if (critter->y_acc < 0)
    {
        y_vel = -(a_to_v[-(critter->y_acc)]);
    }

    if (critter->y_t)
    {
        y_vel += critter->y_t;
    }

    if (critter->ticks < 120)
    {
        return 0;
    }

    return y_vel;
}

static void render_critter(eg_app *app, eg_entity *critter)
{
    // int tile = 0;

    // Animation logic for walking to the right
    // if (critter->animation_ticks < 5)
    // {
    //     tile = 9;
    // }
    // else if (critter->animation_ticks < 10)
    // {
    //     tile = 10;
    // }
    // else if (critter->animation_ticks < 15)
    // {
    //     tile = 11;
    // }

    // Render the critter sprite.
    // sprite_draw_critter(
    //     app,
    //     critter->x_pos + app->cam.x - 3,
    //     critter->y_pos + app->cam.y - 4,
    //     eg_check_flag(critter, ENTITY_FLAG_MIRROR),
    //     tile);
    eg_rect r = {
        .x = critter->x_pos,
        .y = critter->y_pos,
        .w = app->entity_types[critter->type].width,
        .h = app->entity_types[critter->type].height};

    if (critter->ticks < 120)
    {
        eg_set_color(app, EG_COLOR_MILITARY_GREEN);
    }
    else
    {
        eg_set_color(app, EG_COLOR_LIGHT_BLUE);
    }

    eg_draw_rect(app, &r, 1);

    // hit box
    if (app->debug.hitboxes)
    {
        eg_rect hit_box;
        hit_box.x = critter->x_pos + app->cam.x;
        hit_box.y = critter->y_pos + app->cam.y;
        hit_box.w = app->entity_types[critter->type].width;
        hit_box.h = app->entity_types[critter->type].height;

        // Render the critter hit box.
        eg_set_color(app, EG_COLOR_ORANGE);
        eg_draw_rect(app, &hit_box, 0);
    }
}

static void update_critter(eg_app *app, eg_entity *critter)
{
    int avx = get_critter_x_vel(critter);
    int avy = get_critter_y_vel(critter);
    // int mirror = eg_check_flag(critter, ENTITY_FLAG_MIRROR);

    //--------------------------------------------------------------------
    // Horizontal Movement

    // Update horizontal position.
    critter->x_pos += avx;

    //--------------------------------------------------------------------
    // Vertical Movement

    // Update vertical position.
    if (app->ticks % 2)
    {
        critter->y_pos += avy;
    }

    //--------------------------------------------------------------------
    // Behavior Logic

    critter->ticks++;

    // 120 frames of waiting in the canopy
    // fall
    // lie on the ground
    if (critter->ticks >= 480)
    {
        critter->present = 0;
        app->counters[DEMO_COUNTER_CRITTERS]--;
    }
}

static void collide_critter(
    eg_app *app,
    eg_entity *critter,
    eg_entity *other,
    eg_collision *t_res,
    int is_b)
{
    if (critter->ticks >= 120 && other->type == ENTITY_TYPE_NOTE)
    {
        if (other->present)
        {
            other->present = 0;
            app->counters[DEMO_COUNTER_NOTES]--;
        }

        critter->present = 0;
        app->counters[DEMO_COUNTER_CRITTERS]--;

        if (app->counters[DEMO_COUNTER_SCORE] < 100)
        {
            app->counters[DEMO_COUNTER_SCORE]++;
        }
        else
        {
            app->done = 1;
        }
    }
}

void critter_demo_register(eg_entity_type *t)
{
    t->width = 20;
    t->height = 12;
    t->render = render_critter;
    t->update = update_critter;
    t->get_x_vel = get_critter_x_vel;
    t->get_y_vel = get_critter_y_vel;
    t->collide = collide_critter;
    t->spur = 1;
}

eg_entity *critter_demo_create(eg_app *app, int x, int y)
{
    eg_entity *critter = NULL;

    critter = eg_create_entity(app);
    if (critter == NULL)
    {
        return NULL;
    }

    critter->type = ENTITY_TYPE_CRITTER;
    critter->x_pos = x;
    critter->y_pos = y;
    critter->y_acc = 1;

    return critter;
}
