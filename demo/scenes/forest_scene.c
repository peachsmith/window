#include "demo/scenes/scenes.h"
#include "demo/input/input.h"
#include "demo/entities/transition.h"
#include "demo/entities/tns_pause_menu.h"
#include "demo/entities/hud.h"
#include "demo/entities/forest.h"
#include "demo/entities/corgi.h"
#include "demo/entities/floor.h"
#include "demo/entities/wall.h"
#include "demo/util/util.h"
#include "demo/demo.h"

void load_forest_scene(eg_app *app)
{
    app->scene = TNS_SCENE_FOREST;

    app->counters[DEMO_COUNTER_BREATH] = 3;
    app->counters[DEMO_COUNTER_CRITTERS] = 0;

    demo_set_camera(app, EG_CAMERA_NONE);
    app->cam.x = 0;
    app->cam.y = 0;

    // menus
    tns_create_pause_menu(app);

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
