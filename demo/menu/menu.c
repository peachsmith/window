#include "demo/menu/menu.h"

#include <stdio.h>

// maximum number of menus
#define MAX_MENUS 10

// Deprecated
// main menu list for the application
// TODO: remove this once all menus are converted to entities
static eg_menu *menus[MAX_MENUS];

// list of active menu entities
static eg_entity *menu_entities[MAX_MENUS];

int demo_init_menus(eg_app *app)
{
    if (!demo_init_debug_menu(app))
    {
        return 0;
    }

    // demo_init_info_menu(app);
    demo_init_scenes_menu(app);
    demo_init_input_menu(app);

    app->menus = &(menus[0]);
    app->menu_entities = &(menu_entities[0]);
    app->menu_count = 0;

    return 1;
}
