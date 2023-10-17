#include "demo/demo.h"
#include "demo/input/input.h"
#include "demo/scenes/scenes.h"
#include "demo/entities/block.h"
#include "demo/entities/player.h"
#include "demo/entities/henry.h"
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

#include "common/util.h"

void load_scene_3(cr_app *app)
{
    cr_entity **handles = app->extension->entity_handles;

    app->scene = DEMO_SCENE_3;

    // Starting position for building a sequence of blocks.
    int x_start = 50;

    util_set_camera(app, CR_CAMERA_ALL);

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

    block_demo_create(app, x_start - 27 + 18, 94);
    block_demo_create(app, x_start - 27 + 36, 94);
    block_demo_create(app, x_start - 27 + 54, 94);
    block_demo_create(app, x_start - 27 + 72, 94);
    block_demo_create(app, x_start - 27 + 90, 94);

    // player
    handles[DEMO_HANDLE_PLAYER] = demo_create_player(app, 100, 55);

    // Add the rest of the horizontal row.
    block_demo_create(app, x_start - 27 + 108, 94);
    block_demo_create(app, x_start - 27 + 126, 94);
    block_demo_create(app, x_start - 27 + 144, 94);
    block_demo_create(app, x_start - 27 + 162, 94);
    block_demo_create(app, x_start - 27 + 180, 94);
    block_demo_create(app, x_start - 27 + 198, 94);
    block_demo_create(app, x_start - 27 + 216, 94);
    block_demo_create(app, x_start - 27 + 234, 94);
    block_demo_create(app, x_start - 27 + 252, 94);

    // Vertical column 1.
    block_demo_create(app, x_start - 27 + 18, 76);
    block_demo_create(app, x_start - 27 + 18, 58);
    block_demo_create(app, x_start - 27 + 18, 40);
    block_demo_create(app, x_start - 27 + 18, 22);

    // Vertical column 2
    block_demo_create(app, x_start - 27 + 198, 76);
    block_demo_create(app, x_start - 27 + 198, 58);
    block_demo_create(app, x_start - 27 + 198, 40);
    block_demo_create(app, x_start - 27 + 198, 22);

    // A hostile entity.
    henry_demo_create(app, x_start - 27 + 150, 22);

    handles[DEMO_HANDLE_TRANSITION] = transition_demo_create(app);
}
