#include "collision.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

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
int eg_ray_v_rect(
    eg_point *p,
    eg_point *d,
    eg_rect *r,
    eg_t_res *res)
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
    if (dx == 0)
    {
        near_x_dz = 1;
        far_x_dz = 1;

        if ((r->x - p->x) < 0)
        {
            near_x_dz = -1;
        }

        if ((r->x + r->w - p->x) < 0)
        {
            far_x_dz = -1;
        }
    }

    if (dy == 0)
    {
        near_y_dz = 1;
        far_y_dz = 1;

        if ((r->y - p->y) < 0)
        {
            near_y_dz = -1;
        }

        if ((r->y + r->h - p->y) < 0)
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
    // else
    // {
    //     printf("perfect corner collision near_x: %.2f, near_y: %.2f\n", near_x, near_y);
    // }

    return 1;
}

// int eg_check_col(
//     eg_app *app,
//     eg_entity *a,
//     eg_entity *b,
//     eg_t_res *res)
// {
//     if (app == NULL || a == NULL || b == NULL || res == NULL)
//     {
//         return 0;
//     }

//     // Check for velocity.
//     // Currently, this function only works for a moving a and a static b.
//     // TODO: handle collision between two static entities.
//     // TODO: handle collision between two moving entities.
//     if (a->x_vel == 0 && a->y_vel == 0)
//     {
//         return 0;
//     }

//     // width and height of a
//     int aw = app->registry[a->id].width;
//     int ah = app->registry[a->id].height;

//     // Create an expanded target rectangle.
//     eg_rect target = {
//         .x = b->x_pos - (aw / 2),
//         .y = b->y_pos - (ah / 2),
//         .w = app->registry[b->id].width + aw,
//         .h = app->registry[b->id].height + ah};

//     // The origin point P is the center point of a.
//     eg_point p = {
//         .x = a->x_pos + aw / 2,
//         .y = a->y_pos + ah / 2};

//     // The direction vector D is the velocity of a.
//     eg_point d = {.x = a->x_vel, .y = a->y_vel};

//     if (eg_ray_v_rect(&p, &d, &target, res))
//     {
//         // Draw the target rectangle in red.
//         // The dimensions should be the width and height of the target entity
//         // plus the width and height of the source entity.
//         SDL_Rect tmp = {.x = target.x, .y = target.y, .w = target.w, .h = target.h};
//         SDL_SetRenderDrawColor(app->renderer, 255, 0, 0, 255);
//         SDL_RenderDrawRect(app->renderer, &tmp);

//         // Draw the contact point CP in cyan.
//         SDL_SetRenderDrawColor(app->renderer, 0, 255, 255, 255);
//         SDL_Rect c_rect = {.x = res->cp.x - 5, .y = res->cp.y - 5, .w = 10, .h = 10};
//         SDL_RenderFillRect(app->renderer, &c_rect);

//         // Draw the contact normal CN in magenta.
//         SDL_SetRenderDrawColor(app->renderer, 255, 0, 255, 255);
//         SDL_RenderDrawLine(
//             app->renderer,
//             res->cp.x,
//             res->cp.y,
//             res->cp.x + res->cn.x * 20,
//             res->cp.y + res->cn.y * 20);

//         // Draw the direction vector D in orange.
//         SDL_SetRenderDrawColor(app->renderer, 200, 180, 30, 255);
//         SDL_RenderDrawLine(
//             app->renderer,
//             p.x,
//             p.y,
//             d.x,
//             d.y);

//         // Print the collision detection data.
//         // printf("collision D: (%d, %d) CP: (%d, %d) CN: (%d, %d) t: %.2f \n",
//         //        d.x,
//         //        d.y,
//         //        res->cp.x,
//         //        res->cp.y,
//         //        res->cn.x,
//         //        res->cn.y,
//         //        res->t);

//         printf("collision P: (%d, %d) RP: (%d, %d) RS: (%d, %d) source: (%d, %d) t: %.2f\n",
//                p.x,
//                p.y,
//                target.x,
//                target.y,
//                target.w,
//                target.h,
//                aw,
//                ah,
//                res->t);

//         // Adjust the x and y velocities based on the contact normal.
//         // In the case of a platformer, either the x or y velocity will be
//         // adjusted, but probably not both.
//         // int xv = a->x_vel;
//         // int yv = a->y_vel;

//         // if (res->cn.x)
//         // {
//         //     a->x_vel = 0;
//         // }

//         // if (res->cn.y)
//         // {
//         //     a->y_vel = 0;
//         // }
//         // printf("old v: (%d, %d) new v: (%d, %d)\n", xv, yv, a->x_vel, a->y_vel);

//         return 1;
//     }

//     return 0;
// }

int eg_check_past_col(
    eg_app *app,
    eg_entity *a,
    eg_entity *b,
    eg_t_res *res)
{
    if (app == NULL || a == NULL || b == NULL || res == NULL)
    {
        return 0;
    }

    // Check for velocity.
    // Currently, this function only works for a moving a and a static b.
    // TODO: handle collision between two static entities.
    // TODO: handle collision between two moving entities.
    if (a->x_vel == 0 && a->y_vel == 0)
    {
        return 0;
    }

    // width and height of a
    int aw = app->registry[a->id].width;
    int ah = app->registry[a->id].height;

    // Create an expanded target rectangle.
    eg_rect target = {
        .x = b->x_pos - (aw / 2),
        .y = b->y_pos - (ah / 2),
        .w = app->registry[b->id].width + aw,
        .h = app->registry[b->id].height + ah};

    // The origin point P is the center point of a.
    // We subtract the velocity to get the position of the rectangle before
    // the velocity was applied.
    eg_point p = {
        .x = (a->x_pos - a->x_vel) + aw / 2,
        .y = (a->y_pos - a->y_vel) + ah / 2};

    // The direction vector D is the velocity of a.
    // Since the origin point P is the previous position of a before applying
    // velocity, the actual position of a is P + D.
    eg_point d = {.x = a->x_vel, .y = a->y_vel};

    if (eg_ray_v_rect(&p, &d, &target, res))
    {
        // Draw the target rectangle in red.
        // The dimensions should be the width and height of the target entity
        // plus the width and height of the source entity.
        SDL_Rect tmp = {.x = target.x, .y = target.y, .w = target.w, .h = target.h};
        SDL_SetRenderDrawColor(app->renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(app->renderer, &tmp);

        // Draw the contact point CP in cyan.
        SDL_SetRenderDrawColor(app->renderer, 0, 255, 255, 255);
        SDL_Rect c_rect = {.x = res->cp.x - 5, .y = res->cp.y - 5, .w = 10, .h = 10};
        SDL_RenderFillRect(app->renderer, &c_rect);

        // Draw the contact normal CN in magenta.
        SDL_SetRenderDrawColor(app->renderer, 255, 0, 255, 255);
        SDL_RenderDrawLine(
            app->renderer,
            res->cp.x,
            res->cp.y,
            res->cp.x + res->cn.x * 20,
            res->cp.y + res->cn.y * 20);

        // Draw the direction vector D in orange.
        SDL_SetRenderDrawColor(app->renderer, 200, 180, 30, 255);
        SDL_RenderDrawLine(
            app->renderer,
            p.x,
            p.y,
            p.x + d.x * 10,
            p.y + d.y * 10);

        // Print the collision detection data.
        // printf("collision D: (%d, %d) CP: (%d, %d) CN: (%d, %d) t: %.2f \n",
        //        d.x,
        //        d.y,
        //        res->cp.x,
        //        res->cp.y,
        //        res->cn.x,
        //        res->cn.y,
        //        res->t);

        printf("collision P: (%d, %d) D: (%d, %d) RP: (%d, %d) RS: (%d, %d) source: (%d, %d) t: %.2f\n",
               p.x,
               p.y,
               d.x,
               d.y,
               target.x,
               target.y,
               target.w,
               target.h,
               aw,
               ah,
               res->t);

        // Adjust the x and y velocities based on the contact normal.
        // In the case of a platformer, either the x or y velocity will be
        // adjusted, but probably not both.
        // int xv = a->x_vel;
        // int yv = a->y_vel;

        // if (res->cn.x)
        // {
        //     a->x_vel = 0;
        // }

        // if (res->cn.y)
        // {
        //     a->y_vel = 0;
        // }
        // printf("old v: (%d, %d) new v: (%d, %d)\n", xv, yv, a->x_vel, a->y_vel);

        return 1;
    }

    return 0;
}
