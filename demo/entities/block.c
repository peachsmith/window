#include "demo/entities/block.h"
#include "demo/entities/entity_types.h"
#include "demo/collision/collision.h"

#include <stdio.h>

static void render_block(eg_app *app, eg_entity *block)
{
    eg_rect r;
    r.x = block->x_pos;
    r.y = block->y_pos;
    r.w = app->registry[block->id].width;
    r.h = app->registry[block->id].height;

    eg_set_color(app, 0x02, 0x75, 0xD8, 0xFF);
    eg_draw_rect(app, &r, 0);
}

static void collide_block(
    eg_app *app,
    eg_entity *block,
    eg_entity *other,
    eg_ray_res *t_res,
    int is_b)
{
    // The collision resolution correction factor formula is pulled from
    // the video at https://www.youtube.com/watch?v=8JJ-4JgR7Dg.

    // Correction factor.
    float t1 = 1 - t_res->t;

    if (t_res->cn.x)
    {
        int absx = other->x_vel > 0 ? other->x_vel : -(other->x_vel);
        float correction = t_res->cn.x * absx * t1;
        other->x_vel += (int)correction;
    }

    if (t_res->cn.y)
    {
        int absy = other->y_vel > 0 ? other->y_vel : -(other->y_vel);
        float correction = t_res->cn.y * absy * t1;
        other->y_vel += (int)correction;
    }
}

void block_demo_register(eg_entity_type *t)
{
    t->id = ENTITY_TYPE_BLOCK;
    t->width = 12;
    t->height = 12;
    t->render = render_block;
    t->collide = collide_block;
}

void block_demo_register_big(eg_entity_type *t)
{
    t->id = ENTITY_TYPE_BIG_BLOCK;
    t->width = 52;
    t->height = 52;
    t->render = render_block;
    t->collide = collide_block;
}

eg_entity *block_demo_create(int x, int y)
{
    eg_entity *block = NULL;

    block = eg_create_entity();
    if (block == NULL)
    {
        return NULL;
    }

    block->id = ENTITY_TYPE_BLOCK;
    block->x_pos = x;
    block->y_pos = y;

    return block;
}

eg_entity *block_demo_create_big(int x, int y)
{
    eg_entity *block = NULL;

    block = eg_create_entity();
    if (block == NULL)
    {
        return NULL;
    }

    block->id = ENTITY_TYPE_BIG_BLOCK;
    block->x_pos = x;
    block->y_pos = y;

    return block;
}
