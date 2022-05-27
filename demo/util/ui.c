#include "demo/util/ui.h"
#include "demo/texture/texture.h"

void ui_draw_panel(eg_app *app, int x, int y, int w, int h)
{
    // dimensions of each tile from the sprite sheet
    int tile_w = 16;
    int tile_h = 16;

    // number of pixels between each tile
    int padding = 2;

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

    // tile coordinates in the sprite sheet for the menu elements
    // These numbers will be multiplied by the tile dimensions to calculate
    // the pixel coordinates.
    int tile_x_left = 3;
    int tile_x_mid = 4;
    int tile_x_right = 5;
    int tile_y_top = 18;
    int tile_y_mid = 19;
    int tile_y_bottom = 20;

    int sheet_x = tile_x_left;
    int sheet_y = tile_y_top;

    // source rectangle for menu tiles
    // There is a 2 pixel margin between each tile in the sprite sheet.
    eg_rect src = {
        .x = sheet_x * (tile_w + padding),
        .y = sheet_y * (tile_h + padding),
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

            src.x = sheet_x * (tile_w + padding);
            src.y = sheet_y * (tile_h + padding);
            dest.x = col;
            dest.y = row;

            eg_draw_texture(app, app->textures[DEMO_TEXTURE_UI], &src, &dest, 0);
        }
    }
}

void ui_draw_text(eg_app *app)
{
}

void ui_draw_cursor(eg_app *app, int x, int y)
{
    int tile_w = 16;
    int tile_h = 16;
    int padding = 2;

    // tile coordinates in the sprite sheet for the cursor
    int cursor_sheet_x = 5;
    int cursor_sheet_y = 26;

    eg_rect cusor_src = {
        .x = cursor_sheet_x * (tile_w + padding),
        .y = cursor_sheet_y * (tile_h + padding),
        .w = tile_w,
        .h = tile_h};

    eg_rect cusor_dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    eg_draw_texture(app,
                    app->textures[DEMO_TEXTURE_UI],
                    &cusor_src,
                    &cusor_dest,
                    0);
}

void ui_draw_indicator(eg_app *app, int x, int y)
{
    int tile_w = 16;
    int tile_h = 16;
    int padding = 2;

    // tile coordinates for the dialog advancement indicator
    int sheet_x = 5;
    int sheet_y = 10;

    eg_rect ind_src = {
        .x = sheet_x * (tile_w + padding),
        .y = sheet_y * (tile_h + padding),
        .w = tile_w,
        .h = tile_h};

    eg_rect ind_dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    eg_draw_texture(
        app,
        app->textures[DEMO_TEXTURE_UI],
        &ind_src,
        &ind_dest,
        0);
}
