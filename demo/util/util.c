#include "demo/util/util.h"
#include "demo/collision/collision.h"

#include <stdio.h>

void demo_draw_collision(
    eg_app *app,
    eg_rect *r,
    eg_collision *res,
    eg_point *p,
    eg_point *d)
{
    eg_rect target_bounds;
    eg_rect cp_rect;
    eg_point p0;
    eg_point p1;

    // Draw the target rectangle in red.
    // The dimensions should be the width and height of the target entity
    // plus the width and height of the source entity.
    target_bounds.x = r->x;
    target_bounds.y = r->y;
    target_bounds.w = r->w;
    target_bounds.h = r->h;
    eg_set_color(app, EG_COLOR_RED);
    eg_draw_rect(app, &target_bounds, 0);

    // Draw the contact point CP in cyan.
    eg_set_color(app, EG_COLOR_CYAN);
    cp_rect.x = res->cp.x - 5;
    cp_rect.y = res->cp.y - 5;
    cp_rect.w = 10;
    cp_rect.h = 10;
    eg_draw_rect(app, &cp_rect, 1);

    // Draw the contact normal CN in a pink.
    eg_set_color(app, EG_COLOR_PINK);
    p0.x = res->cp.x;
    p0.y = res->cp.y;
    p1.x = res->cp.x + res->cn.x * 20;
    p1.y = res->cp.y + res->cn.y * 20;
    eg_draw_line(app, &p0, &p1);

    // Draw the direction vector D in green.
    eg_set_color(app, EG_COLOR_SEA_GREEN);
    p0.x = p->x;
    p0.y = p->y;
    p1.x = p->x + d->x * 10;
    p1.y = p->y + d->y * 10;
    eg_draw_line(app, &p0, &p1);
}

void demo_draw_sat(
    eg_app *app,
    eg_point *pa,
    eg_point *pb,
    eg_point *n0,
    eg_point *n1,
    eg_point *q)
{

    eg_rect rects[7];

    // All of the vertex rectangles will have a width of 4.
    for (int i = 0; i < 7; i++)
    {
        rects[i].w = 4;
        rects[i].h = 4;
    }

    // Create rectangles at each vertex of entity A.
    rects[0].x = pa[0].x - 2;
    rects[0].y = pa[0].y - 2;
    rects[1].x = pa[1].x - 2;
    rects[1].y = pa[1].y - 2;
    rects[2].x = pa[2].x - 2;
    rects[2].y = pa[2].y - 2;
    rects[3].x = pa[3].x - 2;
    rects[3].y = pa[3].y - 2;

    // Create rectangles at each vertex of entity B.
    rects[4].x = pb[0].x - 2;
    rects[4].y = pb[0].y - 2;
    rects[5].x = pb[1].x - 2;
    rects[5].y = pb[1].y - 2;
    rects[6].x = pb[2].x - 2;
    rects[6].y = pb[2].y - 2;

    // Draw the vertices of entity A.
    eg_set_color(app, EG_COLOR_LAVENDAR);
    for (int i = 0; i < 4; i++)
    {
        eg_draw_rect(app, &(rects[i]), 1);
    }

    // Draw the vertices of entity B.
    eg_set_color(app, EG_COLOR_YELLOW);
    for (int i = 4; i < 7; i++)
    {
        eg_draw_rect(app, &(rects[i]), 1);
    }

    // Draw the normal axis.
    eg_set_color(app, EG_COLOR_MILITARY_GREEN);
    eg_draw_line(app, n0, n1);

    // Draw the normal origin Q.
    eg_set_color(app, EG_COLOR_LIGHT_GRAY);
    eg_rect rq = {.x = q->x - 2, .y = q->y - 2, .w = 4, .h = 4};
    eg_draw_rect(app, &rq, 1);
}

void demo_draw_camera(eg_app *app)
{
    eg_point l0, l1, r0, r1, t0, t1, b0, b1;

    eg_set_color(app, EG_COLOR_MAUVE);

    // left boundary
    l0.x = app->cl;
    l0.y = 0;
    l1.x = app->cl;
    l1.y = app->screen_height;
    eg_draw_line(app, &l0, &l1);

    // right boundary
    r0.x = app->cr;
    r0.y = 0;
    r1.x = app->cr;
    r1.y = app->screen_height;
    eg_draw_line(app, &r0, &r1);

    eg_set_color(app, EG_COLOR_TEAL);

    // top boundary
    t0.x = 0;
    t0.y = app->ct;
    t1.x = app->screen_width;
    t1.y = app->ct;
    eg_draw_line(app, &t0, &t1);

    // bottom boundary
    b0.x = 0;
    b0.y = app->cb;
    b1.x = app->screen_width;
    b1.y = app->cb;
    eg_draw_line(app, &b0, &b1);
}

void demo_draw_panel(eg_app *app, eg_rect *rect)
{
    // tile coordinates in the sprite sheet for the menu elements
    // These numbers will be multiplied by the tile dimensions to calculate
    // the pixel coordinates.
    int tile_x_left = 3;
    int tile_x_mid = 4;
    int tile_x_right = 5;
    int tile_y_top = 18;
    int tile_y_mid = 19;
    int tile_y_bottom = 20;

    // dimensions of each tile from the sprite sheet
    int tile_w = 16;
    int tile_h = 16;

    // dimensions of the panel in tiles
    // int panel_w = 13;
    // int panel_h = 5;

    int sheet_x = tile_x_left;
    int sheet_y = tile_y_top;

    // source rectangle for menu tiles
    // There is a 2 pixel margin between each tile in the sprite sheet.
    eg_rect src = {
        .x = sheet_x * (tile_w + 2),
        .y = sheet_y * (tile_h + 2),
        .w = tile_w,
        .h = tile_h};

    // destination rectangle for menu tiles
    eg_rect dest = {
        .x = rect->x,
        .y = rect->y,
        .w = tile_w,
        .h = tile_h};

    int right_limit = rect->x + tile_w * (rect->w - 1);
    int bottom_limit = rect->y + tile_h * (rect->h - 1);

    for (int row = rect->y; row < rect->y + tile_h * rect->h; row += tile_h)
    {
        for (int col = rect->x; col < rect->x + tile_w * rect->w; col += tile_w)
        {
            // top
            if (row == rect->y)
            {
                sheet_y = tile_y_top;

                // top left corner
                if (col == rect->x)
                {
                    sheet_x = tile_x_left;
                }

                // middle
                if (col > rect->x && col < right_limit)
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
            if (row > rect->y && row < bottom_limit)
            {
                sheet_y = tile_y_mid;

                // left side
                if (col == rect->x)
                {
                    sheet_x = tile_x_left;
                }

                // middle
                if (col > rect->x && col < right_limit)
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
                if (col == rect->x)
                {
                    sheet_x = tile_x_left;
                }

                // middle
                if (col > rect->x && col < right_limit)
                {
                    sheet_x = tile_x_mid;
                }

                // bottom right corner
                if (col == right_limit)
                {
                    sheet_x = tile_x_right;
                }
            }

            src.x = sheet_x * (tile_w + 2);
            src.y = sheet_y * (tile_h + 2);
            dest.x = col;
            dest.y = row;

            eg_draw_texture(app, app->textures[0], &src, &dest);
        }
    }
}
