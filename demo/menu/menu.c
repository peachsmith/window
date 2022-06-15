#include "demo/menu/menu.h"

#include <stdio.h>

// maximum number of menus
#define MAX_MENUS 10

// main menu list for the application
static eg_menu *menus[MAX_MENUS];

void demo_init_menus(eg_app *app)
{
    demo_init_pause_menu(app);
    demo_init_fish_menu(app);
    demo_init_info_menu(app);
    demo_init_debug_menu(app);

    app->menus = &(menus[0]);
    app->menu_count = 0;
}
