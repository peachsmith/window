#include "demo/entities/player.h"
#include "demo/entities/entity_types.h"

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

void player_demo_register(eg_entity_type *t)
{
    t->width = 20;
    t->height = 20;
    t->render = render_player;
    t->update = update_player;
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