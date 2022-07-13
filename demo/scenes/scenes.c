#include "demo/scenes/scenes.h"

#include <stdlib.h>

void clear_scene(eg_app *app)
{
    eg_pop_input_handler(app);

    // Destroy the entities.
    while (app->entities != NULL)
    {
        eg_entity *next = app->entities->next;
        eg_destroy_entity(app->entities);
        app->entities = next;
    }

    app->entities = NULL;
}
