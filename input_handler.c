#include "input_handler.h"

#include <stdio.h>

void root_input_callback(eg_app *app)
{
    if (eg_consume_input(app, SDL_SCANCODE_UP))
    {
        printf("up was pressed\n");
    }

    if (eg_consume_input(app, SDL_SCANCODE_DOWN))
    {
        printf("down was pressed\n");
    }

    if (eg_peek_input(app, SDL_SCANCODE_LEFT))
    {
        printf("left was pressed\n");
    }

    if (eg_peek_input(app, SDL_SCANCODE_RIGHT))
    {
        printf("right was pressed\n");
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
    }

    // If the escape key is pressed, terminate the application.
    if (eg_consume_input(app, SDL_SCANCODE_ESCAPE))
    {
        printf("The escape key was pressed. "
               "The application should now terminate.\n");
        app->done = 1;
    }
}

void second_input_callback(eg_app *app)
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
