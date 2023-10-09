#include "demo/scenes/scenes.h"
#include "demo/input/input.h"
#include "demo/entities/transition.h"
#include "demo/entities/forest.h"
#include "demo/entities/main_menu.h"
#include "demo/util/util.h"
#include "demo/demo.h"

void load_title_screen(eg_app *app)
{
    app->scene = DEMO_SCENE_TITLE_SCREEN;

    demo_set_camera(app, EG_CAMERA_NONE);
    app->cam.x = 0;
    app->cam.y = 0;

    // Create the main menu and set it as the current active menu.
    eg_entity *main_menu = tns_create_main_menu(app);
    app->menus[app->menu_count++] = main_menu;
    app->pause = 1;

    // scene transition
    transition_demo_create(app);
}
