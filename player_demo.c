#include "player_demo.h"

#include <stdlib.h>

static void render_player(eg_app *app, eg_entity *player)
{
    SDL_Rect r;
    r.x = player->x_pos;
    r.y = player->y_pos;
    r.w = player->width;
    r.h = player->height;

    SDL_SetRenderDrawColor(app->renderer, 0xF0, 0xAD, 0x4E, 0xFF);
    SDL_RenderFillRect(app->renderer, &r);
}

static void update_player(eg_app *app, eg_entity *player)
{
    // Update the player entity's horizontal and vertical position by adding
    // the velocity.
    player->x_pos += player->x_vel;
    player->y_pos += player->y_vel;

    // Implement a primitive form of inertia by increasing or deccreasing the
    // velocity on each update until the velocity reaches 0.

    if (player->x_vel > 0)
    {
        player->x_vel--;
    }

    if (player->x_vel < 0)
    {
        player->x_vel++;
    }

    if (player->y_vel > 0)
    {
        player->y_vel--;
    }

    if (player->y_vel < 0)
    {
        player->y_vel++;
    }
}

eg_entity *player_demo_create()
{
    eg_entity *player = NULL;

    player = eg_create_entity();
    if (player == NULL)
    {
        return NULL;
    }

    player->x_pos = 50;
    player->y_pos = 50;
    player->width = 20;
    player->height = 20;
    player->render = render_player;
    player->update = update_player;

    return player;
}