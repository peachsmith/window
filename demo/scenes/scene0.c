#include "demo/scenes/scenes.h"
#include "demo/input/input.h"
#include "demo/entities/block.h"
#include "demo/entities/player.h"
#include "demo/entities/sign.h"

#include <stdio.h>

void load_scene_0(eg_app *app)
{
    eg_entity *player;

    // Starting position for building a sequence of blocks.
    int x_start = 10;
    int y_start = 30;

    app->cam.x = 0;
    app->cam.y = 0;

    eg_add_entity(app, block_demo_create(x_start - 27 + 18, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 36, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 54, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 72, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 90, 94));

    // // Vertically moving platform (added before player).
    eg_add_entity(app, block_demo_create_moving(570, y_start - 32, 0));

    // Create the player entity and add it to the app.
    player = player_demo_create(150, 10);
    eg_add_entity(app, player);

    // TEMP: remove this once searchable entities are implemented.
    app->player = player;

    // Add the rest of the horizontal row.
    eg_add_entity(app, block_demo_create(x_start - 27 + 108, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 126, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 144, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 162, 94));

    // A long, horizontal block.
    // eg_add_entity(app, block_demo_create_long(230, 62));
    eg_add_entity(app, block_demo_create_sloped(209, 62, 2)); // horizontal slope

    // A long, horizontal block that can be passed through when holding
    // the down arrow key.
    eg_add_entity(app, throughblock_demo_create_long(640, y_start - 32));

    // Horizontally moving platform (added after player).
    eg_add_entity(app, block_demo_create_moving(740, y_start + 32, 1));

    // Sloped block
    eg_add_entity(app, block_demo_create_sloped(89, y_start + 32, 1)); // 32
    eg_add_entity(app, block_demo_create_sloped(330, y_start + 32, 0));
    eg_add_entity(app, block_demo_create_sloped(450, y_start + 32, 1));
    eg_add_entity(app, block_demo_create(x_start - 27 + 453, 93)); // solid ground to allow transition between sloped blocks

    eg_add_entity(app, block_demo_create_sloped(x_start - 130, 94, 2)); // horizontal slope
    eg_add_entity(app, block_demo_create_sloped(x_start - 250, 63, 0));
    eg_add_entity(app, block_demo_create_sloped(x_start - 370, 31, 0));
    eg_add_entity(app, block_demo_create(x_start - 136, 94)); // solid ground to allow transition between sloped blocks
    eg_add_entity(app, throughblock_demo_create_long(x_start - 370, 14));

    eg_add_entity(app, sign_demo_create(x_start - 27 + 54, 76)); // an interactable sign

    // Create the initial input handler and add it to the app.
    eg_push_input_handler(app, root_input_handler);
}