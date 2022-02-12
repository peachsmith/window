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

    // TEMP: Output the flags. write the most significant bit first.
    if (eg_consume_input(app, SDL_SCANCODE_F))
    {
        uint8_t flags = target->flags;
        for (int i = 7; i >= 0; i--)
        {
            uint8_t f = (flags & (1 << i));
            printf("%u", (f >> i));
        }
        printf("\n");
    }

    if (eg_consume_input(app, SDL_SCANCODE_T))
    {
        eg_set_flag(target, 0);
    }

    if (eg_consume_input(app, SDL_SCANCODE_Y))
    {
        eg_clear_flag(target, 0);
    }

    if (eg_consume_input(app, SDL_SCANCODE_U))
    {
        printf("flag 0: %d\n", eg_check_flag(target, 0));
    }

    if (eg_consume_input(app, SDL_SCANCODE_1))
        eg_toggle_flag(target, 0);
    if (eg_consume_input(app, SDL_SCANCODE_2))
        eg_toggle_flag(target, 1);
    if (eg_consume_input(app, SDL_SCANCODE_3))
        eg_toggle_flag(target, 2);
    if (eg_consume_input(app, SDL_SCANCODE_4))
        eg_toggle_flag(target, 3);
    if (eg_consume_input(app, SDL_SCANCODE_5))
        eg_toggle_flag(target, 4);
    if (eg_consume_input(app, SDL_SCANCODE_6))
        eg_toggle_flag(target, 5);
    if (eg_consume_input(app, SDL_SCANCODE_7))
        eg_toggle_flag(target, 6);
    if (eg_consume_input(app, SDL_SCANCODE_8))
        eg_toggle_flag(target, 7);

    // If the escape key is pressed, terminate the application.
    if (eg_consume_input(app, SDL_SCANCODE_ESCAPE))
    {
        printf("The escape key was pressed. "
               "The application should now terminate.\n");
        app->done = 1;
    }
}
