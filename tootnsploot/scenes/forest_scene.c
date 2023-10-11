#include "tootnsploot/tootnsploot.h"
#include "tootnsploot/input/input.h"
#include "tootnsploot/scenes/scenes.h"
#include "tootnsploot/entities/transition.h"
#include "tootnsploot/entities/pause_menu.h"
#include "tootnsploot/entities/hud.h"
#include "tootnsploot/entities/forest.h"
#include "tootnsploot/entities/corgi.h"
#include "tootnsploot/entities/floor.h"
#include "tootnsploot/entities/wall.h"

#include "common/util.h"

void load_forest_scene(eg_app *app)
{
    app->scene = TNS_SCENE_FOREST;

    // Reset the counters.
    app->counters[TNS_COUNTER_BREATH] = 3;
    app->counters[TNS_COUNTER_CRITTERS] = 0;
    app->counters[TNS_COUNTER_SCORE] = 0;
    for (int i = 0; i < 8; i++)
    {
        app->counters[i + TNS_COUNTER_CRITTER_SLOT_OFFSET] = 0;
    }

    util_set_camera(app, EG_CAMERA_NONE);
    app->cam.x = 0;
    app->cam.y = 0;

    // menus
    tns_create_pause_menu(app);

    // scene transition
    tns_create_transition(app);

    // background
    tns_create_forest(app, 0, 22);

    // floor and walls
    tns_create_floor(app, 0, 142);
    tns_create_wall(app, -24, 2);
    tns_create_wall(app, 240, 2);

    // player
    app->primary = tns_create_corgi(app, 80, 118);

    // heads up display
    tns_create_hud(app, 0, 0);
}
