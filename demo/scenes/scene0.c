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

#include <stdio.h>

void load_scene_0(eg_app *app)
{
    // Starting position for building a sequence of blocks.
    // int x_start = 10;
    // int y_start = 30;

    app->cam.x = -320;
    app->cam.y = 40;

    // menus
    eg_add_entity(app, pause_menu_demo_create(app));
    eg_add_entity(app, fish_menu_demo_create(app));
    eg_add_entity(app, info_menu_demo_create(app));
    eg_add_entity(app, debug_menu_demo_create(app));
    eg_add_entity(app, scene_menu_demo_create(app));
    eg_add_entity(app, input_menu_demo_create(app));

    // dialogs
    eg_add_entity(app, demo_dialog_demo_create(app));
    eg_add_entity(app, info_dialog_demo_create(app));
    eg_add_entity(app, jimbo_dialog_demo_create(app));
    eg_add_entity(app, sign_dialog_demo_create(app));

    eg_add_entity(app, block_demo_create(app, 1, 94));
    eg_add_entity(app, block_demo_create(app, 19, 94));
    eg_add_entity(app, block_demo_create(app, 37, 94));
    eg_add_entity(app, block_demo_create(app, 55, 94));
    eg_add_entity(app, block_demo_create(app, 73, 94));

    // Vertically moving platform (added before player).
    eg_add_entity(app, jimbo_demo_create(app, 434, -60)); // jimbo BEFORE vertically moving platform
    eg_add_entity(app, block_demo_create_moving(app, 433, -2, 0));
    eg_add_entity(app, jimbo_demo_create(app, 459, -60)); // jimbo AFTER vertically moving platform

    // Horizontally moving platform (added before player).
    eg_add_entity(app, jimbo_demo_create(app, 635, -40)); // jimbo BEFORE horizontally moving platform
    eg_add_entity(app, block_demo_create_moving(app, 640, -2, 1));
    eg_add_entity(app, jimbo_demo_create(app, 659, -40)); // jimbo AFTER horizontally moving platform

    // A sign that contains information.
    eg_add_entity(app, sign_demo_create(app, 37, 76));

    // player
    eg_add_entity(app, player_demo_create(app, 150, 0));

    // scene transition
    eg_add_entity(app, transition_demo_create(app));

    // Add the rest of the horizontal row.
    eg_add_entity(app, block_demo_create(app, 91, 94));
    eg_add_entity(app, block_demo_create(app, 109, 94));
    eg_add_entity(app, block_demo_create(app, 127, 94));
    eg_add_entity(app, block_demo_create(app, 145, 94));

    // Horizontally moving platform (added after player).
    eg_add_entity(app, block_demo_create_moving(app, 640, 62, 1));

    // Sloped platform inclined from left to right, followed by a flat
    // sloped platform (for dismounting) followed by a slope down, then up.
    //
    // It looks something like this
    //    _____
    //   /     \    /
    //  /       \  /
    // /         \/
    eg_add_entity(app, block_demo_create_sloped(app, 145, 76, 1)); // ramp up from left to right
    eg_add_entity(app, block_demo_create_sloped(app, 217, 76, 2)); // flat slope
    eg_add_entity(app, block_demo_create_sloped(app, 289, 76, 0)); // ramp down from left to right
    eg_add_entity(app, block_demo_create_sloped(app, 361, 76, 1)); // ramp up from left to right
    eg_add_entity(app, block_demo_create(app, 355, 93));           // solid ground to allow transition between sloped blocks

    // Long slope built from several sloped platforms.
    eg_add_entity(app, block_demo_create_sloped(app, -71, 94, 2));
    eg_add_entity(app, block_demo_create_sloped(app, -143, 76, 0));
    eg_add_entity(app, block_demo_create_sloped(app, -215, 58, 0));
    eg_add_entity(app, block_demo_create(app, -85, 94)); // solid ground to allow transition between sloped blocks

    // Vertically moving platform (after player).
    eg_add_entity(app, block_demo_create_moving(app, 489, -2, 0));
}