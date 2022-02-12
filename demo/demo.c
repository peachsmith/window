#include "demo/demo.h"
#include "demo/input/input.h"
#include "demo/entities/player.h"
#include "demo/entities/block.h"
#include "demo/entities/qwe.h"
#include "demo/entities/entity_types.h"
#include "demo/collision/collision.h"
#include "demo/scenes/scenes.h"
#include "colors.h"

#include <stdio.h>

/**
 * Implmentation of the update function.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 */
static void update(eg_app *app)
{
    // Clear the screen.
    eg_clear_screen(app);

    // Handle input.
    if (app->input != NULL)
    {
        app->input->callback(app, app->input->target);
    }

    // Handle collisions.
    handle_collisions(app);

    // Update state.
    eg_entity *ent = app->entities;
    while (ent != NULL)
    {
        if (app->registry[ent->id].update != NULL)
        {
            app->registry[ent->id].update(app, ent);
        }
        ent = ent->next;
    }
}

/**
 * Implmentation of the draw function.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 */
static void draw(eg_app *app)
{
    // Render the entities.
    eg_entity *ent = app->entities;
    while (ent != NULL)
    {
        if (app->registry[ent->id].render != NULL)
        {
            app->registry[ent->id].render(app, ent);
        }
        ent = ent->next;
    }

    // Show the contents of the current frame.
    eg_render_screen(app);
}

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
    app->update = update;
    app->draw = draw;

    // Register the entity types.
    player_demo_register(&reg[ENTITY_TYPE_PLAYER]);
    entity_demo_register_q(&reg[ENTITY_TYPE_Q]);
    entity_demo_register_w(&reg[ENTITY_TYPE_W]);
    entity_demo_register_e(&reg[ENTITY_TYPE_E]);
    block_demo_register(&reg[ENTITY_TYPE_BLOCK]);
    block_demo_register_big(&reg[ENTITY_TYPE_BIG_BLOCK]);

    load_scene_0(app);

    return 1;
}
