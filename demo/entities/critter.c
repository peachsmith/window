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

    // The critter has no reason to collide with a solid block from
    // the side.
    // if (critter->x_t)
    // {
    //     x_vel += critter->x_t;
    // }

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
    int tile = 0;
    int grounded = eg_check_flag(critter, ENTITY_FLAG_GROUND);

    // If the critter has fallen from the tree, is still in the air, and has
    // not yet been healed with the power of music, render it upside down.
    if (!grounded && !critter->result)
    {
        tile = 2;
    }
    else if (grounded && critter->result)
    {
        if (critter->animation_ticks < 5)
        {
            tile = 1;
        }
    }
    else if (!critter->result)
    {
        tile = 2;
    }

    // Render the critter sprite.
    if (critter->ticks >= 120)
    {
        sprite_draw_critter(
            app,
            critter->x_pos,
            critter->y_pos,
            eg_check_flag(critter, ENTITY_FLAG_MIRROR),
            tile);
    }
    else
    {
        // TODO: render shaking leaves to indicate that a critter is about
        // to fall from the trees.
        eg_rect r = {
            .x = critter->x_pos,
            .y = critter->y_pos,
            .w = app->entity_types[critter->type].width,
            .h = app->entity_types[critter->type].height};

        eg_set_color(app, EG_COLOR_MILITARY_GREEN);
        eg_draw_rect(app, &r, 1);
    }

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
    int grounded = eg_check_flag(critter, ENTITY_FLAG_GROUND);
    int mirror = eg_check_flag(critter, ENTITY_FLAG_MIRROR);

    //--------------------------------------------------------------------
    // Horizontal Movement

    if (critter->result && grounded)
    {
        critter->x_acc = mirror ? 1 : -1;
    }

    // Update horizontal position.
    critter->x_pos += avx;

    //--------------------------------------------------------------------
    // Vertical Movement

    // vertical correction factor applied by collision with a solid object
    if (critter->y_t)
    {
        critter->y_t = 0;
    }

    // Update vertical position.
    if (app->ticks % 2)
    {
        critter->y_pos += avy;
    }

    //--------------------------------------------------------------------
    // Behavior Logic

    critter->ticks++;

    // running animation
    if (critter->result && grounded)
    {
        critter->animation_ticks++;
        if (critter->animation_ticks >= 10)
        {
            critter->animation_ticks = 0;
        }
    }

    // TODO: pre fall animation
    // TODO: expiration animation
    // TODO: healing animation (may require the use of an additional counter)

    // If 400 ticks have passed and the critter has not yet been healed by
    // the power of music, then send it to live on a farm.
    if (critter->ticks >= 400 && !critter->result && critter->present)
    {
        critter->present = 0;
        app->counters[DEMO_COUNTER_CRITTERS]--;
        app->counters[critter->data]--;
    }

    // If the critter runs off the screen, remove it from the scene.
    if ((critter->x_pos < -20 || critter->x_pos > 240) && critter->present)
    {
        critter->present = 0;
        app->counters[DEMO_COUNTER_CRITTERS]--;
        app->counters[critter->data]--;
    }
}

static void collide_critter(
    eg_app *app,
    eg_entity *critter,
    eg_entity *other,
    eg_collision *t_res,
    int is_b)
{
    if (critter->ticks < 120 || critter->result || other->type != ENTITY_TYPE_NOTE)
    {
        return;
    }

    if (other->present)
    {
        other->present = 0;
        app->counters[DEMO_COUNTER_NOTES]--;
    }

    if (!critter->result)
    {
        critter->result = 1;
    }

    // If a certain score is reached, the game ends.
    if (app->counters[DEMO_COUNTER_SCORE] < 100)
    {
        app->counters[DEMO_COUNTER_SCORE]++;
    }
    else
    {
        app->done = 1;
    }
}

void tns_register_critter(eg_entity_type *t)
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

eg_entity *tns_create_critter(eg_app *app, int x, int y)
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
