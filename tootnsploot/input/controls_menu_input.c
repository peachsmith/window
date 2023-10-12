#include "tootnsploot/tootnsploot.h"
#include "tootnsploot/input/input.h"
#include "tootnsploot/entities/entity_types.h"
#include "tootnsploot/scenes/scenes.h"
#include "common/menu.h"

void controls_menu_input_handler(cr_app *app)
{
    // If either the escape key or the x key is pressed, return to the main menu.
    if (cr_consume_input(app, CR_KEYCODE_ESCAPE) || cr_consume_input(app, CR_KEYCODE_X))
    {
        app->menu_count--;
        cr_pop_input_handler(app);
        return;
    }
}
