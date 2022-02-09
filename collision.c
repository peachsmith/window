#include "collision.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

/**
 * Draws various elements of a collision event on the screen.
 * This function is intended for debugging purposes.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_rect* - reference to the target rectangle
 *   eg_t_res* - a reference to a collision result struct
 *   eg_point* - a reference to an origin point
 *   eg_point* - a reference to a direction vector
 */
static void draw_collision(
    eg_app *app,
    eg_rect *r,
    eg_ray_res *res,
    eg_point *p,
    eg_point *d)
{
    // Draw the target rectangle in red.
    // The dimensions should be the width and height of the target entity
    // plus the width and height of the source entity.
    SDL_Rect target_bounds = {
        .x = r->x,
        .y = r->y,
        .w = r->w,
        .h = r->h};
    SDL_SetRenderDrawColor(app->renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(app->renderer, &target_bounds);

    // Draw the contact point CP in cyan.
    SDL_SetRenderDrawColor(app->renderer, 0, 255, 255, 255);
    SDL_Rect cp_rect = {
        .x = res->cp.x - 5,
        .y = res->cp.y - 5,
        .w = 10,
        .h = 10};
    SDL_RenderFillRect(app->renderer, &cp_rect);

    // Draw the contact normal CN in magenta.
    SDL_SetRenderDrawColor(app->renderer, 255, 0, 255, 255);
    SDL_RenderDrawLine(
        app->renderer,
        res->cp.x,
        res->cp.y,
        res->cp.x + res->cn.x * 20,
        res->cp.y + res->cn.y * 20);

    // Draw the direction vector D in green.
    SDL_SetRenderDrawColor(app->renderer, 0, 255, 100, 255);
    SDL_RenderDrawLine(
        app->renderer,
        p->x,
        p->y,
        p->x + d->x * 10,
        p->y + d->y * 10);
}

/**
 * Determines if a ray intersects a rectangle.
 * It is based on the video at https://www.youtube.com/watch?v=8JJ-4JgR7Dg.
 *
 * This is an implementation of Swept AABB (Axis-Aligned Bounding Box).
 * It assumes that all entities are rectangles that are axis-aligned, meaning
 * their sides are parallel with the x and y axes.
 *
 * Given a origin point P and an ending point Q, the direction vector D can
 * be calculated as Q - P.
 * The position of the target rectangle, RP, is the point of the top left
 * vertex of the rectangle. The size of the rectangle, RS, is a vector whose
 * components are the width and height of the rectangle. The point of the
 * bottom right vertex can therefore be calculated as RP + RS.
 *
 * Any point along the line segment formed by direction vector D can be
 * calculated as follows:
 * P(t) = D * t + P
 * where P(0) = P and P(1) = Q.
 *
 * Intersection Points N and F
 * ---------------------------
 * The intersection points are the points along the line D where the line
 * intersects the sides of the target rectangle. These points are the near
 * point N and the far point F.
 * Near point N is the intersection point closest to origin point P and far
 * point F is the intersection point closes to point Q.
 * For the purposes of this implementation, near point N is considered to be
 * the contact point CP.
 *
 * Given a contact point CP, the contact normal CN is the unit vector
 * orthogonal to the side of the target rectangle
 *
 *
 *     P
 *      \
 *       \
 *        \ N
 *  RP +---*--------------+
 *     |    \             |
 *     |     \            |
 *     |      \           |
 *     |       \ F        |
 *     +--------*---------+ RP + RS
 *               \
 *                \
 *                 Q
 *
 *
 * Perfect Corner Collision
 * ------------------------
 * The source video does not take into account perfect corner collisions where
 * the x and y components of t_near are equal. The caller of this function
 * must, therefore, make a context-dependent decision on how to resolve the
 * collision.
 * For example, if the application is using a top-down view, then a
 * perfect corner collision may be treated as a head on collision with a
 * wall, and both the x and y velocities could be set to 0.
 * Alternatively, if the application is a side-scrolling platformer, the
 * x velocity could be set to 0, and the y velocity would be controlled by
 * gravity.
 *
 * Params:
 *   eg_point* - the origin point of the ray
 *   eg_point* - the direction vector of the ray
 *   eg_rect* - the target rectangle
 *   eg_t_res* - results of intersection detection
 */
static int ray_v_rect(
    eg_point *p,
    eg_point *d,
    eg_rect *r,
    eg_ray_res *res)
{
    if (p == NULL || d == NULL || r == NULL)
    {
        return 0;
    }

    // Direction vector components.
    float dx = (float)d->x;
    float dy = (float)d->y;

    // Flags to detect division by 0.
    // These will be referred to as dz flags.
    int near_x_dz = 0;
    int near_y_dz = 0;
    int far_x_dz = 0;
    int far_y_dz = 0;

    //--------------------------------------------------------------------
    // Detect division by zero.
    // The values stored in the dz flags indicate the sign of the dividend.
    // A dz value of 1 indicates division of a positive dividen by 0, and a
    // dz value of -1 indicates division of a negative dividend by 0.
    // When attempting to divide by 0, if the dividend of the t_far component
    // is 0, then we do not consider a collision to have occurred.
    // TODO: figure out and explain why a far dividend of 0 means no collision.
    if (dx == 0)
    {
        near_x_dz = 1;
        far_x_dz = 1;

        int near_div = r->x - p->x;
        int far_div = r->x + r->w - p->x;

        if (far_div == 0)
        {
            return 0;
        }

        if (near_div < 0)
        {
            near_x_dz = -1;
        }

        if (far_div < 0)
        {
            far_x_dz = -1;
        }
    }

    if (dy == 0)
    {
        near_y_dz = 1;
        far_y_dz = 1;

        int near_div = r->y - p->y;
        int far_div = r->y + r->h - p->y;

        if (far_div == 0)
        {
            return 0;
        }

        if (near_div < 0)
        {
            near_y_dz = -1;
        }

        if (far_div < 0)
        {
            far_y_dz = -1;
        }
    }

    //--------------------------------------------------------------------
    // Calculate the intersection points N and F.
    // The components of near point N are near_x and near_y.
    // The components of far point F are far_x and far_y.
    //
    // In the event of a division by 0, we assign the intersection point
    // component a value of FLT_MAX or -FLT_MAX depending on the sign of the
    // dividend. If the dividend is positive, the component will be FLT_MAX.
    // If the dividend is negative, the component will be -FLT_MAX.
    //
    // When not dividing by 0, the intersection point components can be
    // calculated like so:
    // near_x = (RPx - Px) / Dx
    // far_x = (RPx + RSx - Px) / Dx
    // near_y = (RPy - Py) / Dy
    // far_y = (RPy + RSy - Py) / Dy

    float near_x;
    float near_y;
    float far_x;
    float far_y;

    // near_x
    if (near_x_dz)
    {
        near_x = near_x_dz > 0 ? FLT_MAX : -FLT_MAX;
    }
    else
    {
        near_x = ((float)(r->x) - p->x) / dx;
    }

    // near_y
    if (near_y_dz)
    {
        near_y = near_y_dz > 0 ? FLT_MAX : -FLT_MAX;
    }
    else
    {
        near_y = ((float)(r->y) - p->y) / dy;
    }

    // far_x
    if (far_x_dz)
    {
        far_x = far_x_dz > 0 ? FLT_MAX : -FLT_MAX;
    }
    else
    {
        far_x = ((float)(r->x) + r->w - p->x) / dx;
    }

    // far_y
    if (far_y_dz)
    {
        far_y = far_y_dz > 0 ? FLT_MAX : -FLT_MAX;
    }
    else
    {
        far_y = ((float)(r->y) + r->h - p->y) / dy;
    }

    //--------------------------------------------------------------------
    // Verify the intersection points N and F.
    // In order for the first collision check to work, we need to ensure that
    // N < F. We can do this by swapping the values of the near and far
    // components if the near components are greater than the far components.
    // This accounts for negative values, allowing us to detect collisions in
    // all directions.

    // Ensure that near_x < far_x
    if (near_x > far_x)
    {
        float tmp = near_x;
        near_x = far_x;
        far_x = tmp;
    }

    // Ensure that near_y < far_y
    if (near_y > far_y)
    {
        float tmp = near_y;
        near_y = far_y;
        far_y = tmp;
    }

    // Perform the first collision check.
    // For a collision to occur, the following conditions must be met:
    // near_x < far_y and near_y < far_x.
    // So if near_x > far_y or near_y > far_x, then a collision has not
    // occurred.
    if (near_x > far_y || near_y > far_x)
    {
        return 0;
    }

    //--------------------------------------------------------------------
    // Calculate the t values used to calculate the intersection points N and
    // F. Using the function P(t) = D * t + P we want to find the values
    // t_near and t_far such that P(t_near) = N and P(t_far) = F.

    // t_near is be the maximum of the set { near_x, near_y }.
    float t_near = near_x >= near_y ? near_x : near_y;

    // t_far is be the minimum of the set { far_x, far_y }.
    float t_far = far_x < far_y ? far_x : far_y;

    //--------------------------------------------------------------------
    // Check the t values to verify that the intersection points N and F
    // exist in appropriate locations relative to P and Q.

    // Perform the second collision check.
    // A negative t_far value indicates that the contact point is behind the
    // point P. We do not consider this to be a valid collision.
    //
    //    Q     P  +---------+
    //    *<----*  | target  |
    //             +---------+
    if (t_far < 0)
    {
        return 0;
    }

    // Perform the third collision check.
    // A t_near value of >= 1 indicates that the contact point is at or after
    // point Q. We also do not consider this to be a valid collision.
    //
    //    P     Q  +---------+
    //    *---->*  | target  |
    //             +---------+
    if (t_near >= 1)
    {
        return 0;
    }

    // if (near_x_dz || near_y_dz || far_x_dz || far_y_dz)
    // {
    //     printf("dz: %d, %d, %d, %d t_near: %.4f, t_far: %.4f\n",
    //            near_x_dz,
    //            near_y_dz,
    //            far_x_dz,
    //            far_y_dz,
    //            t_near,
    //            t_far);
    // }

    //--------------------------------------------------------------------
    // Calculate and set the output values.
    // We want to determine three things:
    // 1. A t value for which P(t) = CP
    // 2. The contact point CP
    // 3. The contact normal CN

    // The t value has already been calculated, so we just set it here.
    res->t = t_near;

    // Calculate the contact point CP.
    // P(t) = D * t + P
    res->cp.x = (int)(dx * t_near + p->x);
    res->cp.y = (int)(dy * t_near + p->y);

    // Determine the contact normal CN.
    // Note that if near_x == near_y, then we have a perfect corner collision
    // and the contact normal will not be set.
    if (near_x > near_y)
    {
        // If near_x > near_y and Dx < 0, then CN is (-1, 0)
        // If near_x > near_y and Dx > 0, then CN is (1, 0)
        if (dx < 0)
        {
            res->cn.x = 1;
            res->cn.y = 0;
        }
        else
        {
            res->cn.x = -1;
            res->cn.y = 0;
        }
    }
    else if (near_x < near_y)
    {
        // If near_x < near_y and Dy < 0, then CN is (0, 1)
        // If near_x < near_y and Dy > 0, then CN is (0, -1)
        if (dy < 0)
        {
            res->cn.x = 0;
            res->cn.y = 1;
        }
        else
        {
            res->cn.x = 0;
            res->cn.y = -1;
        }
    }
    else
    {
        // If we reached this point, we have a perfect corner collision.
        // In this case, the contact normal is not a true normal.
        // We set both x and y to 1 or -1 so that the caller of this
        // function can detect corner collisions and handle them.
        res->cn.x = dx < 0 ? 1 : -1;
        res->cn.y = dy < 0 ? 1 : -1;
    }

    // printf("%2d %2d %2d %2d ", near_x_dz, near_y_dz, far_x_dz, far_y_dz);

    // printf("P: (%d, %d) D: (%d, %d) RP: (%d, %d) RP+RS: (%d, %d) t_near: (%.4f) t_far: (%.4f)\n",
    //        p->x,
    //        p->y,
    //        d->x,
    //        d->y,
    //        r->x,
    //        r->y,
    //        r->x + r->w,
    //        r->h + r->h,
    //        t_near,
    //        t_far);

    return 1;
}

int eg_check_past_col(
    eg_app *app,
    eg_entity *a,
    eg_entity *b,
    eg_ray_res *res)
{
    eg_entity *source; // source entity A
    eg_entity *target; // target entity B
    eg_point p;        // origin point P
    eg_point d;        // direction vector D
    eg_rect r;         // target rectangle R

    if (app == NULL || a == NULL || b == NULL || res == NULL)
    {
        return 0;
    }

    // Check for velocity.
    // This function is only intended for a collision scenario in which at
    // least one of the entities has a velocity.
    // Two entities may be overlapping, and that may have some effect
    // elsewhere, but for the purposes of ray casting, it not considered a
    // collision.
    if (a->x_vel == 0 && a->y_vel == 0 && b->x_vel == 0 && b->y_vel == 0)
    {
        return 0;
    }

    source = a;
    target = b;

    // If target entity B has velocity but source entity A does not, then
    // we swap the source and the target. This is just switching pointers
    // in the scope of this function. The actual memory that is referenced
    // remains unchanged.
    if (a->x_vel == 0 && a->y_vel == 0 && (b->x_vel != 0 || b->y_vel != 0))
    {
        source = b;
        target = a;
    }

    // Get the width and height of source entity A.
    int aw = app->registry[source->id].width;
    int ah = app->registry[source->id].height;

    // Create a rectangle representing the boundaries of target entity B.
    // Since the origin point of the ray
    r.x = target->x_pos - (aw / 2);
    r.y = target->y_pos - (ah / 2);
    r.w = app->registry[target->id].width + aw;
    r.h = app->registry[target->id].height + ah;

    // The origin point P is the center point of source entity A.
    p.x = source->x_pos + aw / 2;
    p.y = source->y_pos + ah / 2;

    // The direction vector D is the velocity of source entity A.
    d.x = source->x_vel;
    d.y = source->y_vel;

    if (ray_v_rect(&p, &d, &r, res))
    {
        // printf("P: (%d, %d), RP: (%d, %d), RP+RS: (%d, %d) D: (%d, %d)\n",
        //        p.x,
        //        p.y,
        //        r.x,
        //        r.y,
        //        r.x + r.w,
        //        r.y + r.h,
        //        d.x,
        //        d.y);

        // Draw the collision event information.
        draw_collision(app, &r, res, &p, &d);

        return 1;
    }

    return 0;
}

int is_overlapped(
    eg_rect *a,
    eg_rect *b,
    eg_overlap *res)
{
    // Get the width and height of both entities.
    int aw = a->w;
    int ah = a->h;
    int bw = b->w;
    int bh = b->h;

    // Get the x and y position of both entities.
    // These will be the position of the left and right edges.
    int a_left = a->x;
    int a_top = a->y;
    int b_left = b->x;
    int b_top = b->y;

    // Get the positions of the right and bottom edges.
    int a_right = a_left + aw;
    int a_bottom = a_top + ah;
    int b_right = b_left + bw;
    int b_bottom = b_top + bh;

    // Clear the result data.
    // dx0 is difference between the right side of A and the left side of B.
    // dx1 is difference between the right side of B and the left side of A.
    // dy0 is difference between the bottom side of A and the top side of B.
    // dy1 is difference between the bottom side of B and the top side of A.
    res->dx0 = 0;
    res->dx1 = 0;
    res->dy0 = 0;
    res->dy1 = 0;

    int overlap_x = 0;
    int overlap_y = 0;

    // Check if the left edge of A is in between the left and right edges
    // of b.
    if (a_left > b_left && a_left < b_right)
    {
        res->dx0 = a_right - b_left;
        res->dx1 = b_right - a_left;
        overlap_x = 1;
    }

    // Check if the right edge of a is in between the left and right edges
    // of B.
    if (a_right > b_left && a_right < b_right)
    {
        res->dx0 = a_right - b_left;
        res->dx1 = b_right - a_left;
        overlap_x = 1;
    }

    // Check if B is inside of A.
    if (b_left >= a_left && b_left <= a_right &&
        b_right >= a_left && b_right <= a_right)
    {
        overlap_x = 1;
    }

    // Check if the top edge of A is in between the top and bottom edges
    // of B.
    if (a_top > b_top && a_top < b_bottom)
    {
        res->dy0 = a_bottom - b_top;
        res->dy1 = b_bottom - a_top;
        overlap_y = 1;
    }

    // Check if the bottom edge of A is in between the top and bottom
    // edges of B.
    if (a_bottom > b_top && a_bottom < b_bottom)
    {
        res->dy0 = a_bottom - b_top;
        res->dy1 = b_bottom - a_top;
        overlap_y = 1;
    }

    // Check if B is inside of A.
    if (b_top >= a_top && b_top <= a_bottom &&
        b_bottom >= a_top && b_bottom <= a_bottom)
    {
        overlap_y = 1;
    }

    // If the boundaries of the two entities overlap in both directions, then
    // a there is a collision.
    if (overlap_x && overlap_y)
    {
        return 1;
    }

    return 0;
}
