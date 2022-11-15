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
#include "demo/entities/info_dialog.h"
#include "demo/entities/demo_dialog.h"
#include "demo/entities/jimbo_dialog.h"

#include <stdio.h>

void load_scene_0(eg_app *app)
{
    eg_entity *player;

    // Starting position for building a sequence of blocks.
    // int x_start = 10;
    // int y_start = 30;

    app->cam.x = -320;
    app->cam.y = 40;

    eg_add_entity(app, transition_demo_create());

    // menus
    eg_add_entity(app, pause_menu_demo_create());
    eg_add_entity(app, fish_menu_demo_create());
    eg_add_entity(app, info_menu_demo_create());

    // dialogs
    eg_add_entity(app, demo_dialog_demo_create());
    eg_add_entity(app, info_dialog_demo_create());
    eg_add_entity(app, jimbo_dialog_demo_create());

    // eg_add_entity(app, jimbo_demo_create(x_start - 27 + 200, 36)); // an interactable NPC

    eg_add_entity(app, block_demo_create(1, 94));
    eg_add_entity(app, block_demo_create(19, 94));
    eg_add_entity(app, block_demo_create(37, 94));
    eg_add_entity(app, block_demo_create(55, 94));
    eg_add_entity(app, block_demo_create(73, 94));

    // Vertically moving platform (added before player).
    eg_add_entity(app, jimbo_demo_create(434, -60)); // jimbo BEFORE vertically moving platform
    eg_add_entity(app, block_demo_create_moving(433, -2, 0));
    eg_add_entity(app, jimbo_demo_create(459, -60)); // jimbo AFTER vertically moving platform

    // Horizontally moving platform (added before player).
    eg_add_entity(app, jimbo_demo_create(635, -40)); // jimbo BEFORE horizontally moving platform
    eg_add_entity(app, block_demo_create_moving(640, -2, 1));
    eg_add_entity(app, jimbo_demo_create(659, -40)); // jimbo AFTER horizontally moving platform

    // Create the player entity and add it to the app.
    player = player_demo_create(150, 0); // 150, 40
    eg_add_entity(app, player);

    // TEMP: remove this once searchable entities are implemented.
    app->player = player;

    // Add the rest of the horizontal row.
    eg_add_entity(app, block_demo_create(91, 94));
    eg_add_entity(app, block_demo_create(109, 94));
    eg_add_entity(app, block_demo_create(127, 94));
    eg_add_entity(app, block_demo_create(145, 94));

    // Horizontally moving platform (added after player).
    eg_add_entity(app, block_demo_create_moving(640, 62, 1));

    // Sloped platform inclined from left to right, followed by a flat
    // sloped platform (for dismounting) followed by a slope down, then up.
    //
    // It looks something like this
    //    _____
    //   /     \    /
    //  /       \  /
    // /         \/
    eg_add_entity(app, block_demo_create_sloped(145, 76, 1)); // ramp up from left to right
    eg_add_entity(app, block_demo_create_sloped(217, 76, 2)); // flat slope
    eg_add_entity(app, block_demo_create_sloped(289, 76, 0)); // ramp down from left to right
    eg_add_entity(app, block_demo_create_sloped(361, 76, 1)); // ramp up from left to right
    eg_add_entity(app, block_demo_create(355, 93));           // solid ground to allow transition between sloped blocks

    // Long slope built from several sloped platforms.
    eg_add_entity(app, block_demo_create_sloped(-71, 94, 2));
    eg_add_entity(app, block_demo_create_sloped(-143, 76, 0));
    eg_add_entity(app, block_demo_create_sloped(-215, 58, 0));
    eg_add_entity(app, block_demo_create(-85, 94)); // solid ground to allow transition between sloped blocks

    // A sign that contains information.
    eg_add_entity(app, sign_demo_create(37, 76));

    // Vertically moving platform (after before player).
    eg_add_entity(app, block_demo_create_moving(489, -2, 0));

    // TODO: push input handler outside of scene loader
    // Create the initial input handler and add it to the app.
    // eg_push_input_handler(app, root_input_handler);
}