#include "demo/util/util.h"
#include "demo/collision/collision.h"

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