#include "tootnsploot/tootnsploot.h"
#include "tootnsploot/input/input.h"
#include "tootnsploot/scenes/scenes.h"
#include "tootnsploot/entities/transition.h"
#include "tootnsploot/entities/forest.h"
#include "tootnsploot/entities/main_menu.h"
#include "tootnsploot/entities/controls_menu.h"
#include "tootnsploot/entities/characters_menu.h"

#include "common/util.h"

void load_title_screen(cr_app *app)
{
    app->scene = TNS_SCENE_TITLE_SCREEN;

    util_set_camera(app, CR_CAMERA_NONE);
    app->cam.x = 0;
    app->cam.y = 0;

    // Create the main menu and set it as the current active menu.
    cr_entity *main_menu = tns_create_main_menu(app);
    app->menus[app->menu_count++] = main_menu;
    app->pause = 1;

    tns_create_controls_menu(app);
    tns_create_characters_menu(app);

    // scene transition
    tns_create_transition(app);
}
