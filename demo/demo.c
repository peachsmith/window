#include "demo/demo.h"
#include "demo/input.h"
#include "demo/player.h"
#include "demo/entity.h"
#include "demo/block.h"
#include "demo/entity_types.h"

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

    // // TODO: move this after the input handler creation once the collision
    // // detection bugs are fixed.
    // // Create some blocks for demonstrating collision detection.
    // eg_entity *b0 = block_demo_create(50, 75);
    // eg_entity *b1 = block_demo_create_big(100, 75);
    // // eg_entity *b1 = block_demo_create(130, 100);
    // if (b0 == NULL || b1 == NULL)
    // {
    //     return 0;
    // }

    // eg_add_entity(app, b0);
    // eg_add_entity(app, b1);
    // // eg_add_entity(app, b2);

    // Create the player entity.
    eg_entity *player = player_demo_create();
    if (player == NULL)
    {
        fprintf(stderr, "failed to create player\n");
        return 0;
    }

    // Add a small block.
    eg_entity *b0 = block_demo_create(50, 75);
    if (b0 == NULL)
    {
        return 0;
    }
    eg_add_entity(app, b0);

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

    // Add a large block.
    eg_entity *b1 = block_demo_create_big(100, 75);
    if (b1 == NULL)
    {
        return 0;
    }
    eg_add_entity(app, b1);

    return 1;
}
