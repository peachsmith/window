#include "demo/player.h"
#include "demo/entity_types.h"

#include <stdio.h>
#include <stdlib.h>

static void render_player(eg_app *app, eg_entity *player)
{
    SDL_Rect r;
    r.x = player->x_pos;
    r.y = player->y_pos;
    r.w = app->registry[player->id].width;
    r.h = app->registry[player->id].height;

    SDL_SetRenderDrawColor(app->renderer, 0xF0, 0xAD, 0x4E, 0xFF);
    SDL_RenderFillRect(app->renderer, &r);
}

static void update_player(eg_app *app, eg_entity *player, int axis)
{
    if (axis == EG_AXIS_X)
    {
        // Update horizontal position.
        player->x_pos += player->x_vel;

        // Perform horizontal inertia.
        if (player->x_vel > 0)
        {
            player->x_vel--;
        }

        if (player->x_vel < 0)
        {
            player->x_vel++;
        }
    }
    else if (axis == EG_AXIS_Y)
    {
        // Update vertical position.
        player->y_pos += player->y_vel;

        // Perform vertical inertia.
        if (player->y_vel > 0)
        {
            player->y_vel--;
        }

        if (player->y_vel < 0)
        {
            player->y_vel++;
        }
    }
}

static void collide_player(
    eg_app *app,
    eg_entity *player,
    eg_entity *other,
    eg_collision_result *res,
    int is_b)
{
    int dx0 = res->dx0;
    int dx1 = res->dx1;
    int dy0 = res->dy0;
    int dy1 = res->dy1;

    // In a collision between entities A and B, if this entity is entity B,
    // then we negate its deltas, so that its position will be adjusted
    // correctly.
    if (is_b)
    {
        dx0 = -dx0;
        dx1 = -dx1;
        dy0 = -dy0;
        dy1 = -dy1;
    }

    if (res->direction == EG_AXIS_X)
    {
        // Set the x velocity to 0.
        player->x_vel = 0;

        // Determine which way to adjust this entity's position.
        if (res->dx0 <= res->dx1)
        {
            player->x_pos -= dx0;
        }
        else
        {
            player->x_pos += dx1;
        }
    }
    else if (res->direction == EG_AXIS_Y)
    {
        // Set the x velocity to 0.
        player->y_vel = 0;

        // Determine which was to adjust this entity's position.
        if (res->dy0 <= res->dy1)
        {
            player->y_pos -= dy0;
        }
        else
        {
            player->y_pos += dy1;
        }
    }
}

void player_demo_register(eg_entity_type *t)
{
    t->width = 20;
    t->height = 20;
    t->render = render_player;
    t->update = update_player;
    t->collide = collide_player;
}

eg_entity *player_demo_create()
{
    eg_entity *player = NULL;

    player = eg_create_entity();
    if (player == NULL)
    {
        return NULL;
    }

    player->id = ENTITY_TYPE_PLAYER;
    player->x_pos = 50;
    player->y_pos = 50;

    return player;
}