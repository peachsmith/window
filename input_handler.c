#include "example.h"

void my_input_handler(eg_app *app)
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

    // If the escape key is pressed, terminate the application.
    if (eg_consume_input(app, SDL_SCANCODE_ESCAPE))
    {
        printf("The escape key was pressed. "
               "The application should now terminate\n.");
        app->done = 1;
    }
}
