#include "demo/collision/collision.h"
#include "demo/util/util.h"

#include <float.h>
#include <stdlib.h>
#include <stdio.h>

static int intersect(
    eg_point *p0,
    eg_point *p1,
    eg_point *q0,
    eg_point *q1,
    float *tx,
    float *ty)
{
    // line from centroid to bottom left corner
    float line_r1s_x = (float)p0->x;
    float line_r1s_y = (float)p0->y;
    float line_r1e_x = (float)(p1->x);
    float line_r1e_y = (float)(p1->y);

    // right side of player entity
    float line_r2s_x = (float)(q0->x);
    float line_r2s_y = (float)(q0->y);
    float line_r2e_x = (float)(q1->x);
    float line_r2e_y = (float)(q1->y);

    float ih = (line_r2e_x - line_r2s_x) * (line_r1s_y - line_r1e_y) - (line_r1s_x - line_r1e_x) * (line_r2e_y - line_r2s_y);
    float it1 = ((line_r2s_y - line_r2e_y) * (line_r1s_x - line_r2s_x) + (line_r2e_x - line_r2s_x) * (line_r1s_y - line_r2s_y)) / ih;
    float it2 = ((line_r1s_y - line_r1e_y) * (line_r1s_x - line_r2s_x) + (line_r1e_x - line_r1s_x) * (line_r1s_y - line_r2s_y)) / ih;

    if (it1 >= 0.0f && it1 < 1.0f && it2 >= 0.0f && it2 < 1.0f)
    {
        *tx = (1.0f - it1) * (line_r1e_x - line_r1s_x);
        *ty = (1.0f - it1) * (line_r1e_y - line_r1s_y);
        return 1;
        // printf("[DEBUG] line intersection collision (%.2f, %.2f)\n", tx, ty);
    }

    return 0;
}

/**
 * An alternative collision detection function based on the code by David Barr.
 * Video: https://www.youtube.com/watch?v=7Ik2vowGcU0
 * Source: https://github.com/OneLoneCoder/olcPixelGameEngine/blob/master/Videos/OneLoneCoder_PGE_PolygonCollisions1.cpp
 */
int demo_sat(
    eg_app *app,
    eg_entity *a,
    eg_entity *b,
    eg_collision *res)
{
    eg_point pa[4]; // vertices of entity A
    eg_point pb[3]; // vertices of entity B

    // Get the width and height of each entity.
    int aw = app->registry[a->type].width;
    int ah = app->registry[a->type].height;
    int bw = app->registry[b->type].width;
    int bh = app->registry[b->type].height;

    // Get the positions of each entity.
    // TODO: account for camera position.
    int ax = a->x_pos;
    int ay = a->y_pos;
    int bx = b->x_pos;
    int by = b->y_pos;

    // Get the vertices for entity A.
    // Entity A is assumed to be a rectangle, so there are four points.

    // top left vertex
    pa[0].x = ax;
    pa[0].y = ay;

    // top right vertex
    pa[1].x = ax + aw;
    pa[1].y = ay;

    // bottom right vertex
    pa[2].x = ax + aw;
    pa[2].y = ay + ah;

    // bottom left vertex
    pa[3].x = ax;
    pa[3].y = ay + ah;

    // Get the vertices for entity B.
    // Entity B is assumed to be a triangle, so there are three points.
    // For now, we assume that entity B is a right triangle with a hypoteneus
    // connecting the bottom left and top right vertices.

    // top right vertex
    pb[0].x = bx + bw;
    pb[0].y = by;

    // bottom right vertex
    pb[1].x = bx + bw;
    pb[1].y = by + bh;

    // bottom left vertex
    pb[2].x = bx;
    pb[2].y = by + bh;

    // Find the Centroid of the triangle.
    eg_point c = {.x = pb[2].x + ((2 * bw) / 3), .y = pb[2].y - bh / 3};

    // Line intersection check.

    // line from centroid to bottom left corner
    // float line_r1s_x = (float)c.x;
    // float line_r1s_y = (float)c.y;
    // float line_r1e_x = (float)(pb[2].x);
    // float line_r1e_y = (float)(pb[2].y);

    // // right side of player entity
    // float line_r2s_x = (float)(pa[1].x);
    // float line_r2s_y = (float)(pa[1].y);
    // float line_r2e_x = (float)(pa[2].x);
    // float line_r2e_y = (float)(pa[2].y);

    // float ih = (line_r2e_x - line_r2s_x) * (line_r1s_y - line_r1e_y) - (line_r1s_x - line_r1e_x) * (line_r2e_y - line_r2s_y);
    // float it1 = ((line_r2s_y - line_r2e_y) * (line_r1s_x - line_r2s_x) + (line_r2e_x - line_r2s_x) * (line_r1s_y - line_r2s_y)) / ih;
    // float it2 = ((line_r1s_y - line_r1e_y) * (line_r1s_x - line_r2s_x) + (line_r1e_x - line_r1s_x) * (line_r1s_y - line_r2s_y)) / ih;

    float tx;
    float ty;

    // if (it1 >= 0.0f && it1 < 1.0f && it2 >= 0.0f && it2 < 1.0f)
    // {
    if (intersect(&c, &(pb[2]), &(pa[1]), &(pa[2]), &tx, &ty))
    {
        // float tx = (1.0f - t1) * (pb[2].x - c.x);
        // float ty = (1.0f - t1) * (pb[2].y - c.y);
        printf("[DEBUG] line intersection collision (%.2f, %.2f)\n", tx, ty);
    }

    // printf("[DEBUG] C: (%d, %d)\n", c.x, c.y);
    eg_rect cr = {.x = c.x - 2, .y = c.y - 2, .w = 4, .h = 4};
    eg_set_color(app, EG_COLOR_INDIGO);
    eg_draw_rect(app, &cr, 1);
    eg_draw_line(app, &c, &(pb[2]));

    // demo_draw_sat(app, pa, pb, &n0, &n1, &q);

    return 0;
}
