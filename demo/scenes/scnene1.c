#include "demo/scenes/scenes.h"
#include "demo/input/input.h"
#include "demo/entities/block.h"
#include "demo/entities/player.h"
#include "demo/entities/transition.h"
#include "demo/entities/pause_menu.h"
#include "demo/entities/fish_menu.h"
#include "demo/entities/info_menu.h"
#include "demo/entities/debug_menu.h"
#include "demo/entities/scene_menu.h"
#include "demo/entities/input_menu.h"
#include "demo/entities/info_dialog.h"
#include "demo/entities/demo_dialog.h"

void load_scene_1(eg_app *app)
{
    // Starting position for building a sequence of blocks.
    int x_start = 50;
    // int y_start = 0; // currently unused

    app->cam.x = 0;
    app->cam.y = 0;

    // menus
    eg_add_entity(app, pause_menu_demo_create());
    eg_add_entity(app, fish_menu_demo_create());
    eg_add_entity(app, info_menu_demo_create());
    eg_add_entity(app, debug_menu_demo_create());
    eg_add_entity(app, scene_menu_demo_create());
    eg_add_entity(app, input_menu_demo_create());

    // dialogs
    eg_add_entity(app, demo_dialog_demo_create());
    eg_add_entity(app, info_dialog_demo_create());

    // moving platforms added before the player
    eg_add_entity(app, block_demo_create_moving(x_start - 63, 54, 0));  // vertical
    eg_add_entity(app, block_demo_create_moving(x_start + 253, 94, 1)); // horizontal

    eg_add_entity(app, block_demo_create(x_start - 27 + 18, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 36, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 54, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 72, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 90, 94));

    eg_add_entity(app, block_demo_create(x_start - 27 + 36, 76)); // x = 59
    eg_add_entity(app, block_demo_create(x_start - 27 + 36, 58)); // x = 59
    eg_add_entity(app, block_demo_create(x_start - 27 + 36, 40)); // x = 59

    // player
    eg_add_entity(app, player_demo_create(100, 55));

    eg_add_entity(app, transition_demo_create());

    // Add the rest of the horizontal row.
    eg_add_entity(app, block_demo_create(x_start - 27 + 108, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 126, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 144, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 162, 94));

    eg_add_entity(app, block_demo_create(x_start - 27 + 108, 76)); // x = 131

    // scene transition
    eg_add_entity(app, transition_demo_create());
}
