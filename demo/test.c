#include "example.h"
#include "demo/entities/entity_types.h"
#include "demo/collision/collision.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    // Scenarios:
    // 1. The source entity is to the left of the target, moving right.
    // 2. The source entity is to the left of the target, moving left.
    // 3. The source entity is to the right of the target, moving left.
    // 4. The source entity is to the right of the target, moving right.
    // 5. The source entity is above the target, moving down.
    // 6. The source entity is above the target, moving up.
    // 7. The source entity is below the target, moving up.
    // 8. The source entity is below the target, moving down.

    eg_app app;
    eg_entity source;
    eg_entity target;
    app.cam.x = 0;
    app.cam.y = 0;
    eg_entity_type reg[ENTITY_TYPE_MAX];
    eg_collision res;

    // Register the source entity
    reg[ENTITY_TYPE_PLAYER].id = ENTITY_TYPE_PLAYER;
    reg[ENTITY_TYPE_PLAYER].width = 20;
    reg[ENTITY_TYPE_PLAYER].height = 32;
    reg[ENTITY_TYPE_PLAYER].render = NULL;
    reg[ENTITY_TYPE_PLAYER].update = NULL;
    reg[ENTITY_TYPE_PLAYER].collide = NULL;

    // Register the target entity
    reg[ENTITY_TYPE_BLOCK].id = ENTITY_TYPE_BLOCK;
    reg[ENTITY_TYPE_BLOCK].width = 12;
    reg[ENTITY_TYPE_BLOCK].height = 12;
    reg[ENTITY_TYPE_BLOCK].render = NULL;
    reg[ENTITY_TYPE_BLOCK].update = NULL;
    reg[ENTITY_TYPE_BLOCK].collide = NULL;
    app.entity_types = reg;

    // P calculation
    // p.x = a->x_pos + aw / 2;
    // p.y = a->y_pos + ah / 2;

    source.type = ENTITY_TYPE_PLAYER;
    source.x_pos = 0;
    source.y_pos = 34;
    source.carrier = NULL;

    target.type = ENTITY_TYPE_BLOCK;
    target.x_pos = 120;
    target.y_pos = 30;
    target.x_vel = 0;
    target.y_vel = 0;
    target.carrier = NULL;

    int test_count = 0;
    int col_count = 0;

    //------------------------------------------------------
    // BEGIN Scenario 1
    // The source entity is to the left of the target, moving right.

    // Set x position of source to be to the left of the target.
    //                            Rx >= Px
    // target_pos - source_width / 2 >= source_pos + source_width / 2
    // 120        - 10               >= source_pos + 10
    //                           110 >= source_pos + 10
    //                           100 >= source_pos
    source.x_pos = 100;

    // For any positive x velocity,
    // all y velocities SHOULD result in a collision.
    // We will test y velocities from -50 to 50.
    source.x_vel = 1;
    for (int i = 0; i <= 100; i++)
    {
        test_count++;
        source.y_vel = -50 + i;
        if (demo_swept_aabb(&app, &source, &target, &res))
        {
            col_count++;
        }
    }

    if (test_count == col_count)
    {
        printf("[PASS] EXPECTED %d collisions, found %d collisions\n",
               test_count,
               col_count);
    }
    else
    {
        printf("[FAIL] EXPECTED %d collisions, found %d collisions\n",
               test_count,
               col_count);
    }

    // Reset test counters.
    test_count = 0;
    col_count = 0;

    //------------------------------------------------------
    // BEGIN Scenario 2
    // The source entity is to the left of the target, moving left.

    // For any negative x velocity,
    // all y velocities should NOT result in a collision.
    // We will test y velocities from -50 to 50.
    source.x_vel = -2;
    for (int i = 0; i <= 100; i++)
    {
        test_count++;
        source.y_vel = -50 + i;
        if (demo_swept_aabb(&app, &source, &target, &res))
        {
            col_count++;
        }
    }

    if (col_count == 0)
    {
        printf("[PASS] EXPECTED 0 collisions, found %d collisions for D: (%d, %d)\n",
               col_count, source.x_vel, source.y_vel);
    }
    else
    {
        printf("[FAIL] EXPECTED 0 collisions, found %d collisions for D: (%d, %d)\n",
               col_count, source.x_vel, source.y_vel);
    }

    // Reset test counters.
    test_count = 0;
    col_count = 0;

    //------------------------------------------------------
    // BEGIN Scenario 3
    // The source entity is to the right of the target, moving left.

    // Set x position of source to be to the right of the target.
    //                                                     Rx + Rw <= Px
    // target_pos - source_width / 2 + source_width + target_width <= source_pos + source_width / 2
    // 120        - 10               + 20           + 12           <= source_pos + 10
    //                                                         142 <= source_pos + 10
    //                                                         132 <= source_pos
    source.x_pos = 132;

    // For any negative x velocity,
    // all y velocities SHOULD result in a collision.
    // We will test y velocities from -50 to 50.
    source.x_vel = -2;
    for (int i = 0; i <= 100; i++)
    {
        test_count++;
        source.y_vel = -50 + i;
        if (demo_swept_aabb(&app, &source, &target, &res))
        {
            col_count++;
        }
    }

    if (test_count == col_count)
    {
        printf("[PASS] EXPECTED %d collisions, found %d collisions\n",
               test_count,
               col_count);
    }
    else
    {
        printf("[FAIL] EXPECTED %d collisions, found %d collisions\n",
               test_count,
               col_count);
    }

    // Reset test counters.
    test_count = 0;
    col_count = 0;

    //------------------------------------------------------
    // BEGIN Scenario 4
    // The source entity is to the right of the target, moving right.

    // For any positive x velocity,
    // all y velocities should NOT result in a collision.
    // We will test y velocities from -50 to 50.
    source.x_vel = 2;
    for (int i = 0; i <= 100; i++)
    {
        test_count++;
        source.y_vel = -50 + i;
        if (demo_swept_aabb(&app, &source, &target, &res))
        {
            col_count++;
        }
    }

    if (col_count == 0)
    {
        printf("[PASS] EXPECTED 0 collisions, found %d collisions for D: (%d, %d)\n",
               col_count, source.x_vel, source.y_vel);
    }
    else
    {
        printf("[FAIL] EXPECTED 0 collisions, found %d collisions for D: (%d, %d)\n",
               col_count, source.x_vel, source.y_vel);
    }

    // Reset test counters.
    test_count = 0;
    col_count = 0;

    //------------------------------------------------------
    // BEGIN Scenario 5
    // The source entity is above the target, moving down.

    // Set the source x position to match the target x position.
    source.x_pos = target.x_pos;

    // Set y position of source to be to above of the target.
    //                             Ry <= Py
    // target_pos - source_height / 2 <= source_pos + source_height / 2
    // 30         - 16                <= source_pos + 16
    //                             14 <= source_pos + 16
    //                             -2 <= source_pos
    source.y_pos = -2;

    // For any positive y velocity,
    // all x velocities SHOULD result in a collision.
    // We will test x velocities from -50 to 50.
    source.y_vel = 2;
    for (int i = 0; i <= 100; i++)
    {
        test_count++;
        source.x_vel = -50 + i;
        if (demo_swept_aabb(&app, &source, &target, &res))
        {
            col_count++;
        }
    }

    if (test_count == col_count)
    {
        printf("[PASS] EXPECTED %d collisions, found %d collisions\n",
               test_count,
               col_count);
    }
    else
    {
        printf("[FAIL] EXPECTED %d collisions, found %d collisions\n",
               test_count,
               col_count);
    }

    // Reset test counters.
    test_count = 0;
    col_count = 0;

    //------------------------------------------------------
    // BEGIN Scenario 6
    // The source entity is above the target, moving up.

    // For any negative y velocity,
    // all x velocities should NOT result in a collision.
    // We will test x velocities from -50 to 50.
    source.y_vel = -2;
    for (int i = 0; i <= 100; i++)
    {
        test_count++;
        source.x_vel = -50 + i;
        if (demo_swept_aabb(&app, &source, &target, &res))
        {
            col_count++;
        }
    }

    if (col_count == 0)
    {
        printf("[PASS] EXPECTED 0 collisions, found %d collisions for D: (%d, %d)\n",
               col_count, source.x_vel, source.y_vel);
    }
    else
    {
        printf("[FAIL] EXPECTED 0 collisions, found %d collisions for D: (%d, %d)\n",
               col_count, source.x_vel, source.y_vel);
    }

    // Reset test counters.
    test_count = 0;
    col_count = 0;

    //------------------------------------------------------
    // BEGIN Scenario 7
    // The source entity is below the target, moving up.

    // Set y position of source to be below the target.
    //                                                     Ry + Rh <= Py
    // target_pos - source_height / 2 + source_height + target_height <= source_pos + source_heigh / 2
    // 30         - 16                + 32            + 12            <= source_pos + 16
    //                                                             58 <= source_pos + 16
    //                                                             42 <= source_pos
    source.y_pos = 42;

    // For any negative y velocity,
    // all x velocities SHOULD result in a collision.
    // We will test x velocities from -50 to 50.
    source.y_vel = -2;
    for (int i = 0; i <= 100; i++)
    {
        test_count++;
        source.x_vel = -50 + i;
        if (demo_swept_aabb(&app, &source, &target, &res))
        {
            col_count++;
        }
    }

    if (test_count == col_count)
    {
        printf("[PASS] EXPECTED %d collisions, found %d collisions\n",
               test_count,
               col_count);
    }
    else
    {
        printf("[FAIL] EXPECTED %d collisions, found %d collisions\n",
               test_count,
               col_count);
    }

    // Reset test counters.
    test_count = 0;
    col_count = 0;

    //------------------------------------------------------
    // BEGIN Scenario 8
    // The source entity is below the target, moving down.

    // For any positive y velocity,
    // all x velocities should NOT result in a collision.
    // We will test x velocities from -50 to 50.
    source.y_vel = 2;
    for (int i = 0; i <= 100; i++)
    {
        test_count++;
        source.x_vel = -50 + i;
        if (demo_swept_aabb(&app, &source, &target, &res))
        {
            col_count++;
        }
    }

    if (col_count == 0)
    {
        printf("[PASS] EXPECTED 0 collisions, found %d collisions for D: (%d, %d)\n",
               col_count, source.x_vel, source.y_vel);
    }
    else
    {
        printf("[FAIL] EXPECTED 0 collisions, found %d collisions for D: (%d, %d)\n",
               col_count, source.x_vel, source.y_vel);
    }

    return 0;
}
