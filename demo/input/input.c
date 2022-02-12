#include "demo/input/input.h"
#include "demo/entities/player.h"
#include "demo/entities/entity_types.h"

#include <stdio.h>

void root_input_callback(eg_app *app, eg_entity *target)
{
    // left movement
    if (eg_peek_input(app, SDL_SCANCODE_LEFT))
    {
        if (target->x_vel >= -2)
        {
            target->x_vel -= 2;
        }
    }

    // right movement
    if (eg_peek_input(app, SDL_SCANCODE_RIGHT))
    {
        if (target->x_vel <= 2)
        {
            target->x_vel += 2;
        }
    }

    // jumping
    if (eg_consume_input(app, SDL_SCANCODE_SPACE))
    {
        if (!eg_check_flag(target, 0))
        {
            eg_set_flag(target, 0);
            target->y_vel -= 12;
        }
    }

    // TEMP: Reset the state of various things.
    if (eg_consume_input(app, SDL_SCANCODE_R))
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
    if (eg_consume_input(app, SDL_SCANCODE_ESCAPE))
    {
        printf("The escape key was pressed. "
               "The application should now terminate.\n");
        app->done = 1;
    }
}
