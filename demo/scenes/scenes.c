#include "demo/scenes/scenes.h"

#include <stdlib.h>

void clear_scene(eg_app *app)
{
    app->entity_count = 0;
    app->menu_count = 0;
    app->dialog_count = 0;
    app->overlay_count = 0;
}
