#include "collision.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

void eg_div(int dividend, int divisor, eg_quot *result)
{
    if (result == NULL)
    {
        return;
    }

    // Check for attempts to divide by zero.
    if (divisor == 0)
    {
        result->quotient = 0;
        result->remainder = 0;
        if (dividend < 0)
        {
            result->zero = -1;
        }
        else
        {
            result->zero = 1;
        }

        return;
    }

    // Clear the zero flag.
    result->zero = 0;

    // Calculate the quotient and the remainder.
    result->quotient = dividend / divisor;
    result->remainder = dividend % divisor;
}

int eg_divcmp(eg_quot *a, eg_quot *b)
{
    // Check for NULL pointers.
    // For the purposes of comparing division results, we consider a NULL
    // pointer to be less than a non NULL pointer.
    if (a == NULL && b != NULL)
    {
        return -1;
    }

    if (a == NULL && b == NULL)
    {
        return 0;
    }

    if (a != NULL && b == NULL)
    {
        return 1;
    }

    // Check for zero flags.
    if (a->zero == -1 && b->zero > -1)
    {
        return -1;
    }

    if (a->zero == 1 && b->zero < 1)
    {
        return 1;
    }

    if (a->zero && a->zero == b->zero)
    {
        return 0;
    }

    // Compare the quotients.
    if (a->quotient < b->quotient)
    {
        return -1;
    }

    if (a->quotient > b->quotient)
    {
        return 1;
    }

    // Compare the remainders.
    if (a->remainder < b->remainder)
    {
        return -1;
    }

    if (a->remainder == b->remainder)
    {
        return 0;
    }

    if (a->remainder > b->remainder)
    {
        return 1;
    }

    // This should be unreachable.
    return 0;
}

void eg_divswap(eg_quot *a, eg_quot *b)
{
    if (a == NULL || b == NULL)
    {
        return;
    }

    // Store the contents of a.
    eg_quot tmp = {
        .quotient = a->quotient,
        .remainder = a->remainder,
        .zero = a->zero};

    // Put the contents of b in a.
    a->quotient = b->quotient;
    a->remainder = b->remainder;
    a->zero = b->zero;

    // Put the contents of a in b.
    b->quotient = tmp.quotient;
    b->remainder = tmp.remainder;
    b->zero = tmp.zero;
}

void eg_divsort(eg_quot q[], int len, int (*cmp)(eg_quot *, eg_quot *))
{
    int sorted = 0;
    while (!sorted)
    {
        sorted = 1;
        for (int i = 0; i < len - 1; i++)
        {
            eg_quot tmp;
            if (cmp(&(q[i]), &(q[i + 1])) > 0)
            {
                sorted = 0;
                tmp = q[i + 1];
                q[i + 1] = q[i];
                q[i] = tmp;
            }
        }
    }
}

// The following is based on the collision detection and resolution video
// at https://www.youtube.com/watch?v=8JJ-4JgR7Dg
//
//
/*

Given starting point P and ending point Q, D is the line connecting those
two points. For the line D, any point along the line can be represented as
P(t) = P + D * t where P(0) = P and P(1) = Q.

For a given rectangle R, the point of origin of the rectangle is the top left
vertex, which is denoted as RP. The size of this rectangle (width and height)
is denoted as RS. So the bottom right vertex of the rectangle is RS + RP.

In the following figure, the line D originates at point P and connects to
point Q. The points P(t0) and P(t1) represent the points along the line where
the line intersects with the axis of the left and right sides of the rectangle
respectively.

        RP
        +---------------+
    P   |               |
    +   |               |
        |               |
        |               |
        |               |   Q
        |               |   +
        +---------------+
                        RS + RP

near_x = (RPx - Px) / Dx
far_x = (RPx + RSx - Px) / Dx
near_y = (RPy - Py) / Dy
far_y = (RPy + RSy - Py) / Dy

For a collision to have occurred, near_x < far_y and near_y < far_x.

t_near = max(near_x, near_y)
t_far = min(far_x, far_y)


Note:
To account for lines with a negative component, the near and far points
must be sorted, such that near < far.



*/

/**
 * Determines if a ray intersects a rectangle.
 *
 * Params:
 *   eg_point* - the origin point of the ray
 *   eg_point* - the direction vector of the ray
 *   eg_rect* - the rectangle
 *   float* - distance to intersection
 *   eg_point* - contact point
 *   eg_point* - contact normal
 */
int eg_ray_v_rect(
    eg_point *p,
    eg_point *d,
    eg_rect *r,
    float *t_near,
    eg_point *c_point,
    eg_point *c_normal)
{
    if (p == NULL || d == NULL || r == NULL)
    {
        return 0;
    }

    float near_x;
    float near_y;
    float far_x;
    float far_y;

    // near_x = (RPx - Px) / Dx
    // far_x = (RPx + RSx - Px) / Dx
    // near_y = (RPy - Py) / Dy
    // far_y = (RPy + RSy - Py) / Dy

    float dx = (float)d->x;
    float dy = (float)d->y;

    // Flags to detect division by 0.
    // When we encounter a division by 0, we assume the result is a value of
    // negative or positive infinity based on the sign of the dividend.
    int near_x_inf = 0;
    int near_y_inf = 0;
    int far_x_inf = 0;
    int far_y_inf = 0;

    // Detect division by zero.
    // TODO: handle this properly.
    if (dx == 0)
    {
        near_x_inf = 1;
        far_x_inf = 1;

        if ((r->p.x - p->x) < 0)
        {
            near_x_inf = -1;
        }

        if ((r->p.x + r->w - p->x) < 0)
        {
            far_x_inf = -1;
        }

        printf("divide by 0 (x axis) near_x: %d*infinity, far_x: %d*infinity\n",
               near_x_inf,
               far_x_inf);
        dx = FLT_MAX;
    }

    if (dy == 0)
    {
        near_y_inf = 1;
        far_y_inf = 1;

        if ((r->p.y - p->y) < 0)
        {
            near_y_inf = -1;
        }

        if ((r->p.y + r->h - p->y) < 0)
        {
            far_y_inf = -1;
        }

        printf("divide by 0 (y axis) near_y: %d*infinity, far_y: %d*infinity\n",
               near_y_inf,
               far_y_inf);
        dy = FLT_MAX;
    }

    near_x = ((float)(r->p.x) - p->x) / dx;
    near_y = ((float)(r->p.y) - p->y) / dy;

    far_x = ((float)(r->p.x) + r->w - p->x) / dx;
    far_y = ((float)(r->p.y) + r->h - p->y) / dy;

    // If the near components are greater than the far components, swap them.
    if (near_x > far_x)
    {
        float tmp = near_x;
        near_x = far_x;
        far_x = tmp;
    }

    if (near_y > far_y)
    {
        float tmp = near_y;
        near_y = far_y;
        far_y = tmp;
    }

    // For a collision to occur, the following conditions must be met:
    // near_x < far_y and near_y < far_x.
    // So if near_x > far_y or near_y > far_x, then a collision has not
    // occurred.
    if (near_x > far_y || near_y > far_x)
    {
        return 0;
    }

    // Points of intersection.
    *t_near = near_x;
    float t_far = far_x;

    // t_near must be the maximum of {near_x, near_y}.
    if (near_x < near_y)
    {
        *t_near = near_y;
    }

    // t_far must be the minimum of {far_x, far_y}.
    if (far_x >= far_y)
    {
        t_far = far_y;
    }

    // If the collision occurred behind the ray direction, then we don't
    // acknowledge that as a collision.
    if (t_far < 0)
    {
        return 0;
    }

    // P(t) = P + D * t

    // Determine direction of normal vector.
    // If near_x > near_y and Dx > 0, then the normal is in the -x direction (left).
    // If near_x < near_y and Dy > 0, then the normal is in the -y direction (up).
    // If near_x > near_y and Dx < 0, then the normal is in the x direction (right).
    // If near_x < near_y and Dy < 0, then the normal is in the y direction (down).

    c_point->x = (int)(p->x + *t_near * dx);
    c_point->y = (int)(p->y + *t_near * dy);

    if (near_x > near_y)
    {
        if (dx < 0)
        {
            c_normal->x = 1;
            c_normal->y = 0;
        }
        else
        {
            c_normal->x = -1;
            c_normal->y = 0;
        }
    }
    else if (near_x < near_y)
    {
        if (dy < 0)
        {
            c_normal->x = 0;
            c_normal->y = 1;
        }
        else
        {
            c_normal->x = 0;
            c_normal->y = -1;
        }
    }

    // Since a t value of 1 represents the endpoint point Q,
    // a collision can only occur if t_near < 1.
    if (*t_near >= 1)
    {
        return 0;
    }

    return 1;
}
