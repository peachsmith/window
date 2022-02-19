// This is a demonstrationg of using the Example interface.
// The prepare function is defined and implemented in

#include "example.h"
#include "demo/demo.h"
#include <stdlib.h>

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

    // Implement application behavior.
    if (!prepare(app))
    {
        eg_destroy_app(app);
        eg_terminate();
        return 1;
    }

    // Begin the main loop.
    // TODO: refactor this to use begin_frame and end_frame
    while (!app->done)
    {
        // Process OS events.
        eg_process_events(app);

        // Update the state of the application.
        app->update(app);

        // Draw the contents of the current frame.
        app->draw(app);

        // Delay to regulate framerate.
        eg_delay(app);
    }

    // Destroy the application context.
    eg_destroy_app(app);

    // Terminate the underlying implementation.
    eg_terminate();

    return 0;
}
