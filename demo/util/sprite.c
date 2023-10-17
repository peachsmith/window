#include "demo/demo.h"
#include "demo/assets.h"
#include "demo/util/sprite.h"

void sprite_draw_frank(cr_app *app, int x, int y, int mirror, int tile)
{
    // tile dimensions
    int tile_w = 24;
    int tile_h = 24;

    // tile coordinates
    int tile_x = tile;
    int tile_y = 0;

    cr_rect src = {
        .x = tile_x * tile_w,
        .y = tile_y * tile_h,
        .w = tile_w,
        .h = tile_h};

    cr_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    cr_draw_texture(
        app,
        app->textures[DEMO_TEXTURE_CHARACTERS],
        &src,
        &dest,
        mirror);
}

void sprite_draw_jimbo(cr_app *app, int x, int y, int mirror, int tile)
{
    // tile dimensions
    int tile_w = 24;
    int tile_h = 24;

    // tile coordinates
    int tile_x = tile + 6;
    int tile_y = 0;

    cr_rect src = {
        .x = tile_x * tile_w,
        .y = tile_y * tile_h,
        .w = tile_w,
        .h = tile_h};

    cr_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    cr_draw_texture(
        app,
        app->textures[DEMO_TEXTURE_CHARACTERS],
        &src,
        &dest,
        mirror);
}

void sprite_draw_billy(cr_app *app, int x, int y, int mirror, int tile)
{
    // tile dimensions
    int tile_w = 24;
    int tile_h = 24;

    // tile coordinates
    int tile_x = tile + 4;
    int tile_y = 0;

    cr_rect src = {
        .x = tile_x * tile_w,
        .y = tile_y * tile_h,
        .w = tile_w,
        .h = tile_h};

    cr_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    cr_draw_texture(
        app,
        app->textures[DEMO_TEXTURE_CHARACTERS],
        &src,
        &dest,
        mirror);
}

void sprite_draw_henry(cr_app *app, int x, int y, int mirror, int tile)
{
    // tile dimensions
    int tile_w = 24;
    int tile_h = 24;

    // tile coordinates
    int tile_x = tile + 6;
    int tile_y = 1;

    cr_rect src = {
        .x = tile_x * tile_w,
        .y = tile_y * tile_h,
        .w = tile_w,
        .h = tile_h};

    cr_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    cr_draw_texture(
        app,
        app->textures[DEMO_TEXTURE_CHARACTERS],
        &src,
        &dest,
        mirror);
}

void sprite_draw_sign(cr_app *app, int x, int y)
{
    // tile dimensions
    int tile_w = 18;
    int tile_h = 18;

    // tile coordinates
    int tile_x = 6;
    int tile_y = 4;

    cr_rect src = {
        .x = tile_x * tile_w,
        .y = tile_y * tile_h,
        .w = tile_w,
        .h = tile_h};

    cr_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    cr_draw_texture(
        app,
        app->textures[DEMO_TEXTURE_SCENERY],
        &src,
        &dest,
        0);
}

void sprite_draw_brick(cr_app *app, int x, int y)
{
    // tile dimensions
    int tile_w = 18;
    int tile_h = 18;

    // tile coordinates
    int tile_x = 7;
    int tile_y = 2;

    cr_rect src = {
        .x = tile_x * tile_w,
        .y = tile_y * tile_h,
        .w = tile_w,
        .h = tile_h};

    cr_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    cr_draw_texture(
        app,
        app->textures[DEMO_TEXTURE_SCENERY],
        &src,
        &dest,
        0);
}

void sprite_draw_slope(cr_app *app, int x, int y, int length, int mirror)
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

    cr_rect src = {
        .x = tile_x * tile_w,
        .y = tile_y * tile_h,
        .w = tile_w,
        .h = tile_h};

    cr_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    for (int i = 0; i < length; i++)
    {
        cr_draw_texture(
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

void sprite_draw_flat_slope(cr_app *app, int x, int y, int length)
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

    cr_rect src = {
        .x = tile_x * tile_w,
        .y = tile_y * tile_h,
        .w = tile_w,
        .h = tile_h};

    cr_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    for (int i = 0; i < length; i++)
    {
        cr_draw_texture(
            app,
            app->textures[DEMO_TEXTURE_SCENERY],
            &src,
            &dest,
            0);

        dest.x += tile_w;
    }
}

void sprite_draw_grass_block(cr_app *app, int x, int y, int w, int h)
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
    cr_rect src = {
        .x = sheet_x * tile_w,
        .y = sheet_y * tile_h,
        .w = tile_w,
        .h = tile_h};

    // destination rectangle for menu tiles
    cr_rect dest = {
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

            cr_draw_texture(
                app,
                app->textures[DEMO_TEXTURE_SCENERY],
                &src,
                &dest,
                0);
        }
    }
}

void sprite_draw_background(cr_app *app, int type)
{
    cr_rect dest = {
        .x = 0,
        .y = 0,
        .w = app->screen_width,
        .h = app->screen_height};

    cr_set_color(app, 0xFF2090D0);
    cr_draw_rect(app, &dest, 1);
}

void sprite_draw_fireball(cr_app *app, int x, int y, int mirror, int tile)
{
    // tile dimensions
    int tile_w = 18;
    int tile_h = 18;

    // tile coordinates
    int tile_x = tile;
    int tile_y = 9;

    cr_rect src = {
        .x = tile_x * tile_w,
        .y = tile_y * tile_h,
        .w = tile_w,
        .h = tile_h};

    cr_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    cr_draw_texture(
        app,
        app->textures[DEMO_TEXTURE_SCENERY],
        &src,
        &dest,
        mirror);
}
