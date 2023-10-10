#include "demo/scenes/scenes.h"
#include "demo/input/input.h"
#include "demo/entities/block.h"
#include "demo/entities/player.h"
#include "demo/entities/sign.h"
#include "demo/entities/jimbo.h"
#include "demo/entities/transition.h"
#include "demo/entities/pause_menu.h"
#include "demo/entities/fish_menu.h"
#include "demo/entities/info_menu.h"
#include "demo/entities/debug_menu.h"
#include "demo/entities/scene_menu.h"
#include "demo/entities/input_menu.h"
#include "demo/entities/info_dialog.h"
#include "demo/entities/demo_dialog.h"
#include "demo/entities/jimbo_dialog.h"
#include "demo/entities/sign_dialog.h"
#include "demo/entities/hud.h"
#include "demo/entities/forest.h"
#include "demo/entities/corgi.h"
#include "demo/entities/floor.h"
#include "demo/entities/wall.h"
#include "demo/util/util.h"
#include "demo/demo.h"

void load_forest_scene(eg_app *app)
{
    app->scene = DEMO_SCENE_FOREST;

    app->counters[DEMO_COUNTER_BREATH] = 3;

    demo_set_camera(app, EG_CAMERA_NONE);
    app->cam.x = 0;
    app->cam.y = 0;

    // menus
    pause_menu_demo_create(app);
    fish_menu_demo_create(app);
    info_menu_demo_create(app);
    debug_menu_demo_create(app);
    scene_menu_demo_create(app);
    input_menu_demo_create(app);

    // dialogs
    demo_dialog_demo_create(app);
    info_dialog_demo_create(app);

    // scene transition
    transition_demo_create(app);

    // background
    forest_demo_create(app, 0, 22);

    // floor and walls
    tns_create_floor(app, 0, 142);
    tns_create_wall(app, -24, 2);
    tns_create_wall(app, 240, 2);

    // player
    app->primary = tns_create_corgi(app, 80, 118);

    // heads up display
    tns_create_hud(app, 0, 0);
}
