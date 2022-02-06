#ifndef TEMP_EG_COLLISION_H
#define TEMP_EG_COLLISION_H

// This is a temporary interface for collision detection.
// In the future, it may be kept in its own file, or it may be incorporated
// into the core source file.

// A Quotient represents the result of a division operation where the two
// operands are integers.
typedef struct eg_quot
{
    int quotient;
    int remainder;
    int zero; // used to detect division by zero.
} eg_quot;

// A point represents the x and y coordinates in 2D space.
typedef struct eg_point
{
    int x;
    int y;
} eg_point;

// A rectangle.
typedef struct eg_rect
{
    eg_point p; // location of top left corner
    int w;
    int h;
} eg_rect;

typedef int (*eg_quot_comparator)(eg_quot *, eg_quot *);

/**
 * Divides two integers.
 * The first argument is the dividend, and it will be divided by the second
 * argument, which is the divisor. The result will be stored in the struct
 * referenced by the third argument, which is a pointer to an eg_quot struct.
 *
 * If the third argument is NULL, this function returns without performing any
 * operation.
 *
 * If the divisor is 0, then the z flag of the eg_quot struct is set to 1 or
 * -1, and the quotient and remainder values are set to 0. The sign of the
 * zero flag is the sign of the dividend. So dividing 3 by 0 will result in a
 * zero flag of 1, and dividing -3 by 0 will result in a zero flag of -1.
 *
 * Params:
 *   int - the dividend, the number that will be divided
 *   int - the divisor, the number by which the dividend will eb divided
 *   eg_quot* - a pointer to the struct that will receive the result.
 */
void eg_div(int dividend, int divisor, eg_quot *result);

/**
 * Compares the contents of two eg_quot structs.
 * This function takes as its arguments two references to eg_quot structs, a
 * and b. The value returned is an int with a value of -1, 0, or 1.
 * If a < b, then -1 is returned.
 * If a == b, then 0 is returned.
 * If a > b, then 1 is returned.
 *
 * For the purposes of comparison, a NULL pointer is considered less than a
 * non NULL pointer. Two NULL pointers are considered to be equal.
 *
 * An eg_quot whose zero flag is set is considered to be infinitely greater
 * than an eg_quot whose zero flag is not set. If two eq_quot structs have
 * their zero flags set, they are considered to be equal.
 *
 * Params:
 *   eg_quot* - a reference to the first division result
 *   eg_quot* - a reference to the second division result
 *
 * Returns:
 *   int - 1 if a > b, 0 if a == b, or -1 if a < b.
 */
int eg_divcmp(eg_quot *a, eg_quot *b);

/**
 * Swaps the contents of two eg_quot structs.
 *
 * Params:
 *   eg_quot* - a reference to an eg_quot
 *   eg_quot* - a reference to an eg_quot
 */
void eg_divswap(eg_quot *a, eg_quot *b);

/**
 * Sorts an array of eg_quot structs.
 * This uses bubble sort, so it is not intended to be very performant, but
 * it's good enough for now.
 *
 * This function sorts the array into ascending order. For two eg_quot structs
 * a and b, if a < b, then a will be before b in the sorted array. What
 * constitues "less than" is decided by the comparator function.
 *
 * Params:
 *   eg_quot[] - pointer to the first element of an array of eg_quot
 *   int - the length of the array
 *   eg_quot_comparator - comparator function.
 */
void eg_divsort(eg_quot q[], int len, eg_quot_comparator);

/**
 * Determines if a ray intersects a rectangle.
 *
 * Params:
 *   eg_point* - the origin point of the ray
 *   eg_point* - the direction vector of the ray
 *   eg_rect* - the rectangle
 *   eg_quot* - distance to intersection
 *   eg_point* - contact point
 *   eg_point* - contact normal
 */
int eg_ray_v_rect(
    eg_point *p,
    eg_point *d,
    eg_rect *r,
    float *t_near,
    eg_point *c_point,
    eg_point *c_normal);

#endif