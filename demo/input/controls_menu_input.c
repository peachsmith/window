#include "demo/input/input.h"
#include "demo/menu/menu.h"
#include "demo/entities/entity_types.h"
#include "demo/scenes/scenes.h"

#include <stdio.h>

void tns_controls_menu_input_handler(eg_app *app)
{
    // If either the escape key or the x key is pressed, return to the main menu.
    if (eg_consume_input(app, EG_KEYCODE_ESCAPE) || eg_consume_input(app, EG_KEYCODE_X))
    {
        app->menu_count--;
        eg_pop_input_handler(app);
        return;
    }
}
