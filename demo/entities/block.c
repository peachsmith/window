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
    eg_t_res *t_res,
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
    if (is_b)
    {
        printf("block is b\n");
        dx0 = -dx0;
        dx1 = -dx1;
        dy0 = -dy0;
        dy1 = -dy1;
    }
    else
    {
        printf("block is a\n");
        cnx = -cnx;
        cny = -cny;
    }

    // Determine which direction to resolve the collision.
    if (cnx == -1)
    {
        // left
        other->x_vel = 0;
        // other->x_pos -= dx1;
        other->x_pos += dx0;
    }

    if (cnx == 1)
    {
        // right
        other->x_vel = 0;
        // other->x_pos += dx0;
        other->x_pos -= dx1;
    }

    if (cny == -1)
    {
        // up
        other->y_vel = 0;
        // other->y_pos -= dy1;
        other->y_pos += dy0;
    }

    if (cny == 1)
    {
        // down
        other->y_vel = 0;
        // other->y_pos += dy0;
        other->y_pos -= dy1;
    }

    // Set the x velocity to 0.
    // Relocated within contact normal logic.
    // other->x_vel = 0;

    // Determine which way to adjust this entity's position.
    // if (res->dx0 <= res->dx1)
    // {
    //     other->x_pos += dx0;
    // }
    // else
    // {
    //     other->x_pos -= dx1;
    // }

    // Set the x velocity to 0.
    // Relocated within contact normal logic.
    // other->y_vel = 0;

    // Determine which was to adjust this entity's position.
    // if (res->dy0 <= res->dy1)
    // {
    //     other->y_pos += dy0;
    // }
    // else
    // {
    //     other->y_pos -= dy1;
    // }
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
