#include "demo/menu/menu.h"

#include <stdio.h>

// maximum number of menus
#define MAX_MENUS 10

// list of active menu entities
static eg_entity *menus[MAX_MENUS];

int demo_init_menus(eg_app *app)
{
    app->menus = &(menus[0]);
    app->menu_count = 0;

    return 1;
}
