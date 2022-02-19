#include "demo/entities/player.h"
#include "demo/entities/entity_types.h"

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
    // Get the width and height of the player.
    int w = app->registry[player->id].width;
    int h = app->registry[player->id].height;

    // Update horizontal position.
    if (player->x_pos + w >= app->cr && player->x_vel > 0)
    {
        player->x_pos = app->cr - w;
        app->cam.x -= player->x_vel;
    }
    else if (player->x_pos <= app->cl + 1 && player->x_vel < 0)
    {
        player->x_pos = app->cl + 1;
        app->cam.x -= player->x_vel;
    }
    else
    {
        player->x_pos += player->x_vel;
    }

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
    if (player->y_pos + h >= app->cb && player->y_vel > 0)
    {
        player->y_pos = app->cb - h;
        app->cam.y -= player->y_vel;
    }
    else if (player->y_pos <= app->ct + 1 && player->y_vel < 0)
    {
        player->y_pos = app->ct + 1;
        app->cam.y -= player->y_vel;
    }
    else
    {
        player->y_pos += player->y_vel;
    }

    // Apply gravity.
    if (player->y_vel < 4)
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