#include "crumbs.h"
#include "tootnsploot/tootnsploot.h"

int main(int argc, char **argv)
{
    if (!eg_initialize())
    {
        return 1;
    }

    eg_app *app = eg_create_app();
    if (app == NULL)
    {
        eg_terminate();
        return 1;
    }

    if (!tns_prepare(app))
    {
        eg_destroy_app(app);
        eg_terminate();
        return 1;
    }

    while (!app->done)
    {
        eg_begin_frame(app);
        app->update(app);
        app->draw(app);
        eg_end_frame(app);
    }

    eg_destroy_app(app);
    eg_terminate();

    return 0;
}
