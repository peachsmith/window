#include "common/menu.h"

// maximum number of menus
#define MAX_MENUS 10

// list of active menu entities
static cr_entity *menus[MAX_MENUS];

int common_init_menus(cr_app *app)
{
    app->menus = &(menus[0]);
    app->menu_count = 0;

    return 1;
}
