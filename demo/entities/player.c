#include "demo/entities/player.h"
#include "demo/entities/entity_types.h"
#include "colors.h"

#include <stdio.h>
#include <stdlib.h>

static void render_player(eg_app *app, eg_entity *player)
{
    eg_rect r;
    r.x = player->x_pos;
    r.y = player->y_pos;
    r.w = app->registry[player->id].width;
    r.h = app->registry[player->id].height;

    eg_set_color(app, EG_COLOR_ORANGE);
    eg_draw_rect(app, &r, 0);
}

static void update_player(eg_app *app, eg_entity *player)
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

void player_demo_register(eg_entity_type *t)
{
    t->width = 20;
    t->height = 32;
    t->render = render_player;
    t->update = update_player;
}

eg_entity *player_demo_create(int x, int y)
{
    eg_entity *player = NULL;

    player = eg_create_entity();
    if (player == NULL)
    {
        return NULL;
    }

    player->id = ENTITY_TYPE_PLAYER;
    player->x_pos = x;
    player->y_pos = y;

    return player;
}