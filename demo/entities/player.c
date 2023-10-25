#include "demo/demo.h"
#include "demo/entities/entity_types.h"
#include "demo/entities/player.h"
#include "demo/util/sprite.h"

#include "common/util.h"

// temporary counter for animation
static int tmp_counter = 0;

#define PLAYER_X_ACC_LIMIT 24
#define PLAYER_Y_ACC_LIMIT 24
#define PLAYER_IFRAME_LIMIT 20

static int get_player_x_vel(cr_entity *player)
{
    // acceleration to velocity conversion table
    int a_to_v[PLAYER_X_ACC_LIMIT] = {
        1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3, 3,
        4, 4, 4, 4, 4, 4};

    // Make sure the acceleration is within the limit.
    // Any acceleration outside of the limits will result in
    // maximum velocity.
    int acc = player->x_acc;
    if (acc >= PLAYER_X_ACC_LIMIT)
    {
        acc = PLAYER_X_ACC_LIMIT - 1;
    }
    if (acc <= -PLAYER_X_ACC_LIMIT)
    {
        acc = -PLAYER_X_ACC_LIMIT + 1;
    }

    int x_vel = 0;
    if (acc > 0)
    {
        x_vel = a_to_v[acc];
    }
    else if (acc < 0)
    {
        x_vel = -(a_to_v[-acc]);
    }

    if (player->x_t)
    {
        x_vel += player->x_t;
    }

    return x_vel;
}

static int get_player_y_vel(cr_entity *player)
{
    // acceleration to velocity conversion table
    // int a_to_v[PLAYER_Y_ACC_LIMIT] = {
    //     1, 1, 1, 1, 1, 1,
    //     2, 2, 2, 2, 2, 2,
    //     3, 3, 3, 3, 3, 3,
    //     4, 4, 4, 4, 4, 4};
    int a_to_v[PLAYER_Y_ACC_LIMIT] = {
        1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3, 3,
        4, 4, 4, 4, 4, 4};

    // Make sure the acceleration is within the limit.
    // Any acceleration outside of the limits will result in
    // maximum velocity.
    int acc = player->y_acc;
    if (acc >= PLAYER_Y_ACC_LIMIT)
    {
        acc = PLAYER_Y_ACC_LIMIT - 1;
    }
    if (acc <= -PLAYER_Y_ACC_LIMIT)
    {
        acc = -PLAYER_Y_ACC_LIMIT + 1;
    }

    int y_vel = 0;
    if (acc > 0)
    {
        y_vel = a_to_v[acc];
    }
    else if (acc < 0)
    {
        y_vel = -(a_to_v[-acc]);
    }

    if (player->y_t)
    {
        y_vel += player->y_t;
    }

    return y_vel;
}

static void render_player(cr_app *app, cr_entity *player)
{
    if (!util_is_on_screen(app, player))
    {
        return;
    }

    int tile = 0;
    int left_pressed = cr_peek_input(app, CR_KEYCODE_LEFT);
    int right_pressed = cr_peek_input(app, CR_KEYCODE_RIGHT);
    int grounded = cr_check_flag(player, ENTITY_FLAG_GROUND);
    int iframes = 0; // whether or not the player currently has invincibility frames.

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

    if (cr_check_flag(player, ENTITY_FLAG_INVINCIBLE) && player->iframes % 2)
    {
        iframes = 1;
    }

    if (!iframes)
    {
        // Render the player sprite.
        sprite_draw_frank(
            app,
            player->x_pos,
            player->y_pos,
            cr_check_flag(player, ENTITY_FLAG_MIRROR), tile);
    }

    // hit box
    if (app->debug.hitboxes)
    {
        cr_rect hit_box;
        hit_box.x = player->x_pos;
        hit_box.y = player->y_pos;
        hit_box.w = app->entity_types[player->type].width;
        hit_box.h = app->entity_types[player->type].height;

        // Render the player hit box.
        cr_set_color(app, CR_COLOR_VINIK_ORANGE);
        cr_draw_rect(app, &hit_box, 0);
    }
}

static void update_player(cr_app *app, cr_entity *player)
{
    // Get the width and height of the player.
    int w = app->entity_types[player->type].width;
    int h = app->entity_types[player->type].height;

    // Check the MOVE flag to see if the player is being carried by a
    // a moving platform.
    int carried = cr_check_flag(player, ENTITY_FLAG_MOVE);
    int grounded = cr_check_flag(player, ENTITY_FLAG_GROUND);
    int left_pressed = cr_peek_input(app, CR_KEYCODE_LEFT);
    int right_pressed = cr_peek_input(app, CR_KEYCODE_RIGHT);

    int avx = get_player_x_vel(player);
    int avy = get_player_y_vel(player);

    //--------------------------------------------------------------------
    // Horizontal Movement

    // horizontal velocity applied by a carrier
    if (carried && avx == 0)
    {
        avx += player->x_vel;
    }

    // horizontal correction factor applied by collision with a solid object
    if (player->x_t)
    {
        player->x_acc = 0;
        player->x_t = 0;
    }

    // Update horizontal position.
    if (app->cam.config == CR_CAMERA_ALL && player->x_pos + w >= app->cam.cr && avx > 0)
    {
        int dcam = (player->x_pos + w) - app->cam.cr;
        player->x_pos = app->cam.cr - w;
        app->cam.x -= avx;
        app->cam.x -= dcam;
    }
    else if (app->cam.config == CR_CAMERA_ALL && player->x_pos <= app->cam.cl + 1 && avx < 0)
    {
        int dcam = player->x_pos - (app->cam.cl + 1);
        player->x_pos = app->cam.cl + 1;
        app->cam.x -= avx;
        app->cam.x -= dcam;
    }
    else
    {
        player->x_pos += avx;
    }

    // Perform horizontal inertia.
    if (player->x_acc > 0 && !right_pressed)
    {
        player->x_acc--;
    }

    if (player->x_acc < 0 && !left_pressed)
    {
        player->x_acc++;
    }

    //--------------------------------------------------------------------
    // Vertical Movement

    // If the player is standing on a moving platform,
    // adjust the y velocity to match the platform.
    if (carried && player->carrier != NULL)
    {
        int cf = 0;

        // Determine the correction factor.
        // If the platform was already updated, recalculate the correction
        // factor that was done in the collision resolution.
        // Otherwise, we just add the platform's y velocity to the player's.
        if (cr_check_flag(player->carrier, ENTITY_FLAG_UPDATE))
        {
            cf = player->carrier->y_pos + app->cam.y - (player->y_pos + h);
        }
        else
        {
            cf = player->y_vel + player->carrier->y_vel;
        }

        avy = cf;
    }

    // vertical correction factor applied by collision with a solid object
    if (player->y_t && !carried)
    {
        player->y_acc = 0;
        player->y_t = 0;
    }

    // Set the link pointer to NULL.
    if (!carried && player->carrier != NULL)
    {
        player->carrier = NULL;
    }

    // Clear the carry flag.
    cr_clear_flag(player, ENTITY_FLAG_MOVE);

    // Update vertical position.
    if (app->cam.config == CR_CAMERA_ALL && player->y_pos + h >= app->cam.cb && avy > 0)
    {
        int dcam = (player->y_pos + h) - app->cam.cb;
        player->y_pos = app->cam.cb - h;
        app->cam.y -= avy;
        app->cam.y -= dcam;
    }
    else if (app->cam.config == CR_CAMERA_ALL && player->y_pos <= app->cam.ct + 1 && avy < 0)
    {
        int dcam = player->y_pos - (app->cam.ct + 1);
        player->y_pos = app->cam.ct + 1;
        app->cam.y -= avy;
        app->cam.y -= dcam;
    }
    else
    {
        player->y_pos += avy;
    }

    // If the player is on a sloped platform, set the y velocity to some
    // value that causes the player to collide with the platform before
    // moving into open space.
    if (cr_check_flag(player, ENTITY_FLAG_SLOPE))
    {
        cr_clear_flag(player, ENTITY_FLAG_SLOPE);
        player->y_acc = PLAYER_Y_ACC_LIMIT - 1;
    }

    // Apply gravity.
    if (player->y_acc < PLAYER_Y_ACC_LIMIT - 1)
    {
        // jump height control
        int act = app->actuation_counters[CR_KEYCODE_SPACE];
        int delay_acceleration = 0;

        if (act && act <= 15)
        {
            if (act < 5)
            {
                delay_acceleration = 1;
            }
            else if (act < 12)
            {
                delay_acceleration = 1;
            }
        }

        if (!delay_acceleration)
        {
            player->y_acc++;
        }
    }

    //--------------------------------------------------------------------
    // Animation Logic

    // Invincibility frames.
    if (cr_check_flag(player, ENTITY_FLAG_INVINCIBLE))
    {
        player->iframes++;

        if (player->iframes >= PLAYER_IFRAME_LIMIT)
        {
            cr_clear_flag(player, ENTITY_FLAG_INVINCIBLE);
            player->iframes = 0;
        }
    }

    // Advance the counter for walking to the right
    if ((cr_peek_input(app, CR_KEYCODE_LEFT) || cr_peek_input(app, CR_KEYCODE_RIGHT)) && grounded)
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

void demo_register_player(cr_entity_type *t)
{
    t->width = 24;
    t->height = 24;
    t->render = render_player;
    t->update = update_player;
    t->get_x_vel = get_player_x_vel;
    t->get_y_vel = get_player_y_vel;
    t->control = 1;
}

cr_entity *demo_create_player(cr_app *app, int x, int y)
{
    cr_entity *player = NULL;

    player = cr_create_entity(app);
    if (player == NULL)
    {
        return NULL;
    }

    player->type = ENTITY_TYPE_PLAYER;
    player->x_pos = x;
    player->y_pos = y;

    return player;
}