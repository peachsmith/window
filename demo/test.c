#include "example.h"
#include "demo/entities/entity_types.h"
#include "demo/collision/collision.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    // eg_initialize();

    // [DEBUG] RP: (110, 14) RP+RS: (142, 58), D: (2, -11) P: (110, 50)
    // [DEBUG] RP: (110, 14) RP+RS: (142, 58), D: (-2, -11) P: (110, 50)

    // Scenario:
    // The source entity is to the left of the target.
    // The source entity's right side is flush with the target entity's
    // left side.

    eg_app app;
    eg_entity source;
    eg_entity target;
    app.cam.x = 0;
    app.cam.y = 0;
    eg_entity_type reg[ENTITY_TYPE_MAX];
    eg_ray_res res;

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
    app.registry = reg;

    // P calculation
    // p.x = a->x_pos + aw / 2;
    // p.y = a->y_pos + ah / 2;

    source.id = ENTITY_TYPE_PLAYER;
    source.x_pos = 100;
    source.y_pos = 34;

    target.id = ENTITY_TYPE_BLOCK;
    target.x_pos = 120;
    target.y_pos = 30;
    target.x_vel = 0;
    target.y_vel = 0;

    int test_count = 0;
    int col_count = 0;

    // For any positive x velocity,
    // all y velocities SHOULD result in a collision.
    // We will test y velocities from -50 to 50.
    source.x_vel = 1;
    for (int i = 0; i <= 100; i++)
    {
        test_count++;
        source.y_vel = -50 + i;
        if (eg_swept_aabb(&app, &source, &target, &res))
        {
            col_count++;
        }
    }

    printf("[TEST] EXPECTED %d collisions, found %d collisions\n",
           test_count,
           col_count);

    test_count = 0;
    col_count = 0;

    // For any negative x velocity,
    // all y velocities should NOT result in a collision.
    // We will test y velocities from -50 to 50.
    // for (int j = 1; j < 100; j++)
    // {
    //     test_count = 0;
    //     col_count = 0;
    source.x_vel = -3;
    for (int i = 0; i <= 100; i++)
    {
        test_count++;
        source.y_vel = -50 + i;
        if (eg_swept_aabb(&app, &source, &target, &res))
        {
            col_count++;
            // printf("[ERROR] D: (%d, %d)\n",
            //        source.x_vel,
            //        source.y_vel);
        }
    }
    printf("[TEST] EXPECTED 0 collisions, found %d collisions for D: (%d, %d)\n",
           col_count, source.x_vel, source.y_vel);
    // }

    // eg_terminate();

    return 0;
}
