#include "demo/demo.h"
#include "demo/input/input.h"
#include "demo/entities/player.h"
#include "demo/entities/block.h"
#include "demo/entities/entity_types.h"
#include "demo/collision/collision.h"
#include "demo/scenes/scenes.h"
#include "demo/util/util.h"
#include "demo/menu/menu.h"

#include <stdio.h>

/**
 * Implmentation of the update function.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 */
static void update(eg_app *app)
{
    // Handle input.
    if (app->input != NULL)
    {
        app->input->callback(app, app->input->target);
    }

    if (app->pause)
    {
        return;
    }

    // Handle collisions.
    demo_handle_collisions(app);

    // Update state.
    eg_entity *ent = app->entities;
    while (ent != NULL)
    {
        if (app->registry[ent->type].update != NULL)
        {
            app->registry[ent->type].update(app, ent);
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
        if (app->registry[ent->type].render != NULL)
        {
            app->registry[ent->type].render(app, ent);
        }
        ent = ent->next;
    }

    if (app->pause)
    {
        for (int i = 0; i < app->menu_count; i++)
        {
            eg_menu *m = app->menus[i];
            m->render(app, m);
        }
    }
}

int demo_prepare(eg_app *app)
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

    // Initialize menus.
    demo_init_menus(app);

    // Alegreya-VariableFont_wght
    // JetBrainsMonoNL-Regular
    if (!eg_load_font(app, "../Alegreya-VariableFont_wght.ttf", 16))
    {
        fprintf(stderr, "failed to load font\n");
    }

    // if (!eg_load_font(app, "../JetBrainsMonoNL-Regular.ttf", 16))
    // {
    //     fprintf(stderr, "failed to load font\n");
    // }

    // Register the entity types.
    player_demo_register(&reg[ENTITY_TYPE_PLAYER]);
    block_demo_register(&reg[ENTITY_TYPE_BLOCK]);
    block_demo_register_big(&reg[ENTITY_TYPE_BLOCK_BIG]);
    block_demo_register_long(&reg[ENTITY_TYPE_BLOCK_LONG]);
    throughblock_demo_register_long(&reg[ENTITY_TYPE_THROUGHBLOCK_LONG]);
    block_demo_register_moving(&reg[ENTITY_TYPE_BLOCK_MOVING]);
    block_demo_register_sloped(&reg[ENTITY_TYPE_BLOCK_SLOPE]);

    load_scene_0(app);

    return 1;
}
