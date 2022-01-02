#include <stdio.h>
#include "example.h"

int main(int argc, char **argv)
{
    // Initialize the underlying implementation.
    if (!eg_initialize())
    {
        return 1;
    }

    // Create the application context.
    eg_app *app = eg_create_app();
    if (app == NULL)
    {
        eg_terminate();
        return 1;
    }

    // Begin the main loop.
    while (!app->done)
    {
        // Process OS events.
        eg_process_events(app);

        // Handle input.
        eg_handle_input(app);

        // Update the state of the application.
        eg_update(app);

        // Draw the contents of the current frame.
        eg_draw(app);

        // Delay to regulate framerate.
        eg_delay(app);
    }

    // Destroy the application context.
    eg_destroy_app(app);

    // Terminate the underlying implementation.
    eg_terminate();

    return 0;
}
