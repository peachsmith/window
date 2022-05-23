#include "demo/collision/collision.h"
#include "demo/util/util.h"
#include "demo/entities/entity_types.h"

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
int demo_line(
    eg_app *app,
    eg_entity *a,
    eg_entity *b,
    eg_collision *res)
{
    // If the source entity is not a collider, then don't bother checking
    // for a collision.
    if (a->type != ENTITY_TYPE_PLAYER)
    {
        return 0;
    }

    eg_point pa[4]; // vertices of entity A
    eg_point pb[4]; // vertices of entity B

    // Get the width and height of each entity.
    int aw = app->registry[a->type].width;
    int ah = app->registry[a->type].height;
    int bw = app->registry[b->type].width;
    int bh = app->registry[b->type].height;

    // Get the positions of each entity.
    // TODO: account for camera position.
    int ax = a->x_pos;
    int ay = a->y_pos;
    int bx = b->x_pos + app->cam.x;
    int by = b->y_pos + app->cam.y;

    // Get the vertices for entity A.

    // top left vertex
    pa[0].x = ax + a->x_vel;
    pa[0].y = ay; // + a->y_vel;

    // top right vertex
    pa[1].x = ax + aw + a->x_vel;
    pa[1].y = ay + a->y_vel;

    // bottom right vertex
    pa[2].x = ax + aw / 2 + a->x_vel;
    pa[2].y = ay + ah + a->y_vel;

    // bottom left vertex
    pa[3].x = ax + a->x_vel;
    pa[3].y = ay + ah + a->y_vel;

    // Get the vertices for entity B.

    // top left vertex
    pb[0].x = bx;
    pb[0].y = by;

    // top right vertex
    pb[1].x = bx + bw;
    pb[1].y = by;

    // bottom right vertex
    pb[2].x = bx + bw;
    pb[2].y = by + bh;

    // bottom left vertex
    pb[3].x = bx;
    pb[3].y = by + bh;

    // Find the Centroid of the triangle.
    // eg_point c = {.x = pb[2].x + ((2 * bw) / 3), .y = pb[2].y - bh / 3};

    // The starting point of the source line.
    eg_point aa = {.x = pa[0].x, .y = pa[0].y + ah / 2};
    eg_point ab = pa[2];

    // Check the flags of the platform to see which direction the diagonal
    // line is sloped.
    int dir = b->flags & 3;
    eg_point ba = pb[3];
    eg_point bb = pb[1];

    if (dir == 0)
    {
        ba = pb[0];
        bb = pb[2];
        aa.x += aw;
    }
    else if (dir == 2)
    {
        ba = pb[0];
        bb = pb[1];
    }

    // // Draw the source line.
    // // printf("[DEBUG] C: (%d, %d)\n", c.x, c.y);
    // eg_rect cr = {.x = aa.x - 2, .y = aa.y - 2, .w = 4, .h = 4};
    // eg_set_color(app, EG_COLOR_INDIGO);
    // eg_draw_rect(app, &cr, 1);
    // eg_draw_line(app, &aa, &ab);

    if (intersect(&aa, &ab, &ba, &bb, &(res->tx), &(res->ty)))
    {
        if (dir == 2)
        {
            printf("[DEBUG] horizontal slope detected. ty: %.2f, ay: %d, ah: %d, by: %d\n",
                   res->ty,
                   ay,
                   ah,
                   by);

            // Draw the source line.
            // printf("[DEBUG] C: (%d, %d)\n", c.x, c.y);
            eg_rect cr = {.x = aa.x - 2, .y = aa.y - 2, .w = 4, .h = 4};
            eg_set_color(app, EG_COLOR_INDIGO);
            eg_draw_rect(app, &cr, 1);
            eg_draw_line(app, &aa, &ab);
        }

        // printf("[DEBUG] player V0: (%d, %d)\n", a->x_vel, a->y_vel);
        return 1;
    }

    return 0;
}
