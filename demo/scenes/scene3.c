#include "demo/scenes/scenes.h"
#include "demo/input/input.h"
#include "demo/entities/block.h"
#include "demo/entities/player.h"
#include "demo/entities/henry.h"
#include "demo/entities/transition.h"
#include "demo/entities/pause_menu.h"
#include "demo/entities/fish_menu.h"
#include "demo/entities/demo_dialog_entity.h"

#include <stdlib.h>

void load_scene_3(eg_app *app)
{
    eg_entity *player;

    // Starting position for building a sequence of blocks.
    int x_start = 50;
    // int y_start = 0; // currently unused

    app->cam.x = 0;
    app->cam.y = 0;

    // screen transition
    // This is added first, since it should be rendered on top of all other
    // entities.
    eg_add_entity(app, transition_demo_create());

    // pause menu
    eg_add_entity(app, pause_menu_demo_create());
    eg_add_entity(app, fish_menu_demo_create());

    // dialogs
    eg_add_entity(app, demo_dialog_entity_demo_create());

    eg_add_entity(app, block_demo_create(x_start - 27 + 18, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 36, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 54, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 72, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 90, 94));

    // eg_add_entity(app, block_demo_create(x_start - 27 + 36, 76)); // x = 59
    // eg_add_entity(app, block_demo_create(x_start - 27 + 36, 58)); // x = 59
    // eg_add_entity(app, block_demo_create(x_start - 27 + 36, 40)); // x = 59

    // eg_add_entity(app, block_demo_create(x_start - 27 + 54, 40)); // x = 59

    // Create the player entity and add it to the app.
    player = player_demo_create(100, 55);
    eg_add_entity(app, player);

    // TEMP: remove this once searchable entities are implemented.
    app->player = player;

    // Add the rest of the horizontal row.
    eg_add_entity(app, block_demo_create(x_start - 27 + 108, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 126, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 144, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 162, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 180, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 198, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 216, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 234, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 252, 94));

    // Vertical column 1.
    eg_add_entity(app, block_demo_create(x_start - 27 + 18, 76));
    eg_add_entity(app, block_demo_create(x_start - 27 + 18, 58));
    eg_add_entity(app, block_demo_create(x_start - 27 + 18, 40));
    eg_add_entity(app, block_demo_create(x_start - 27 + 18, 22));

    // Vertical column 2
    eg_add_entity(app, block_demo_create(x_start - 27 + 198, 76));
    eg_add_entity(app, block_demo_create(x_start - 27 + 198, 58));
    eg_add_entity(app, block_demo_create(x_start - 27 + 198, 40));
    eg_add_entity(app, block_demo_create(x_start - 27 + 198, 22));

    // A hostile entity.
    eg_add_entity(app, henry_demo_create(x_start - 27 + 150, 22));

    // Ceiling of varying height.
    // eg_add_entity(app, block_demo_create(x_start - 27 + 90, 22));
    // eg_add_entity(app, block_demo_create(x_start - 27 + 108, 32));
    // eg_add_entity(app, block_demo_create(x_start - 27 + 126, 42));
    // eg_add_entity(app, block_demo_create(x_start - 27 + 144, 52));

    // TODO: push input handler outside of scene loader
    // Create the initial input handler and add it to the app.
    // eg_push_input_handler(app, root_input_handler);
}
