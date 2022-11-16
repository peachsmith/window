#include "demo/demo.h"
#include "demo/input/input.h"
#include "demo/entities/entity_types.h"
#include "demo/entities/player.h"
#include "demo/entities/block.h"
#include "demo/entities/sign.h"
#include "demo/entities/jimbo.h"
#include "demo/entities/billy.h"
#include "demo/entities/henry.h"
#include "demo/entities/transition.h"
#include "demo/entities/pause_menu.h"
#include "demo/entities/fish_menu.h"
#include "demo/entities/info_menu.h"
#include "demo/entities/debug_menu.h"
#include "demo/entities/input_menu.h"
#include "demo/entities/demo_dialog.h"
#include "demo/entities/info_dialog.h"
#include "demo/entities/jimbo_dialog.h"
#include "demo/entities/sign_dialog.h"
#include "demo/collision/collision.h"
#include "demo/scenes/scenes.h"
#include "demo/menu/menu.h"
#include "demo/dialog/dialog.h"
#include "demo/font/font.h"
#include "demo/texture/texture.h"
#include "demo/audio/audio.h"
#include "demo/util/util.h"
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
        eg_entity *d = app->dialog_entities[app->dialog_count - 1];
        app->registry[d->type].update(app, d);
    }

    // Update menus.
    // TODO: update menus with other entities.
    if (app->menu_count > 0)
    {
        eg_entity *m = app->menu_entities[app->menu_count - 1];
        app->registry[m->type].update(app, m);
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
        int menu_flag = eg_check_flag(ent, ENTITY_FLAG_MENU);
        if (t.update != NULL)
        {
            if (app->pause)
            {
                if (pause_flag && !menu_flag)
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

    //------------------------------------------------------------------------
    // Sprite layer
    while (ent != NULL)
    {
        eg_entity_type t = app->registry[ent->type];
        int pause_flag = eg_check_flag(ent, ENTITY_FLAG_PAUSE);
        int menu_flag = eg_check_flag(ent, ENTITY_FLAG_MENU);
        if (t.render != NULL)
        {
            // We render all entities without the pause flag, regardless of
            // whether or not the application is paused.
            // However, entities that have the pause flag will only be
            // rendered if the application is paused.
            if (app->pause)
            {
                if (!menu_flag)
                {
                    t.render(app, ent);
                }
            }
            else if (!pause_flag && !menu_flag)
            {
                t.render(app, ent);
            }
        }
        ent = ent->next;
    }

    //------------------------------------------------------------------------
    // Menu layer
    if (app->pause)
    {
        for (int i = 0; i < app->menu_count; i++)
        {
            eg_entity *m = app->menu_entities[i];
            app->registry[m->type].render(app, m);
        }

        if (app->dialog_count > 0)
        {
            eg_entity *d = app->dialog_entities[app->dialog_count - 1];
            app->registry[d->type].render(app, d);
        }
    }

    //------------------------------------------------------------------------
    // Debug layer
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

    // platforms and other blocks
    block_demo_register(&reg[ENTITY_TYPE_BLOCK]);
    block_demo_register_big(&reg[ENTITY_TYPE_BLOCK_BIG]);
    block_demo_register_long(&reg[ENTITY_TYPE_BLOCK_LONG]);
    throughblock_demo_register_long(&reg[ENTITY_TYPE_THROUGHBLOCK_LONG]);
    block_demo_register_moving(&reg[ENTITY_TYPE_BLOCK_MOVING]);
    block_demo_register_sloped(&reg[ENTITY_TYPE_BLOCK_SLOPE]);

    // interactables
    sign_demo_register(&reg[ENTITY_TYPE_SIGN]);   // npc dialog
    jimbo_demo_register(&reg[ENTITY_TYPE_JIMBO]); // npc dialog
    billy_demo_register(&reg[ENTITY_TYPE_BILLY]); // npc movement
    henry_demo_register(&reg[ENTITY_TYPE_HENRY]); // hostile

    // scene transitions
    transition_demo_register(&reg[ENTITY_TYPE_TRANSITION]);

    // menus
    pause_menu_demo_register(&reg[ENTITY_TYPE_PAUSE_MENU]);
    fish_menu_demo_register(&reg[ENTITY_TYPE_FISH_MENU]);
    info_menu_demo_register(&reg[ENTITY_TYPE_INFO_MENU]);
    debug_menu_demo_register(&reg[ENTITY_TYPE_DEBUG_MENU]);
    input_menu_demo_register(&reg[ENTITY_TYPE_INPUT_MENU]);

    // dialogs
    demo_dialog_demo_register(&reg[ENTITY_TYPE_DEMO_DIALOG]);
    info_dialog_demo_register(&reg[ENTITY_TYPE_INFO_DIALOG]);
    jimbo_dialog_demo_register(&reg[ENTITY_TYPE_JIMBO_DIALOG]);
    sign_dialog_demo_register(&reg[ENTITY_TYPE_SIGN_DIALOG]);

    // Load the initial scene.
    load_scene_0(app);
    eg_push_input_handler(app, root_input_handler);

    // Play music
    // eg_play_sound(app, app->sounds[DEMO_SONG_FIELD]);

    return 1;
}
