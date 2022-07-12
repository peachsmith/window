#include "demo/entities/player.h"
#include "demo/entities/entity_types.h"
#include "demo/texture/texture.h"
#include "demo/util/sprite.h"

#include <stdio.h>
#include <stdlib.h>

// temporary counter for animation
static int tmp_counter = 0;

static void render_player(eg_app *app, eg_entity *player)
{
    int tile = 0;
    int left_pressed = eg_peek_input(app, EG_KEYCODE_LEFT);
    int right_pressed = eg_peek_input(app, EG_KEYCODE_RIGHT);
    int grounded = eg_check_flag(player, ENTITY_FLAG_GROUND);

    // Animation logic for walking to the right
    if ((left_pressed || right_pressed) && grounded)
    {
        if (tmp_counter < 6)
        {
            tile = 0;
        }
        else if (tmp_counter < 16)
        {
            tile = 1;
        }
    }

    if (!grounded)
    {
        tile = 1;
    }

    // Render the player sprite.
    sprite_draw_character(
        app,
        player->x_pos,
        player->y_pos,
        eg_check_flag(player, ENTITY_FLAG_MIRROR), tile);

    // hit box
    if (app->debug.hitboxes)
    {
        eg_rect hit_box;
        hit_box.x = player->x_pos;
        hit_box.y = player->y_pos;
        hit_box.w = app->registry[player->type].width;
        hit_box.h = app->registry[player->type].height;

        // Render the player hit box.
        eg_set_color(app, EG_COLOR_ORANGE);
        eg_draw_rect(app, &hit_box, 0);
    }
}

static void update_player(eg_app *app, eg_entity *player)
{
    // Get the width and height of the player.
    int w = app->registry[player->type].width;
    int h = app->registry[player->type].height;

    // Check the MOVE flag to see if the player is being carried by a
    // a moving platform.
    int carry = eg_check_flag(player, ENTITY_FLAG_MOVE);

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
        // printf("[DEBUG] player V: (%d, %d)\n", player->x_vel, player->y_vel);
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

    // If the player is standing on a moving platform,
    // adjust the y velocity to match the platform.
    if (carry && player->carrier != NULL)
    {
        int cf = 0;

        // Determine the correction factor.
        // If the platform was already updated, recalculate the correction
        // factor that was done in the collision resolution.
        // Otherwise, we just add the platform's y velocity to the player's.
        if (eg_check_flag(player->carrier, ENTITY_FLAG_UPDATE))
        {
            cf = player->carrier->y_pos + app->cam.y - (player->y_pos + h);
        }
        else
        {
            cf = player->y_vel + player->carrier->y_vel;
        }

        player->y_vel = cf;
    }

    // Set the link pointer to NULL.
    if (!carry && player->carrier != NULL)
    {
        player->carrier = NULL;
    }

    // Clear the carry flag.
    eg_clear_flag(player, ENTITY_FLAG_MOVE);

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

    // If the player is on a sloped platform, set the y velocity to some
    // value that causes the player to collide with the platform before
    // moving into open space.
    if (eg_check_flag(player, ENTITY_FLAG_SLOPE))
    {
        eg_clear_flag(player, ENTITY_FLAG_SLOPE);
        player->y_vel = 4;
    }

    // Apply gravity.
    if (player->y_vel < 4)
    {
        player->y_vel++;
    }

    // Advance the counter for walking to the right
    if ((eg_peek_input(app, EG_KEYCODE_LEFT) || eg_peek_input(app, EG_KEYCODE_RIGHT)) && eg_check_flag(player, ENTITY_FLAG_GROUND))
    {
        tmp_counter++;
        if (tmp_counter >= 20)
        {
            tmp_counter = 0;
        }
    }
    else
    {
        tmp_counter = 0;
    }
}

void player_demo_register(eg_entity_type *t)
{
    t->width = 24;
    t->height = 24;
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

    player->id = DEBUG_PLAYER;
    player->type = ENTITY_TYPE_PLAYER;
    player->x_pos = x;
    player->y_pos = y;
    player->sprite_x = 0;
    player->sprite_y = 0;

    return player;
}