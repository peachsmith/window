#include "demo/entities/block.h"
#include "demo/entities/entity_types.h"
#include "demo/collision/collision.h"

#include <stdio.h>

// Renders a standard block
static void render_block(eg_app *app, eg_entity *block)
{
    eg_rect r;
    r.x = block->x_pos + app->cam.x;
    r.y = block->y_pos + app->cam.y;
    r.w = app->registry[block->id].width;
    r.h = app->registry[block->id].height;

    eg_set_color(app, EG_COLOR_LIGHT_BLUE);
    eg_draw_rect(app, &r, 0);
}

// Renders a throughblock
static void render_throughblock(eg_app *app, eg_entity *block)
{
    eg_rect r;
    r.x = block->x_pos + app->cam.x;
    r.y = block->y_pos + app->cam.y;
    r.w = app->registry[block->id].width;
    r.h = app->registry[block->id].height;

    eg_set_color(app, EG_COLOR_MAUVE);
    eg_draw_rect(app, &r, 0);
}

// Renders a moving block
static void render_moving_block(eg_app *app, eg_entity *block)
{
    eg_rect r;
    r.x = block->x_pos + app->cam.x;
    r.y = block->y_pos + app->cam.y;
    r.w = app->registry[block->id].width;
    r.h = app->registry[block->id].height;

    eg_set_color(app, EG_COLOR_SEA_GREEN);
    eg_draw_rect(app, &r, 0);
}

static void update_moving_block(eg_app *app, eg_entity *block)
{
    // Update the position of the platform before updating the velocity.
    // This prevents a disconnect between the platform and any entities
    // being carried by it.
    block->y_pos += block->y_vel;

    // Switch direction to start moving down.
    if (block->y_pos <= 0)
    {
        block->y_vel = 1;
    }

    // If the platform is above position 80 and is moving down,
    // continue to move down.
    if (block->y_pos < 120 && block->y_vel >= 0)
    {
        block->y_vel = 1;
    }

    // Switch directions to start moving up.
    if (block->y_pos >= 120)
    {
        block->y_vel = -1;
    }

    // If the platform is below position 30 and is moving up,
    // continue to move up.
    if (block->y_pos > 0 && block->y_vel < 0)
    {
        block->y_vel = -1;
    }
}

static void collide_block(
    eg_app *app,
    eg_entity *block,
    eg_entity *other,
    eg_collision *t_res,
    int is_b)
{
    // The collision resolution correction factor formula is pulled from
    // the video at https://www.youtube.com/watch?v=8JJ-4JgR7Dg.

    // Clear the jump flag and set the ground flag.
    // Only do this if the entity collided with the block from above.
    if (t_res->cn.y < 0 && !t_res->cn.x)
    {
        eg_clear_flag(other, ENTITY_FLAG_JUMP);
        eg_set_flag(other, ENTITY_FLAG_GROUND);
    }

    // If the block is a throughblock, check if we should resolve the collision.
    if (block->id == ENTITY_TYPE_THROUGHBLOCK_LONG)
    {
        // If the down key is pressed and the y component of the contact
        // is negative, don't resolve the collision.
        if (eg_check_flag(other, ENTITY_FLAG_DOWN))
        {
            return;
        }

        // If the y component of the contact normal is greater than zero,
        // then the source entity is approaching from below, and we don't
        // resolve the collision.
        if (t_res->cn.y > 0)
        {
            return;
        }
    }

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

    // If the block is a moving platform, update the source entity's position
    // according to the correction factor, then set the source entity's
    // velocity to match the platform.
    if (block->id == ENTITY_TYPE_BLOCK_MOVING)
    {
        if (t_res->cn.y < 0)
        {
            int ah = app->registry[other->id].height;
            eg_set_flag(other, ENTITY_FLAG_MOVE);
            other->link = block;
            other->y_vel = block->y_pos + app->cam.y - (other->y_pos + ah);
            // if (block->y_vel)
            // {
            //     other->y_vel += block->y_vel;
            // }
        }
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

void block_demo_register_big(eg_entity_type *t)
{
    t->id = ENTITY_TYPE_BLOCK_BIG;
    t->width = 52;
    t->height = 52;
    t->render = render_block;
    t->collide = collide_block;
}

eg_entity *block_demo_create_big(int x, int y)
{
    eg_entity *block = NULL;

    block = eg_create_entity();
    if (block == NULL)
    {
        return NULL;
    }

    block->id = ENTITY_TYPE_BLOCK_BIG;
    block->x_pos = x;
    block->y_pos = y;

    return block;
}

void block_demo_register_long(eg_entity_type *t)
{
    t->id = ENTITY_TYPE_BLOCK_LONG;
    t->width = 100;
    t->height = 16;
    t->render = render_block;
    t->collide = collide_block;
}

eg_entity *block_demo_create_long(int x, int y)
{
    eg_entity *block = NULL;

    block = eg_create_entity();
    if (block == NULL)
    {
        return NULL;
    }

    block->id = ENTITY_TYPE_BLOCK_LONG;
    block->x_pos = x;
    block->y_pos = y;

    return block;
}

void throughblock_demo_register_long(eg_entity_type *t)
{
    t->id = ENTITY_TYPE_THROUGHBLOCK_LONG;
    t->width = 100;
    t->height = 16;
    t->render = render_throughblock;
    t->collide = collide_block;
}

eg_entity *throughblock_demo_create_long(int x, int y)
{
    eg_entity *block = NULL;

    block = eg_create_entity();
    if (block == NULL)
    {
        return NULL;
    }

    block->id = ENTITY_TYPE_THROUGHBLOCK_LONG;
    block->x_pos = x;
    block->y_pos = y;

    return block;
}

void block_demo_register_moving(eg_entity_type *t)
{
    t->id = ENTITY_TYPE_BLOCK_MOVING;
    t->width = 60;
    t->height = 16;
    t->render = render_moving_block;
    t->update = update_moving_block;
    t->collide = collide_block;
}

eg_entity *block_demo_create_moving(int x, int y)
{
    eg_entity *block = NULL;

    block = eg_create_entity();
    if (block == NULL)
    {
        return NULL;
    }

    block->id = ENTITY_TYPE_BLOCK_MOVING;
    block->x_pos = x;
    block->y_pos = y;

    return block;
}