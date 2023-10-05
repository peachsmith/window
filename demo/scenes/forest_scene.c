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
#include "demo/util/util.h"
#include "demo/demo.h"

void load_forest_scene(eg_app *app)
{
    app->counters[DEMO_COUNTER_NOTES] = 0;

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
    jimbo_dialog_demo_create(app);
    sign_dialog_demo_create(app);

    // scene transition
    transition_demo_create(app);

    // background
    forest_demo_create(app, 0, 22);

    // floor and walls
    block_demo_create_floor(app, 0, 142);
    block_demo_create_wall(app, -23, 2);
    block_demo_create_wall(app, 239, 2);

    // player
    app->primary = corgi_demo_create(app, 80, 80);

    // heads up display
    hud_demo_create(app, 0, 0);
}
