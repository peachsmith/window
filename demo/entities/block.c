#include "demo/demo.h"
#include "demo/entities/entity_types.h"
#include "demo/entities/block.h"
#include "demo/util/sprite.h"

#include "common/util.h"
#include "common/collision.h"

// Renders a standard block
static void render_block(cr_app *app, cr_entity *block)
{
    if (!util_is_on_screen(app, block))
    {
        return;
    }
    
    sprite_draw_brick(
        app,
        block->x_pos + app->cam.x,
        block->y_pos + app->cam.y);

    // hit box
    if (app->debug.hitboxes)
    {
        cr_rect hit_box;
        hit_box.x = block->x_pos + app->cam.x;
        hit_box.y = block->y_pos + app->cam.y;
        hit_box.w = app->entity_types[block->type].width;
        hit_box.h = app->entity_types[block->type].height;

        cr_set_color(app, CR_COLOR_VINIK_LIGHT_BLUE);
        cr_draw_rect(app, &hit_box, 0);
    }
}

// Renders a throughblock
static void render_throughblock(cr_app *app, cr_entity *block)
{
    if (!util_is_on_screen(app, block))
    {
        return;
    }

    cr_rect r;
    r.x = block->x_pos + app->cam.x;
    r.y = block->y_pos + app->cam.y;
    r.w = app->entity_types[block->type].width;
    r.h = app->entity_types[block->type].height;

    cr_set_color(app, CR_COLOR_VINIK_FOREST_GREEN);
    cr_draw_rect(app, &r, 1);

    // hit box
    if (app->debug.hitboxes)
    {
        cr_rect hit_box;
        hit_box.x = block->x_pos + app->cam.x;
        hit_box.y = block->y_pos + app->cam.y;
        hit_box.w = app->entity_types[block->type].width;
        hit_box.h = app->entity_types[block->type].height;

        cr_set_color(app, CR_COLOR_VINIK_LIGHT_BLUE);
        cr_draw_rect(app, &hit_box, 0);
    }
}

// Renders a moving block
static void render_moving_block(cr_app *app, cr_entity *block)
{
    if (!util_is_on_screen(app, block))
    {
        return;
    }

    sprite_draw_grass_block(
        app,
        block->x_pos + app->cam.x,
        block->y_pos + app->cam.y,
        app->entity_types[block->type].width,
        app->entity_types[block->type].height);

    // hit box
    if (app->debug.hitboxes)
    {
        cr_rect hit_box;
        hit_box.x = block->x_pos + app->cam.x;
        hit_box.y = block->y_pos + app->cam.y;
        hit_box.w = app->entity_types[block->type].width;
        hit_box.h = app->entity_types[block->type].height;

        cr_set_color(app, CR_COLOR_VINIK_SEA_GREEN);
        cr_draw_rect(app, &hit_box, 0);
    }
}

// Renders a sloped block
static void render_sloped_block(cr_app *app, cr_entity *block)
{
    if (!util_is_on_screen(app, block))
    {
        return;
    }

    int dir = block->data & 3;

    if (dir < 2)
    {
        sprite_draw_slope(
            app,
            block->x_pos + app->cam.x,
            block->y_pos + app->cam.y,
            4,
            !dir ? 1 : 0);
    }
    else
    {
        sprite_draw_flat_slope(
            app,
            block->x_pos + app->cam.x,
            block->y_pos + app->cam.y,
            4);
    }

    if (app->debug.hitboxes)
    {
        cr_rect r;
        r.x = block->x_pos + app->cam.x;
        r.y = block->y_pos + app->cam.y;
        r.w = app->entity_types[block->type].width;
        r.h = app->entity_types[block->type].height;

        int dir = block->data & 3;

        if (dir < 2)
        {
            // Draw the boundaries of the width and height.
            cr_set_color(app, CR_COLOR_VINIK_RED);
            cr_draw_rect(app, &r, 0);
        }

        // Draw the line.
        // Default is from bottom left to top right.
        cr_set_color(app, CR_COLOR_VINIK_FOREST_GREEN);
        cr_point a = {.x = r.x, .y = r.y + r.h};
        cr_point b = {.x = r.x + r.w, .y = r.y};

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
        cr_draw_line(app, &a, &b);
    }
}

static void move_vertical(cr_app *app, cr_entity *block)
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

static void move_horizontal(cr_app *app, cr_entity *block)
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

static void move_down_right(cr_app *app, cr_entity *block)
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

static void update_moving_block(cr_app *app, cr_entity *block)
{
    // Update the position of the platform before updating the velocity.
    // This prevents a disconnect between the platform and any entities
    // being carried by it.
    block->y_pos += block->y_vel;
    block->x_pos += block->x_vel;

    // The first two bit flags are used to determine which update function
    // is called.
    switch (block->data & 3)
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
    cr_set_flag(block, ENTITY_FLAG_UPDATE);
}

static void collide_block(
    cr_app *app,
    cr_entity *block,
    cr_entity *other,
    cr_collision *t_res)
{
    // The collision resolution correction factor formula is pulled from
    // the video at https://www.youtube.com/watch?v=8JJ-4JgR7Dg.

    if (block->type == ENTITY_TYPE_BLOCK_SLOPE)
    {
        int avy = app->entity_types[other->type].get_y_vel(other);

        // Correct the correction factor.
        if ((block->data & 3) == 2)
        {
            int ah = app->entity_types[other->type].height;
            int by = block->y_pos + app->cam.y;
            int check = other->y_pos + ah;
            if (!app->entity_types[other->type].control)
            {
                check += app->cam.y;
            }
            int ty = (int)(t_res->ty);
            int pre = check + (avy - ty);

            if (pre == by)
            {
                if (ty > avy)
                {
                    ty = avy;
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
        cr_clear_flag(other, ENTITY_FLAG_JUMP);
        cr_set_flag(other, ENTITY_FLAG_GROUND);

        // The source entity has collided with a diagonal line.
        // We now must determine which direction to resolve the collision
        // based on the source entity's velocity.
        other->y_t += -((int)t_res->ty + 1);
        cr_set_flag(other, ENTITY_FLAG_SLOPE);

        return;
    }

    // Clear the jump flag and set the ground flag.
    // Only do this if the entity collided with the block from above.
    if (t_res->cn.y < 0 && !t_res->cn.x)
    {
        cr_clear_flag(other, ENTITY_FLAG_JUMP);
        cr_set_flag(other, ENTITY_FLAG_GROUND);
    }

    // If the block is a throughblock, check if we should resolve the collision.
    if (block->type == ENTITY_TYPE_THROUGHBLOCK_LONG)
    {
        // If the down key is pressed and the y component of the contact
        // is negative, don't resolve the collision.
        if (cr_check_flag(other, ENTITY_FLAG_DOWN))
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
        int avx = app->entity_types[other->type].get_x_vel(other);
        int absx = avx > 0 ? avx : -(avx);
        float correction = t_res->cn.x * absx * t1;

        // Account for rounding.
        if (correction < 0)
        {
            correction -= 0.5f;
        }
        else if (correction > 0)
        {
            correction += 0.5f;
        }

        other->x_t += (int)correction;
    }

    if (t_res->cn.y)
    {
        // Correction factor for landing on top of the block.
        int avy = app->entity_types[other->type].get_y_vel(other);
        int absy = avy > 0 ? avy : -(avy);
        float correction = t_res->cn.y * absy * t1;

        // Account for rounding.
        if (correction < 0)
        {
            correction -= 0.5f;
        }
        else if (correction > 0)
        {
            correction += 0.5f;
        }

        other->y_t += (int)correction;
    }

    // If the block is a moving platform, update the source entity's position
    // according to the correction factor, then set the source entity's
    // velocity to match the platform.
    if (block->type == ENTITY_TYPE_BLOCK_MOVING)
    {
        if (t_res->cn.y < 0)
        {
            int ah = app->entity_types[other->type].height;
            cr_set_flag(other, ENTITY_FLAG_MOVE);
            other->carrier = block;

            // Adjust the x velocity if the source entity is not
            // currently providing its own velocity.
            if (other->x_acc == 0)
            {
                other->x_vel = block->x_vel;
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
            int camy = 0;
            if (!app->entity_types[other->type].control)
            {
                camy = app->cam.y;
            }
            other->y_vel = block->y_pos + app->cam.y - (other->y_pos + camy + ah);
            // other->y_vel = block->y_pos + app->cam.y - (other->y_pos + ah);
        }
    }
}

void demo_register_block(cr_entity_type *t)
{
    t->id = ENTITY_TYPE_BLOCK;
    t->width = 18;
    t->height = 18;
    t->render = render_block;
    t->collide = collide_block;
}

cr_entity *demo_create_block(cr_app *app, int x, int y)
{
    cr_entity *block = NULL;

    block = cr_create_entity(app);
    if (block == NULL)
    {
        return NULL;
    }

    block->type = ENTITY_TYPE_BLOCK;
    block->x_pos = x;
    block->y_pos = y;

    return block;
}

void demo_register_big_block(cr_entity_type *t)
{
    t->id = ENTITY_TYPE_BLOCK_BIG;
    t->width = 52;
    t->height = 52;
    t->render = render_block;
    t->collide = collide_block;
}

cr_entity *demo_create_big_block(cr_app *app, int x, int y)
{
    cr_entity *block = NULL;

    block = cr_create_entity(app);
    if (block == NULL)
    {
        return NULL;
    }

    block->type = ENTITY_TYPE_BLOCK_BIG;
    block->x_pos = x;
    block->y_pos = y;

    return block;
}

void demo_register_long_block(cr_entity_type *t)
{
    t->id = ENTITY_TYPE_BLOCK_LONG;
    t->width = 100;
    t->height = 16;
    t->render = render_block;
    t->collide = collide_block;
}

cr_entity *demo_create_long_block(cr_app *app, int x, int y)
{
    cr_entity *block = NULL;

    block = cr_create_entity(app);
    if (block == NULL)
    {
        return NULL;
    }

    block->type = ENTITY_TYPE_BLOCK_LONG;
    block->x_pos = x;
    block->y_pos = y;

    return block;
}

void demo_register_long_throughblock(cr_entity_type *t)
{
    t->id = ENTITY_TYPE_THROUGHBLOCK_LONG;
    t->width = 100;
    t->height = 16;
    t->render = render_throughblock;
    t->collide = collide_block;
}

cr_entity *demo_create_long_throughblock(cr_app *app, int x, int y)
{
    cr_entity *block = NULL;

    block = cr_create_entity(app);
    if (block == NULL)
    {
        return NULL;
    }

    block->type = ENTITY_TYPE_THROUGHBLOCK_LONG;
    block->x_pos = x;
    block->y_pos = y;

    return block;
}

void demo_register_moving_block(cr_entity_type *t)
{
    t->id = ENTITY_TYPE_BLOCK_MOVING;
    t->width = 54;
    t->height = 18;
    t->render = render_moving_block;
    t->update = update_moving_block;
    t->collide = collide_block;
    t->move = 1;
}

cr_entity *demo_create_moving_block(cr_app *app, int x, int y, int type)
{
    cr_entity *block = NULL;

    block = cr_create_entity(app);
    if (block == NULL)
    {
        return NULL;
    }

    block->type = ENTITY_TYPE_BLOCK_MOVING;
    block->x_pos = x;
    block->y_pos = y;
    block->data = (uint8_t)(type & 3);

    return block;
}

void demo_register_sloped_block(cr_entity_type *t)
{
    t->id = ENTITY_TYPE_BLOCK_SLOPE;
    t->width = 72;
    t->height = 18;
    t->render = render_sloped_block;
    t->collide = collide_block;
    t->slope = 1;
}

cr_entity *demo_create_sloped_block(cr_app *app, int x, int y, int dir)
{
    cr_entity *block = NULL;

    block = cr_create_entity(app);
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
    block->data = dir;

    return block;
}

void block_demo_row(cr_app *app, int x, int y, int length)
{
    int block_w = 18;

    for (int i = 0; i < length; i++)
    {
        demo_create_block(app, x + i * block_w, y);
    }
}

void block_demo_col(cr_app *app, int x, int y, int length)
{
    int block_h = 18;

    for (int i = 0; i < length; i++)
    {
        demo_create_block(app, x, y + i * block_h);
    }
}
