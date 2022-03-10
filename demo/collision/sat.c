#include "demo/collision/collision.h"

#include <float.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * An implementation of separated axis theorem (SAT) based on code written by
 * David Barr.
 * Video: https://www.youtube.com/watch?v=7Ik2vowGcU0
 * Source: https://github.com/OneLoneCoder/olcPixelGameEngine/blob/master/Videos/OneLoneCoder_PGE_PolygonCollisions1.cpp
 *
 *
 * Given a source entity A and a target entity B, both of which are arbitrary
 * convex polygons:
 * Create a normal axis on each side of each entity.
 * For each normal axis, project the points at the vertices of each entity
 * onto that axis.
 * If any of the points projected onto the axis from one polygon are in
 * between points projected from another polygon, then there is overlap for
 * that axis.
 * If all axes contain overlap, then the two entities are in collision.
 * If there is at least one axis that does not contain overlap, then the two
 * entities are not in collision.
 */
int demo_sat(
    eg_app *app,
    eg_entity *a,
    eg_entity *b,
    eg_collision *res)
{
    // TODO: implement SAT.

    return 0;
}
