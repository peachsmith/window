#include "tootnsploot/tootnsploot.h"
#include "tootnsploot/input/input.h"
#include "tootnsploot/entities/entity_types.h"
#include "tootnsploot/scenes/scenes.h"
#include "common/menu.h"

void controls_menu_input_handler(eg_app *app)
{
    // If either the escape key or the x key is pressed, return to the main menu.
    if (eg_consume_input(app, EG_KEYCODE_ESCAPE) || eg_consume_input(app, EG_KEYCODE_X))
    {
        app->menu_count--;
        eg_pop_input_handler(app);
        return;
    }
}
