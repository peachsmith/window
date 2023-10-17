#include "common/menu.h"

// maximum number of menus
#define MAX_MENUS 10

// TODO: move statically allocated data like this into the main.c files of
// any applications.
// list of active menu entities
static cr_entity *menus[MAX_MENUS];

int common_init_menus(cr_app *app)
{
    app->menus = &(menus[0]);
    app->menu_count = 0;

    return 1;
}
