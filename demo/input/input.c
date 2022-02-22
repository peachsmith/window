#include "demo/input/input.h"
#include "demo/entities/player.h"
#include "demo/entities/entity_types.h"

#include <stdio.h>

void root_input_callback(eg_app *app, eg_entity *target)
{
    // left movement
    if (eg_peek_input(app, EG_KEYCODE_LEFT))
    {
        if (target->x_vel >= -2)
        {
            target->x_vel -= 2;
        }
    }

    // right movement
    if (eg_peek_input(app, EG_KEYCODE_RIGHT))
    {
        if (target->x_vel <= 2)
        {
            target->x_vel += 2;
        }
    }

    // down arrow key
    if (eg_peek_input(app, EG_KEYCODE_DOWN))
    {
        if (!eg_check_flag(target, ENTITY_FLAG_DOWN))
        {
            eg_set_flag(target, ENTITY_FLAG_DOWN);
        }
    }
    else if (eg_check_flag(target, ENTITY_FLAG_DOWN))
    {
        eg_clear_flag(target, ENTITY_FLAG_DOWN);
    }

    // jumping
    if (eg_consume_input(app, EG_KEYCODE_SPACE))
    {
        if (eg_check_flag(target, ENTITY_FLAG_GROUND) &&
            !eg_check_flag(target, ENTITY_FLAG_JUMP))
        {
            eg_clear_flag(target, ENTITY_FLAG_GROUND);
            eg_set_flag(target, ENTITY_FLAG_JUMP);
            eg_clear_flag(target, ENTITY_FLAG_MOVE);
            target->link = NULL;
            target->y_vel = 0;
            target->y_vel -= 12;
        }
    }

    // TEMP: Reset the state of various things.
    if (eg_consume_input(app, EG_KEYCODE_R))
    {
        // Reset the target entity (the player).
        target->x_pos = 80;
        target->y_pos = 10;
        target->x_vel = 0;
        target->y_vel = 0;
        target->flags = 0;

        // Reset the camera.
        app->cam.x = 0;
        app->cam.y = 0;
    }

    // If the escape key is pressed, terminate the application.
    if (eg_consume_input(app, EG_KEYCODE_ESCAPE))
    {
        printf("The escape key was pressed. "
               "The application should now terminate.\n");
        app->done = 1;
    }
}
