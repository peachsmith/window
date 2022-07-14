#include "demo/scenes/scenes.h"
#include "demo/input/input.h"
#include "demo/entities/block.h"
#include "demo/entities/player.h"

void load_scene_1(eg_app *app)
{
    eg_entity *player;

    // Starting position for building a sequence of blocks.
    int x_start = 50;
    // int y_start = 0; // currently unused

    app->cam.x = 0;
    app->cam.y = 0;

    // moving platforms added before the player
    eg_add_entity(app, block_demo_create_moving(x_start - 63, 54, 0)); // vertical
    eg_add_entity(app, block_demo_create_moving(x_start + 253, 94, 1)); // horizontal

    eg_add_entity(app, block_demo_create(x_start - 27 + 18, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 36, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 54, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 72, 94));
    eg_add_entity(app, block_demo_create(x_start - 27 + 90, 94));

    eg_add_entity(app, block_demo_create(x_start - 27 + 36, 76)); // x = 59

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

    eg_add_entity(app, block_demo_create(x_start - 27 + 108, 76)); // x = 131

    // moving platforms added after the player
    // eg_add_entity(app, block_demo_create_moving(x_start - 63, 54, 0)); // vertical
    // eg_add_entity(app, block_demo_create_moving(x_start + 253, 94, 1)); // horizontal

    // Create the initial input handler and add it to the app.
    eg_push_input_handler(app, root_input_handler);
}
