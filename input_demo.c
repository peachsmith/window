#include "input_demo.h"
#include "entity_demo.h"
#include "player_demo.h"

#include <stdio.h>

void root_input_callback(eg_app *app, eg_entity *target)
{
    if (eg_peek_input(app, SDL_SCANCODE_UP))
    {
        if (target->y_vel >= -2)
        {
            target->y_vel -= 2;
        }
    }

    if (eg_peek_input(app, SDL_SCANCODE_DOWN))
    {
        if (target->y_vel <= 2)
        {
            target->y_vel += 2;
        }
    }

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

    // If the Q key is pressed, add the Q entity.
    // If the shift key was also pressed, remove the Q entity.
    if (eg_consume_input(app, SDL_SCANCODE_Q))
    {
        if (eg_peek_input(app, SDL_SCANCODE_LSHIFT) ||
            eg_peek_input(app, SDL_SCANCODE_RSHIFT))
        {
            entity_demo_remove(app, ENTITY_ID_Q);
        }
        else if (!entity_demo_add(app, ENTITY_ID_Q))
        {
            app->done = 1;
            return;
        }
    }

    // If the W key is pressed, add the W entity.
    // If the shift key was also pressed, remove the W entity.
    if (eg_consume_input(app, SDL_SCANCODE_W))
    {
        if (eg_peek_input(app, SDL_SCANCODE_LSHIFT) ||
            eg_peek_input(app, SDL_SCANCODE_RSHIFT))
        {
            entity_demo_remove(app, ENTITY_ID_W);
        }
        else if (!entity_demo_add(app, ENTITY_ID_W))
        {
            app->done = 1;
            return;
        }
    }

    // If the E key is pressed, add the E entity.
    // If the shift key was also pressed, remove the E entity.
    if (eg_consume_input(app, SDL_SCANCODE_E))
    {
        if (eg_peek_input(app, SDL_SCANCODE_LSHIFT) ||
            eg_peek_input(app, SDL_SCANCODE_RSHIFT))
        {
            entity_demo_remove(app, ENTITY_ID_E);
        }
        else if (!entity_demo_add(app, ENTITY_ID_E))
        {
            app->done = 1;
            return;
        }
    }

    if (eg_consume_input(app, SDL_SCANCODE_P))
    {
        entity_demo_print(app);
    }

    // If the N key is pressed, create a new input handler and push it onto
    // the stack. If we fail to create the new handler, then terminate the
    // application.
    if (eg_consume_input(app, SDL_SCANCODE_N))
    {
        printf("pushing a new input handler onto the stack...\n");
        eg_input_handler *new_handler =
            eg_create_input_handler(second_input_callback);
        if (new_handler == NULL)
        {
            app->done = 1;
            return;
        }
        eg_push_input_handler(app, new_handler);
        return;
    }

    // If the escape key is pressed, terminate the application.
    if (eg_consume_input(app, SDL_SCANCODE_ESCAPE))
    {
        printf("The escape key was pressed. "
               "The application should now terminate.\n");
        app->done = 1;
    }
}

void second_input_callback(eg_app *app, eg_entity *target)
{
    if (eg_consume_input(app, SDL_SCANCODE_UP))
    {
        printf("up was pressed in the second input handler\n");
    }

    if (eg_consume_input(app, SDL_SCANCODE_DOWN))
    {
        printf("down was pressed in the second input handler\n");
    }

    if (eg_peek_input(app, SDL_SCANCODE_LEFT))
    {
        printf("left was pressed in the second input handler\n");
    }

    if (eg_peek_input(app, SDL_SCANCODE_RIGHT))
    {
        printf("right was pressed in the second input handler\n");
    }

    // If the escape key is pressed, pop the current handler from the stack
    // and destroy it. If the shift key was pressed when the escape key was
    // pressed, then we terminate the application and let the application
    // destruction handle the disposal of all input handlers.
    if (eg_consume_input(app, SDL_SCANCODE_ESCAPE))
    {
        if (eg_peek_input(app, SDL_SCANCODE_LSHIFT) ||
            eg_peek_input(app, SDL_SCANCODE_RSHIFT))
        {
            printf("shift and escape were pressed together. "
                   "The application should now temrinate.\n");
            app->done = 1;
            return;
        }

        printf("The escape key was pressed. "
               "This input handler will now be popped from the stack.\n");

        eg_input_handler *handler = eg_pop_input_handler(app);
        eg_destroy_input_handler(handler);
    }
}
