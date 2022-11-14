#include "demo/demo.h"
#include "demo/input/input.h"
#include "demo/entities/player.h"
#include "demo/entities/block.h"
#include "demo/entities/sign.h"
#include "demo/entities/jimbo.h"
#include "demo/entities/billy.h"
#include "demo/entities/henry.h"
#include "demo/entities/transition.h"
#include "demo/entities/entity_types.h"
#include "demo/collision/collision.h"
#include "demo/scenes/scenes.h"
#include "demo/util/util.h"
#include "demo/menu/menu.h"
#include "demo/dialog/dialog.h"
#include "demo/font/font.h"
#include "demo/texture/texture.h"
#include "demo/audio/audio.h"
#include "demo/util/sprite.h"
#include "demo/util/overlay.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * Implmentation of the update function.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 */
static void update(eg_app *app)
{
    // Handle input.
    if (app->input != NULL && app->input_count > 0)
    {
        app->input[app->input_count - 1](app);
    }

    // Update dialogs.
    if (app->dialog_count > 0)
    {
        eg_dialog *d = app->dialogs[app->dialog_count - 1];
        d->update(app, d);
    }

    // Update menus.
    if (app->menu_count > 0)
    {
        eg_menu *m = app->menus[app->menu_count - 1];
        if (m->update != NULL)
        {
            m->update(app, m);
        }
    }

    // Handle collisions.
    if (!app->pause)
    {
        demo_handle_collisions(app);
    }

    // Update state.
    eg_entity *ent = app->entities;
    while (ent != NULL)
    {
        eg_entity_type t = app->registry[ent->type];
        int pause_flag = eg_check_flag(ent, ENTITY_FLAG_PAUSE);
        if (t.update != NULL)
        {
            if (app->pause)
            {
                if (pause_flag)
                {
                    t.update(app, ent);
                }
            }
            else if (!pause_flag)
            {
                t.update(app, ent);
            }
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
    // Render background
    // sprite_draw_background(app, 0);

    // Main render loop.
    eg_entity *ent = app->entities;
    while (ent != NULL)
    {
        if (app->registry[ent->type].render != NULL)
        {
            app->registry[ent->type].render(app, ent);
        }
        ent = ent->next;
    }

    // TODO: move all rendering into the main render loop.
    if (app->pause)
    {
        for (int i = 0; i < app->menu_count; i++)
        {
            eg_menu *m = app->menus[i];
            m->render(app, m);
        }

        if (app->dialog_count > 0)
        {
            eg_dialog *d = app->dialogs[app->dialog_count - 1];
            d->render(app, d);
        }
    }

    if (app->debug.overlay)
    {
        debug_draw_overlay(app);
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

    // Initialize counters.
    int max_counters = 100;
    int *counters = (int *)malloc(sizeof(int) * max_counters);
    if (counters == NULL)
    {
        return 0;
    }
    app->counters = counters;
    app->counter_count = max_counters;
    for (int i = 0; i < app->counter_count; i++)
    {
        app->counters[i] = -1;
    }

    // Initialize tetxures.
    if (!demo_init_textures(app))
    {
        return 0;
    }

    // Initialize fonts.
    if (!demo_init_fonts(app))
    {
        return 0;
    }

    // Initialize audio
    if (!demo_init_audio(app))
    {
        return 0;
    }

    // Initialize menus.
    if (!demo_init_menus(app))
    {
        return 0;
    }

    // Initialize dialogs.
    demo_init_dialogs(app);

    // Initialize input.
    demo_init_input(app);

    // Register the entity types.
    player_demo_register(&reg[ENTITY_TYPE_PLAYER]);
    block_demo_register(&reg[ENTITY_TYPE_BLOCK]);
    block_demo_register_big(&reg[ENTITY_TYPE_BLOCK_BIG]);
    block_demo_register_long(&reg[ENTITY_TYPE_BLOCK_LONG]);
    throughblock_demo_register_long(&reg[ENTITY_TYPE_THROUGHBLOCK_LONG]);
    block_demo_register_moving(&reg[ENTITY_TYPE_BLOCK_MOVING]);
    block_demo_register_sloped(&reg[ENTITY_TYPE_BLOCK_SLOPE]);
    sign_demo_register(&reg[ENTITY_TYPE_SIGN]);
    jimbo_demo_register(&reg[ENTITY_TYPE_JIMBO]);
    billy_demo_register(&reg[ENTITY_TYPE_BILLY]);
    henry_demo_register(&reg[ENTITY_TYPE_HENRY]);
    transition_demo_register(&reg[ENTITY_TYPE_TRANSITION]);

    // load_scene_0(app);
    load_scene_3(app);
    eg_push_input_handler(app, root_input_handler);

    // Play music
    // eg_play_sound(app, app->sounds[DEMO_SONG_FIELD]);

    return 1;
}
