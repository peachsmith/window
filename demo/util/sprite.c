#include "demo/util/sprite.h"
#include "demo/texture/texture.h"

#include "colors.h"

void sprite_draw_character(eg_app *app, int x, int y, int mirror, int tile)
{
    // tile dimensions
    int tile_w = 24;
    int tile_h = 24;

    // tile coordinates
    int tile_x = tile;
    int tile_y = 0;

    eg_rect src = {
        .x = tile_x * tile_w,
        .y = tile_y * tile_h,
        .w = tile_w,
        .h = tile_h};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    eg_draw_texture(
        app,
        app->textures[DEMO_TEXTURE_CHARACTERS],
        &src,
        &dest,
        mirror);
}

void sprite_draw_jimbo(eg_app *app, int x, int y, int mirror, int tile)
{
    // tile dimensions
    int tile_w = 24;
    int tile_h = 24;

    // tile coordinates
    int tile_x = tile + 6;
    int tile_y = 0;

    eg_rect src = {
        .x = tile_x * tile_w,
        .y = tile_y * tile_h,
        .w = tile_w,
        .h = tile_h};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    eg_draw_texture(
        app,
        app->textures[DEMO_TEXTURE_CHARACTERS],
        &src,
        &dest,
        mirror);
}

void sprite_draw_billy(eg_app *app, int x, int y, int mirror, int tile)
{
    // tile dimensions
    int tile_w = 24;
    int tile_h = 24;

    // tile coordinates
    int tile_x = tile + 4;
    int tile_y = 0;

    eg_rect src = {
        .x = tile_x * tile_w,
        .y = tile_y * tile_h,
        .w = tile_w,
        .h = tile_h};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    eg_draw_texture(
        app,
        app->textures[DEMO_TEXTURE_CHARACTERS],
        &src,
        &dest,
        mirror);
}

void sprite_draw_henry(eg_app *app, int x, int y, int mirror, int tile)
{
    // tile dimensions
    int tile_w = 24;
    int tile_h = 24;

    // tile coordinates
    int tile_x = tile + 6;
    int tile_y = 1;

    eg_rect src = {
        .x = tile_x * tile_w,
        .y = tile_y * tile_h,
        .w = tile_w,
        .h = tile_h};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    eg_draw_texture(
        app,
        app->textures[DEMO_TEXTURE_CHARACTERS],
        &src,
        &dest,
        mirror);
}

void sprite_draw_sign(eg_app *app, int x, int y)
{
    // tile dimensions
    int tile_w = 18;
    int tile_h = 18;

    // tile coordinates
    int tile_x = 6;
    int tile_y = 4;

    eg_rect src = {
        .x = tile_x * tile_w,
        .y = tile_y * tile_h,
        .w = tile_w,
        .h = tile_h};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    eg_draw_texture(
        app,
        app->textures[DEMO_TEXTURE_SCENERY],
        &src,
        &dest,
        0);
}

void sprite_draw_brick(eg_app *app, int x, int y)
{
    // tile dimensions
    int tile_w = 18;
    int tile_h = 18;

    // tile coordinates
    int tile_x = 7;
    int tile_y = 2;

    eg_rect src = {
        .x = tile_x * tile_w,
        .y = tile_y * tile_h,
        .w = tile_w,
        .h = tile_h};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    eg_draw_texture(
        app,
        app->textures[DEMO_TEXTURE_SCENERY],
        &src,
        &dest,
        0);
}

void sprite_draw_slope(eg_app *app, int x, int y, int length, int mirror)
{
    // Only slopes from 1 - 4 tiles in length are supported.
    if (length < 1 || length > 4)
    {
        return;
    }

    // tile dimensions
    int tile_w = 18;
    int tile_h = 18;

    // tile coordinates
    int tile_x = 0;
    int tile_y = 9;

    if (mirror)
    {
        tile_x = 3;
    }

    eg_rect src = {
        .x = tile_x * tile_w,
        .y = tile_y * tile_h,
        .w = tile_w,
        .h = tile_h};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    for (int i = 0; i < length; i++)
    {
        eg_draw_texture(
            app,
            app->textures[DEMO_TEXTURE_SCENERY],
            &src,
            &dest,
            mirror);

        if (mirror)
        {
            src.x -= tile_w;
        }
        else
        {
            src.x += tile_w;
        }

        dest.x += tile_w;
    }
}

void sprite_draw_flat_slope(eg_app *app, int x, int y, int length)
{
    // Only slopes from 1 - 4 tiles in length are supported.
    if (length < 1 || length > 4)
    {
        return;
    }

    // tile dimensions
    int tile_w = 18;
    int tile_h = 18;

    // tile coordinates
    int tile_x = 2;
    int tile_y = 1;

    eg_rect src = {
        .x = tile_x * tile_w,
        .y = tile_y * tile_h,
        .w = tile_w,
        .h = tile_h};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    for (int i = 0; i < length; i++)
    {
        eg_draw_texture(
            app,
            app->textures[DEMO_TEXTURE_SCENERY],
            &src,
            &dest,
            0);

        dest.x += tile_w;
    }
}

void sprite_draw_grass_block(eg_app *app, int x, int y, int w, int h)
{
    // tile dimensions
    int tile_w = 18;
    int tile_h = 18;

    // tile coordinates
    int tile_x_left = 1;
    int tile_x_mid = 2;
    int tile_x_right = 3;
    int tile_y_top = 1;
    int tile_y_mid = 6;
    int tile_y_bottom = 7;

    // The width and height of the panel cannot be less than the width and
    // height of the tiles used to build it.
    if (w < tile_w)
    {
        w = tile_w;
    }

    if (h < tile_h)
    {
        h = tile_h;
    }

    // If the height is less than the tile height,
    // we change the y coordinate to use tiles that have a bottom border.
    if (h == tile_h)
    {
        tile_y_top = 0;
        tile_y_mid = 0;
        tile_y_bottom = 0;
    }

    // If the width is less than the tile width,
    // we change the x coordinate to use tiles that have
    // left and right borders.
    if (w == tile_w)
    {
        tile_x_left = 0;
        tile_x_mid = 0;
        tile_x_right = 0;
    }

    int sheet_x = tile_x_left;
    int sheet_y = tile_y_top;

    // source rectangle for menu tiles
    // There is a 2 pixel margin between each tile in the sprite sheet.
    eg_rect src = {
        .x = sheet_x * tile_w,
        .y = sheet_y * tile_h,
        .w = tile_w,
        .h = tile_h};

    // destination rectangle for menu tiles
    eg_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    int right_limit = x + w - tile_w;
    int bottom_limit = y + h - tile_h;

    for (int row = y; row < y + h; row += tile_h)
    {
        if (row + tile_h > y + h)
        {
            row = bottom_limit - tile_h;
        }

        for (int col = x; col < x + w; col += tile_w)
        {
            if (col + tile_w > x + w)
            {
                col = right_limit - tile_w;
            }

            // top
            if (row == y)
            {
                sheet_y = tile_y_top;

                // top left corner
                if (col == x)
                {
                    sheet_x = tile_x_left;
                }

                // middle
                if (col > x && col < right_limit)
                {
                    sheet_x = tile_x_mid;
                }

                // top right corner
                if (col == right_limit)
                {
                    sheet_x = tile_x_right;
                }
            }

            // middle
            if (row > y && row < bottom_limit)
            {
                sheet_y = tile_y_mid;

                // left side
                if (col == x)
                {
                    sheet_x = tile_x_left;
                }

                // middle
                if (col > x && col < right_limit)
                {
                    sheet_x = tile_x_mid;
                }

                // right side
                if (col == right_limit)
                {
                    sheet_x = tile_x_right;
                }
            }

            // bottom
            if (row == bottom_limit)
            {
                sheet_y = tile_y_bottom;

                // bottom left corner
                if (col == x)
                {
                    sheet_x = tile_x_left;
                }

                // middle
                if (col > x && col < right_limit)
                {
                    sheet_x = tile_x_mid;
                }

                // bottom right corner
                if (col == right_limit)
                {
                    sheet_x = tile_x_right;
                }
            }

            src.x = sheet_x * tile_w;
            src.y = sheet_y * tile_h;
            dest.x = col;
            dest.y = row;

            eg_draw_texture(
                app,
                app->textures[DEMO_TEXTURE_SCENERY],
                &src,
                &dest,
                0);
        }
    }
}

void sprite_draw_background(eg_app *app, int type)
{
    eg_rect dest = {
        .x = 0,
        .y = 0,
        .w = app->screen_width,
        .h = app->screen_height};

    eg_set_color(app, 0xFF2090D0); // EG_COLOR_CORNFLOWER_BLUE);
    eg_draw_rect(app, &dest, 1);
}

void sprite_draw_fireball(eg_app *app, int x, int y, int mirror, int tile)
{
    // tile dimensions
    int tile_w = 18;
    int tile_h = 18;

    // tile coordinates
    int tile_x = tile;
    int tile_y = 9;

    eg_rect src = {
        .x = tile_x * tile_w,
        .y = tile_y * tile_h,
        .w = tile_w,
        .h = tile_h};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    eg_draw_texture(
        app,
        app->textures[DEMO_TEXTURE_SCENERY],
        &src,
        &dest,
        mirror);
}

void sprite_draw_corgi(eg_app *app, int x, int y, int mirror, int tile)
{
    // tile dimensions
    int tile_w = 24;
    int tile_h = 24;

    // tile coordinates
    int tile_x = tile;
    int tile_y = 0;

    eg_rect src = {
        .x = tile_x * tile_w,
        .y = tile_y * tile_h,
        .w = tile_w,
        .h = tile_h};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    eg_draw_texture(
        app,
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        mirror);
}

void sprite_draw_note(eg_app *app, int x, int y)
{
    // tile dimensions
    int tile_w = 24;
    int tile_h = 24;

    // tile coordinates
    int tile_x = 4;
    int tile_y = 0;

    // The note sprite is 11 x 11 pixels, so we subtract 13
    // from the tile width and height.
    eg_rect src = {
        .x = tile_x * tile_w,
        .y = tile_y * tile_h,
        .w = tile_w - 13,
        .h = tile_h - 13};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w - 13,
        .h = tile_h - 13};

    eg_draw_texture(
        app,
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        0);
}

void sprite_draw_critter(eg_app *app, int x, int y, int mirror, int tile)
{
    // tile dimensions
    int tile_w = 20;
    int tile_h = 12;

    // tile coordinates
    int tile_x = tile;
    int tile_y = 9;

    eg_rect src = {
        .x = 96 + tile * tile_w,
        .y = 11,
        .w = tile_w,
        .h = tile_h};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    eg_draw_texture(
        app,
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        mirror);
}

void sprite_draw_sparkle(eg_app *app, int x, int y, int tile)
{
    // tile dimensions
    int tile_w = 19;
    int tile_h = 9;

    eg_rect src = {
        .x = 116 + tile * tile_w,
        .y = 0,
        .w = tile_w,
        .h = tile_h};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    eg_draw_texture(
        app,
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        0);
}

void sprite_draw_puff(eg_app *app, int x, int y, int tile)
{
    // tile dimensions
    int tile_w = 16;
    int tile_h = 8;

    eg_rect src = {
        .x = 158 + tile * tile_w,
        .y = 10,
        .w = tile_w,
        .h = tile_h};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    eg_draw_texture(
        app,
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        0);
}

void sprite_draw_leaves(eg_app *app, int x, int y, int tile)
{
    // tile dimensions
    int tile_w = 20;
    int tile_h = 12;

    eg_rect src = {
        .x = 96 + tile * tile_w,
        .y = 24,
        .w = tile_w,
        .h = tile_h};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    eg_draw_texture(
        app,
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        0);
}

void sprite_draw_trumpet(eg_app *app, int x, int y, int mirror)
{
    eg_rect src = {
        .x = 137,
        .y = 25,
        .w = 14,
        .h = 8};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = 14,
        .h = 8};

    eg_draw_texture(
        app,
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        mirror);
}

void sprite_draw_ground(eg_app *app, int x, int y)
{
    eg_rect src = {
        .x = 0,
        .y = 24,
        .w = 24,
        .h = 18};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = 24,
        .h = 18};

    eg_draw_texture(
        app,
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        0);
}

void sprite_draw_forest(eg_app * app, int x, int y)
{
    eg_rect src = {
        .x = 0,
        .y = 44,
        .w = 240,
        .h = 120};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = 240,
        .h = 120};

    eg_draw_texture(
        app,
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        0);
}
