#include "demo/util/util.h"
#include "demo/collision/collision.h"
#include "demo/texture/texture.h"

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
