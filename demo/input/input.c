#include "demo/input/input.h"
#include "demo/entities/player.h"
#include "demo/entities/entity_types.h"

#include <stdio.h>

void root_input_callback(eg_app *app, eg_entity *target)
{
    if (eg_peek_input(app, SDL_SCANCODE_LEFT))
    {
        if (target->x_vel >= -2)
        {
            target->x_vel -= 2;
        }
    }

    if (eg_peek_input(app, SDL_SCANCODE_RIGHT))
    {
        if (target->x_vel <= 2)
        {
            target->x_vel += 2;
        }
    }

    if (eg_peek_input(app, SDL_SCANCODE_SPACE))
    {
        target->y_vel -= 2;
    }

    // If the escape key is pressed, terminate the application.
    if (eg_consume_input(app, SDL_SCANCODE_ESCAPE))
    {
        printf("The escape key was pressed. "
               "The application should now terminate.\n");
        app->done = 1;
    }
}
