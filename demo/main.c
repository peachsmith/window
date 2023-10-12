// This is a demonstrationg of using the Crumbs interface.
// The prepare function is defined and implemented in

#include "crumbs.h"
#include "demo/demo.h"

int main(int argc, char **argv)
{
    // Initialize the underlying implementation.
    if (!cr_initialize())
    {
        return 1;
    }

    // Create the application context.
    cr_app *app = cr_create_app();
    if (app == NULL)
    {
        cr_terminate();
        return 1;
    }

    // Implement application behavior.
    if (!demo_prepare(app))
    {
        cr_destroy_app(app);
        cr_terminate();
        return 1;
    }

    // Begin the main loop.
    while (!app->done)
    {
        cr_begin_frame(app);

        // Update the state of the application.
        app->update(app);

        // Draw the contents of the current frame.
        app->draw(app);

        cr_end_frame(app);
    }

    // Destroy the application context.
    cr_destroy_app(app);

    // Terminate the underlying implementation.
    cr_terminate();

    return 0;
}
