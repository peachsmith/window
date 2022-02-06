#include "demo/demo.h"
#include "demo/input/input.h"
#include "demo/entities/player.h"
#include "demo/entities/qwe.h"
#include "demo/entities/block.h"
#include "demo/entities/entity_types.h"

#include <stdio.h>

int prepare(eg_app *app)
{
    // Create the entity registry.
    eg_entity_type *reg = eg_create_registry(ENTITY_TYPE_MAX);
    if (reg == NULL)
    {
        fprintf(stderr, "failed to create entity registry\n");
        return 0;
    }
    app->registry = reg;

    // Register the entity types.
    player_demo_register(&reg[ENTITY_TYPE_PLAYER]);
    entity_demo_register_q(&reg[ENTITY_TYPE_Q]);
    entity_demo_register_w(&reg[ENTITY_TYPE_W]);
    entity_demo_register_e(&reg[ENTITY_TYPE_E]);
    block_demo_register(&reg[ENTITY_TYPE_BLOCK]);
    block_demo_register_big(&reg[ENTITY_TYPE_BIG_BLOCK]);

    // A single, large block for the early stages of collision detection.
    eg_add_entity(app, block_demo_create_big(90, 75));

    // Create a basic map.
    // Given 11 blocks b0, b1, ..., b10, the layout should look like this:
    // Note that each block is 15 x 15 pixels.
    //     +--+
    //  b0 |  |
    //     +--+
    //     +--+
    //     |  |
    //     +--+
    //     +--+
    //     |  |
    //     +--+
    //     +--++--++--++--++--++--++--++--+
    //     |  ||  ||  ||  ||  ||  ||  ||  |
    //     +--++--++--++--++--++--++--++--+
    //                                  b10
    //

    // Create the blocks.
    // eg_entity *blocks[11];

    // int x_start = 30;
    // int y_start = 40;

    // vertical column of blocks
    // blocks[0] = block_demo_create(x_start, y_start);
    // blocks[1] = block_demo_create(x_start, y_start + 15);
    // blocks[2] = block_demo_create(x_start, y_start + 30);
    // blocks[3] = block_demo_create(x_start, y_start + 45);

    // horizontal row of blocks
    // blocks[4] = block_demo_create(x_start + 15, y_start + 45);
    // blocks[5] = block_demo_create(x_start + 30, y_start + 45);
    // blocks[6] = block_demo_create(x_start + 45, y_start + 45);
    // blocks[7] = block_demo_create(x_start + 60, y_start + 45);
    // blocks[8] = block_demo_create(x_start + 75, y_start + 45);
    // blocks[9] = block_demo_create(x_start + 90, y_start + 45);
    // blocks[10] = block_demo_create(x_start + 105, y_start + 45);

    // Add some of the blocks here and some of them later to prove that
    // the order in which entities are added doesn't affect their collision.
    // eg_add_entity(app, blocks[0]);
    // eg_add_entity(app, blocks[1]);
    // eg_add_entity(app, blocks[2]);
    // eg_add_entity(app, blocks[3]);
    // eg_add_entity(app, blocks[4]);

    //------------------------------------------
    // BEGIN player an input handler

    // Create the player entity.
    eg_entity *player = player_demo_create();
    if (player == NULL)
    {
        fprintf(stderr, "failed to create player\n");
        return 0;
    }

    player->x_pos = 80;
    player->y_pos = 30;

    // Add the player entity to the app.
    eg_add_entity(app, player);

    // Create the initial input handler.
    eg_input_handler *root_handler =
        eg_create_input_handler(root_input_callback);

    if (root_handler == NULL)
    {
        fprintf(stderr, "failed to create root input handler\n");
        return 0;
    }

    // Set the player entity as the root input handler's target.
    root_handler->target = player;

    // Add the root input handler to the app.
    eg_push_input_handler(app, root_handler);

    // END player an input handler
    //------------------------------------------

    // eg_add_entity(app, blocks[5]);
    // eg_add_entity(app, blocks[6]);
    // eg_add_entity(app, blocks[7]);
    // eg_add_entity(app, blocks[8]);
    // eg_add_entity(app, blocks[9]);
    // eg_add_entity(app, blocks[10]);

    return 1;
}
