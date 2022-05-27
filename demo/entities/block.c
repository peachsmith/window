#include "demo/entities/block.h"
#include "demo/entities/entity_types.h"
#include "demo/collision/collision.h"
#include "demo/util/sprite.h"

#include <stdio.h>

// Renders a standard block
static void render_block(eg_app *app, eg_entity *block)
{
    sprite_draw_brick(
        app,
        block->x_pos + app->cam.x,
        block->y_pos + app->cam.y);

    // hit box
    // eg_rect r;
    // r.x = block->x_pos + app->cam.x;
    // r.y = block->y_pos + app->cam.y;
    // r.w = app->registry[block->type].width;
    // r.h = app->registry[block->type].height;

    // eg_set_color(app, EG_COLOR_LIGHT_BLUE);
    // eg_draw_rect(app, &r, 0);
}

// Renders a throughblock
static void render_throughblock(eg_app *app, eg_entity *block)
{
    eg_rect r;
    r.x = block->x_pos + app->cam.x;
    r.y = block->y_pos + app->cam.y;
    r.w = app->registry[block->type].width;
    r.h = app->registry[block->type].height;

    eg_set_color(app, EG_COLOR_MAUVE);
    eg_draw_rect(app, &r, 0);
}

// Renders a moving block
static void render_moving_block(eg_app *app, eg_entity *block)
{
    sprite_draw_grass_block(
        app,
        block->x_pos + app->cam.x,
        block->y_pos + app->cam.y,
        app->registry[block->type].width,
        app->registry[block->type].height);

    // hit box
    // eg_rect r;
    // r.x = block->x_pos + app->cam.x;
    // r.y = block->y_pos + app->cam.y;
    // r.w = app->registry[block->type].width;
    // r.h = app->registry[block->type].height;

    // eg_set_color(app, EG_COLOR_SEA_GREEN);
    // eg_draw_rect(app, &r, 0);
}

// Renders a sloped block
static void render_sloped_block(eg_app *app, eg_entity *block)
{
    eg_rect r;
    r.x = block->x_pos + app->cam.x;
    r.y = block->y_pos + app->cam.y;
    r.w = app->registry[block->type].width;
    r.h = app->registry[block->type].height;

    int dir = block->flags & 3;

    if (dir < 2)
    {
        // Draw the boundaries of the width and height.
        eg_set_color(app, EG_COLOR_RED);
        eg_draw_rect(app, &r, 0);
    }

    // Draw the line.
    // Default is from bottom left to top right.
    eg_set_color(app, EG_COLOR_FOREST_GREEN);
    eg_point a = {.x = r.x, .y = r.y + r.h};
    eg_point b = {.x = r.x + r.w, .y = r.y};

    if (dir == 0)
    {
        // from top left to bottom right.
        a.y -= r.h;
        b.y += r.h;
    }
    else if (dir == 2)
    {
        // horizontal line from top left to top right.
        a.y -= r.h;
    }
    eg_draw_line(app, &a, &b);
}

static void move_vertical(eg_app *app, eg_entity *block)
{
    // Move down.
    if (block->ticks < 100)
    {
        block->y_vel = 1;
    }

    // Move up.
    if (block->ticks >= 100 && block->ticks < 200)
    {
        block->y_vel = -1;
    }

    // Advance the tick count.
    block->ticks++;

    // Reset tick count.
    if (block->ticks >= 200)
    {
        block->ticks = 0;
    }
}

static void move_horizontal(eg_app *app, eg_entity *block)
{
    // Move left.
    if (block->ticks < 100)
    {
        block->x_vel = -1;
    }

    // Move right.
    if (block->ticks >= 100 && block->ticks < 200)
    {
        block->x_vel = 1;
    }

    // Advance the tick count.
    block->ticks++;

    // Reset tick count.
    if (block->ticks >= 200)
    {
        block->ticks = 0;
    }
}

static void move_down_right(eg_app *app, eg_entity *block)
{
    // Move down and to the left.
    if (block->ticks < 100)
    {
        block->x_vel = 1;
        block->y_vel = 1;
    }

    // Move up and to the right.
    if (block->ticks >= 100 && block->ticks < 200)
    {
        block->x_vel = -1;
        block->y_vel = -1;
    }

    // Advance the tick count.
    block->ticks++;

    // Reset tick count.
    if (block->ticks >= 200)
    {
        block->ticks = 0;
    }
}

static void update_moving_block(eg_app *app, eg_entity *block)
{
    // Update the position of the platform before updating the velocity.
    // This prevents a disconnect between the platform and any entities
    // being carried by it.
    block->y_pos += block->y_vel;
    block->x_pos += block->x_vel;

    // The first two bit flags are used to determine which update function
    // is called.
    switch (block->flags & 3)
    {
    case 0:
        move_vertical(app, block);
        break;

    case 1:
        move_horizontal(app, block);
        break;

    case 2:
        move_down_right(app, block);
        break;

    default:
        return;
    }

    // Set the update flag so that entities being carried by this platform
    // can update their state accordingly.
    eg_set_flag(block, ENTITY_FLAG_UPDATE);
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

    if (block->type == ENTITY_TYPE_BLOCK_SLOPE)
    {
        // Correct the correction factor.
        // TODO: move this into the line.c file.
        if ((block->flags & 3) == 2)
        {
            int ah = app->registry[other->type].height;
            int by = block->y_pos + app->cam.y;
            int check = other->y_pos + ah;
            int ty = (int)(t_res->ty);
            int pre = check + (other->y_vel - ty);

            if (pre == by)
            {
                if (ty > other->y_vel)
                {
                    ty = other->y_vel;
                    t_res->ty = (float)ty;
                }
                else
                {
                    t_res->ty--;
                }
            }
        }

        // For now, we are assuming that the source entity collided with
        // the diagonal line from above.
        eg_clear_flag(other, ENTITY_FLAG_JUMP);
        eg_set_flag(other, ENTITY_FLAG_GROUND);

        // The source entity has collided with a diagonal line.
        // We now must determine which direction to resolve the collision
        // based on the source entity's velocity.

        other->y_vel -= ((int)t_res->ty + 1);
        eg_set_flag(other, ENTITY_FLAG_SLOPE);

        return;
    }

    // Clear the jump flag and set the ground flag.
    // Only do this if the entity collided with the block from above.
    if (t_res->cn.y < 0 && !t_res->cn.x)
    {
        eg_clear_flag(other, ENTITY_FLAG_JUMP);
        eg_set_flag(other, ENTITY_FLAG_GROUND);
    }

    // If the block is a throughblock, check if we should resolve the collision.
    if (block->type == ENTITY_TYPE_THROUGHBLOCK_LONG)
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
        // Correction factor for landing on top of the block.
        int absy = other->y_vel > 0 ? other->y_vel : -(other->y_vel);
        float correction = t_res->cn.y * absy * t1;
        other->y_vel += (int)correction;
    }

    // If the block is a moving platform, update the source entity's position
    // according to the correction factor, then set the source entity's
    // velocity to match the platform.
    if (block->type == ENTITY_TYPE_BLOCK_MOVING)
    {
        if (t_res->cn.y < 0)
        {
            int ah = app->registry[other->type].height;
            eg_set_flag(other, ENTITY_FLAG_MOVE);
            other->carrier = block;

            // Adjust the x velocity if the source entity is not
            // currently providing its own velocity.
            if (other->x_vel == 0)
            {
                other->x_vel += block->x_vel;
            }

            // Adjust the source entity's y velocity.
            // The moving platform may be updated before or after the entity
            // standing on it.
            // The platform's position will be updated, then its velocity
            // will be updated.
            // We want the correction factor to predic and account for this
            // change.
            // The entity being carried by the platform should move with the
            // platform regardless of when they were added to the application.
            other->y_vel = block->y_pos + app->cam.y - (other->y_pos + ah);
        }
    }
}

void block_demo_register(eg_entity_type *t)
{
    t->id = ENTITY_TYPE_BLOCK;
    t->width = 18;
    t->height = 18;
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

    block->type = ENTITY_TYPE_BLOCK;
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

    block->type = ENTITY_TYPE_BLOCK_BIG;
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

    block->type = ENTITY_TYPE_BLOCK_LONG;
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

    block->type = ENTITY_TYPE_THROUGHBLOCK_LONG;
    block->x_pos = x;
    block->y_pos = y;

    return block;
}

void block_demo_register_moving(eg_entity_type *t)
{
    t->id = ENTITY_TYPE_BLOCK_MOVING;
    t->width = 54;
    t->height = 18;
    t->render = render_moving_block;
    t->update = update_moving_block;
    t->collide = collide_block;
}

eg_entity *block_demo_create_moving(int x, int y, int type)
{
    eg_entity *block = NULL;

    block = eg_create_entity();
    if (block == NULL)
    {
        return NULL;
    }

    block->type = ENTITY_TYPE_BLOCK_MOVING;
    block->x_pos = x;
    block->y_pos = y;
    block->flags = (uint8_t)(type & 3);

    return block;
}

void block_demo_register_sloped(eg_entity_type *t)
{
    t->id = ENTITY_TYPE_BLOCK_SLOPE;
    t->width = 120;
    t->height = 32;
    t->render = render_sloped_block;
    t->collide = collide_block;
}

eg_entity *block_demo_create_sloped(int x, int y, int dir)
{
    eg_entity *block = NULL;

    block = eg_create_entity();
    if (block == NULL)
    {
        return NULL;
    }

    block->type = ENTITY_TYPE_BLOCK_SLOPE;
    block->x_pos = x;
    block->y_pos = y;

    // We use the first flag to indicate the direction of the slope.
    // 1 for incline from left to right, or 0 for incline from right
    // to left. 2 for horizontal (used for dismounting slopes).
    block->flags = dir;

    return block;
}
