#include "demo/collision/collision.h"
#include <stdio.h>

// Directions.
// These determine which way we traverse a linked list using previous and
// next pointers.
#define DIR_BACKWARD 0
#define DIR_FORWARD 1

static void detect_collisions(
    eg_app *app,
    eg_entity *source,
    collision_list **cols,
    int direction)
{
    // If direction has a non zero value, then we check every entity after
    // the source. Otherwise we check every entity before the source.
    eg_entity *target = direction ? source->next : source->previous;

    while (target != NULL)
    {
        eg_ray_res res;

        // Use swept AABB to determine if the two entities will collide.
        if (eg_swept_aabb(app, source, target, &res))
        {
            // Add the collision result to the array.
            eg_col_res col_res = {.col = res, .target = target};
            col_push_result(*cols, &col_res);
        }

        target = direction ? target->next : target->previous;
    }
}

/**
 * A comparator function for sorting a list of collision results.
 *
 * Params:
 *   eg_col_res* - a reference to a collision result
 *   eg_col_res* - a reference to another collision result
 */
static int greater(eg_col_res *a, eg_col_res *b)
{
    float t0;     // t value of entity a
    float t1;     // t value of entity b
    eg_point cn0; // contact normal of entity a
    eg_point cn1; // contact normal of entity b
    int corner;   // corner collision flag

    t0 = a->col.t;
    t1 = b->col.t;
    cn0 = a->col.cn;
    cn1 = b->col.cn;
    corner = 0;

    // Check the contact normals.
    // If both the x and y components of a contact normal
    // are non zero, then it is considered "greater" than
    // a true contact normal.
    if (cn0.x && cn0.y && (!cn1.x || !cn1.y))
    {
        corner = 1;
    }

    // Check if we have two possible perfect corner collisions
    // for one source. Hopefully, this never happens.
    // It should only be possible if two targets overlap in
    // just the right way.
    if (cn0.x && cn0.y && cn1.x && cn1.y)
    {
        printf("[WARN] two corner collisions for one source\n");
    }

    // The t value takes priority.
    // We only check the corner collision into account if
    // both t values are equal.
    if (t0 > t1 || (t0 == t1 && corner))
    {
        return 1;
    }

    return 0;
}

void handle_collisions(eg_app *app)
{
    // The collision detection has three stages:
    // Stage 1: Collision Detection
    // Stage 2: Collision Sorting
    // Stage 3: Collision Resolution

    collision_list *cols; // collision result list
    eg_entity *source;    // source entity

    // Create the collision result list.
    // This will be reused on each iteration of the main collision loop.
    cols = col_create_list();
    if (cols == NULL)
    {
        app->done = 1;
        return;
    }

    // main collision loop
    source = app->entities;
    while (source != NULL)
    {
        // Stage 1: Collision Detection
        // We traverse the entity list forwards and backwards from the source
        // entity. This prevents checking an entity for collision with itself.
        detect_collisions(app, source, &cols, DIR_FORWARD);
        detect_collisions(app, source, &cols, DIR_BACKWARD);

        // Stage 2: Collision Sorting
        int sorted = 0;
        while (!sorted)
        {
            sorted = 1;
            for (int i = 0; i < cols->count; i++)
            {
                eg_col_res tmp;
                if (i < cols->count - 1)
                {
                    if (greater(&(cols->data[i]), &(cols->data[i + 1])))
                    {
                        sorted = 0;
                        tmp = cols->data[i];
                        cols->data[i] = cols->data[i + 1];
                        cols->data[i + 1] = tmp;
                    }
                }
            }
        }

        // Stage 3: Collision Resolution
        for (int i = 0; i < cols->count; i++)
        {
            eg_entity *a;     // source entity A
            eg_entity *b;     // target entity B
            eg_ray_res col;   // swept AABB result
            eg_collider cola; // source collision callback
            eg_collider colb; // target collision callback
            // eg_overlap;       // overlap for collision verification.
            // eg_rect ra;
            // eg_rect rb;

            a = source;
            b = cols->data[i].target;

            if (eg_swept_aabb(app, a, b, &col))
            {
                // Verify the collision.


                // Call the source entity's collision function.
                cola = app->registry[a->id].collide;
                if (cola != NULL)
                {
                    cola(app, a, b, &col, 0);
                }

                // Call the target entity's collision function.
                colb = app->registry[b->id].collide;
                if (colb != NULL)
                {
                    colb(app, b, a, &col, 1);
                }
            }
        }

        // Clear the collision list.
        col_clear_list(cols);

        source = source->next;

    } // END main collision detection loop

    // Destroy the collision list.
    col_destroy_list(cols);
}
