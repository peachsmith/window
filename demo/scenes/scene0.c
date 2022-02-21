#include "demo/scenes/scenes.h"
#include "demo/input/input.h"
#include "demo/entities/block.h"
#include "demo/entities/player.h"

/**
 * Create a basic map.
 * Given 18 blocks b0, b1, ..., b17, the layout should look like this:
 *     +--+
 *  b0 |  |
 *     +--+
 *     +--+
 *     |  |
 *     +--+
 *     +--+
 *     |  |
 *     +--+
 *     +--++--++--++--++--++--++--++--++--++--++--+
 *     |  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  |
 *     +--++--++--++--++--++--++--++--++--++--++--+
 *                                             +--+
 *                                             |  |
 *                                             +--+
 *                                             +--+
 *                                             |  |
 *                                             +--+
 *                                             +--+
 *                                             |  |
 *                                             +--+
 *                                             +--+
 *                                             |  | b17
 *                                             +--+
 */
void load_scene_0(eg_app *app)
{
    eg_entity *player;
    eg_callback root_callback;
    eg_input_handler *root_handler;

    // Starting position for building a sequence of blocks.
    int x_start = 10;
    int y_start = 30;

    // Add some of the blocks here and some of them later to prove that
    // the order in which entities are added doesn't affect their collision.

    // a vertical column of blocks
    eg_add_entity(app, block_demo_create(x_start, y_start));
    eg_add_entity(app, block_demo_create(x_start, y_start + 12));
    eg_add_entity(app, block_demo_create(x_start, y_start + 24));
    eg_add_entity(app, block_demo_create(x_start, y_start + 36));

    // TEMP: another column of blocks for debugging the floor-clipping bug.
    eg_add_entity(app, block_demo_create(x_start + 110, y_start));
    eg_add_entity(app, block_demo_create(x_start + 110, y_start + 12));
    eg_add_entity(app, block_demo_create(x_start + 110, y_start + 24));

    // the first part of the horizontal row of blocks
    eg_add_entity(app, block_demo_create(x_start + 12, y_start + 36));
    eg_add_entity(app, block_demo_create(x_start + 24, y_start + 36));
    eg_add_entity(app, block_demo_create(x_start + 36, y_start + 36));
    eg_add_entity(app, block_demo_create(x_start + 48, y_start + 36));
    eg_add_entity(app, block_demo_create(x_start + 60, y_start + 36));

    // Create the player entity and add it to the app.
    player = player_demo_create(80, 10);
    eg_add_entity(app, player);

    // Add the rest of the horizontal row.
    eg_add_entity(app, block_demo_create(x_start + 72, y_start + 36));
    eg_add_entity(app, block_demo_create(x_start + 84, y_start + 36));
    eg_add_entity(app, block_demo_create(x_start + 96, y_start + 36));
    eg_add_entity(app, block_demo_create(x_start + 108, y_start + 36));

    // Add another vertical column of blocks.
    eg_add_entity(app, block_demo_create(x_start + 120, y_start + 36));
    eg_add_entity(app, block_demo_create(x_start + 120, y_start + 48));
    eg_add_entity(app, block_demo_create(x_start + 120, y_start + 60));
    eg_add_entity(app, block_demo_create(x_start + 120, y_start + 72));
    eg_add_entity(app, block_demo_create(x_start + 120, y_start + 84));

    // A long, horizontal block.
    eg_add_entity(app, block_demo_create_long(x_start + 120, y_start + 96));

    // A long, horizontal block that can be passed through when holding
    // the down arrow key.
    eg_add_entity(app, throughblock_demo_create_long(x_start + 132, y_start + 36));

    // Create the initial input handler and add it to the app.
    root_callback = root_input_callback;
    root_handler = eg_create_input_handler(root_callback, player);
    eg_push_input_handler(app, root_handler);
}