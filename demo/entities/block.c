#include "demo/entities/block.h"
#include "demo/entities/entity_types.h"

#include <stdio.h>

static void render_block(eg_app *app, eg_entity *block)
{
    SDL_Rect r;
    r.x = block->x_pos;
    r.y = block->y_pos;
    r.w = app->registry[block->id].width;
    r.h = app->registry[block->id].height;

    SDL_SetRenderDrawColor(app->renderer, 0x02, 0x75, 0xD8, 0xFF);
    // SDL_RenderFillRect(app->renderer, &r);
    SDL_RenderDrawRect(app->renderer, &r);
}

static void collide_block(
    eg_app *app,
    eg_entity *block,
    eg_entity *other,
    eg_overlap *res,
    eg_ray_res *t_res,
    int is_b)
{
    // Get the distance between the edges of this entity and the other entity.
    int dx0 = res->dx0;
    int dx1 = res->dx1;
    int dy0 = res->dy0;
    int dy1 = res->dy1;

    int cnx = t_res->cn.x;
    int cny = t_res->cn.y;

    // In a collision between entities A and B, if this entity is entity B,
    // then we negate its deltas, so that the position of the other entity
    // will be adjusted correctly.
    // If this is entity A, we negate the contact normal.
    if (is_b)
    {
        dx0 = -dx0;
        dx1 = -dx1;
        dy0 = -dy0;
        dy1 = -dy1;
    }
    else
    {
        cnx = -cnx;
        cny = -cny;
    }

    // The collision resolution correction factor formula is pulled from
    // the video at https://www.youtube.com/watch?v=8JJ-4JgR7Dg.

    // Correction factor.
    float t1 = 1 - t_res->t;

    if (cnx)
    {
        int absx = other->x_vel > 0 ? other->x_vel : -(other->x_vel);
        float correction = cnx * absx * t1;
        other->x_vel += (int)correction;
        printf(" (t: %.4f, 1-t: %.4f, correction: %.4f) \n", t_res->t, 1 - t_res->t, correction);
    }

    if (cny)
    {
        int absy = other->y_vel > 0 ? other->y_vel : -(other->y_vel);
        float correction = cny * absy * t1;
        other->y_vel += (int)correction;
        printf(" (t: %.4f, 1-t: %.4f, correction: %.4f) \n", t_res->t, 1 - t_res->t, correction);
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
