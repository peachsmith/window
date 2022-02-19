#include "demo/demo.h"
#include "demo/input/input.h"
#include "demo/entities/player.h"
#include "demo/entities/block.h"
#include "demo/entities/entity_types.h"
#include "demo/collision/collision.h"
#include "demo/scenes/scenes.h"

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

    // TEMP: draw the camera boundaries.
    eg_point l0, l1, r0, r1, t0, t1, b0, b1;

    eg_set_color(app, EG_COLOR_MAUVE);

    // left boundary
    l0.x = app->cl;
    l0.y = 0;
    l1.x = app->cl;
    l1.y = app->screen_height;
    eg_draw_line(app, &l0, &l1);

    // right boundary
    r0.x = app->cr;
    r0.y = 0;
    r1.x = app->cr;
    r1.y = app->screen_height;
    eg_draw_line(app, &r0, &r1);

    eg_set_color(app, EG_COLOR_TEAL);

    // top boundary
    t0.x = 0;
    t0.y = app->ct;
    t1.x = app->screen_width;
    t1.y = app->ct;
    eg_draw_line(app, &t0, &t1);

    // bottom boundary
    b0.x = 0;
    b0.y = app->cb;
    b1.x = app->screen_width;
    b1.y = app->cb;
    eg_draw_line(app, &b0, &b1);

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
    block_demo_register(&reg[ENTITY_TYPE_BLOCK]);
    block_demo_register_big(&reg[ENTITY_TYPE_BLOCK_BIG]);
    block_demo_register_long(&reg[ENTITY_TYPE_BLOCK_LONG]);

    load_scene_0(app);

    return 1;
}
