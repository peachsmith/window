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

        // Clear the screen.
        eg_clear(app);

        // Draw the contents of the current frame.
        eg_draw(app);

        // Render the content of the current frame to the screen.
        eg_show(app);

        // Delay to regulate framerate.
        eg_delay();
    }

    // Destroy the application context.
    eg_destroy_app(app);

    // Terminate the underlying implementation.
    eg_terminate();

    return 0;
}
