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
#include "demo/entities/scene_menu.h"
#include "demo/entities/input_menu.h"
#include "demo/entities/demo_dialog.h"
#include "demo/entities/info_dialog.h"
#include "demo/entities/jimbo_dialog.h"
#include "demo/entities/sign_dialog.h"
#include "demo/entities/fireball.h"
#include "demo/entities/hud.h"
#include "demo/entities/forest.h"
#include "demo/entities/corgi.h"
#include "demo/entities/note.h"
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

// Capacity of entity array
// This must match the value of the app->entity_cap field.
#define ENTITY_MAX 256

// array of entities that will replace the linked list.
// TODO: determine a better time and place to allocate this.
static eg_entity entities[ENTITY_MAX];

// array of entity types
static eg_entity_type entity_types[ENTITY_TYPE_MAX];

// maximum number of overlays
#define MAX_OVERLAYS 10

// list of active overlays entities
static eg_entity *overlays[MAX_OVERLAYS];

static int default_get_x_vel(eg_entity *e)
{
    return e->x_vel;
}

static int default_get_y_vel(eg_entity *e)
{
    return e->y_vel;
}

/**
 * Implmentation of the update function.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 */
static void update(eg_app *app)
{
    if (app->time == TIMING_DELTA)
    {
        if (app->frame_check < 1)
        {
            return;
        }
    }

    // Handle input.
    if (app->input != NULL && app->input_count > 0)
    {
        app->input[app->input_count - 1](app);
    }

    // update dialogs
    if (app->dialog_count > 0)
    {
        eg_entity *d = app->dialogs[app->dialog_count - 1];
        app->entity_types[d->type].update(app, d);
    }

    // update menus
    if (app->menu_count > 0)
    {
        eg_entity *m = app->menus[app->menu_count - 1];
        app->entity_types[m->type].update(app, m);
    }

    // update overlays
    if (app->overlay_count > 0)
    {
        eg_entity *o = app->overlays[app->overlay_count - 1];
        app->entity_types[o->type].update(app, o);
    }

    // handle collisions
    if (!app->pause)
    {
        demo_handle_collisions(app);
    }

    // main update loop
    for (int i = 0; i < app->entity_cap; i++)
    {
        eg_entity *ent = &(app->entities[i]);
        eg_entity_type t = app->entity_types[ent->type];
        int pause_flag = eg_check_flag(ent, ENTITY_FLAG_PAUSE);
        int menu_flag = eg_check_flag(ent, ENTITY_FLAG_MENU);
        if (ent->present && t.update != NULL)
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
    }
}

/**
 * Implmentation of the draw function.
 * This function renders graphics to the screen in four layers:
 * 1. background
 * 2. foreground
 * 3. menu
 * 4. debug
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 */
static void draw(eg_app *app)
{
    if (app->time == TIMING_DELTA)
    {
        if (app->frame_check < 1)
        {
            return;
        }
    }

    //------------------------------------------------------------------------
    // background layer
    sprite_draw_background(app, 0);

    //------------------------------------------------------------------------
    // foreground layer
    for (int i = 0; i < app->entity_cap; i++)
    {
        eg_entity *ent = &(app->entities[i]);
        eg_entity_type t = app->entity_types[ent->type];
        int pause_flag = eg_check_flag(ent, ENTITY_FLAG_PAUSE);
        int menu_flag = eg_check_flag(ent, ENTITY_FLAG_MENU);
        if (ent->present && t.render != NULL)
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
    }

    //------------------------------------------------------------------------
    // menu layer
    if (app->pause)
    {
        // render all open menus
        for (int i = 0; i < app->menu_count; i++)
        {
            eg_entity *m = app->menus[i];
            app->entity_types[m->type].render(app, m);
        }

        // only render the dialog on the top of the stack
        if (app->dialog_count > 0)
        {
            eg_entity *d = app->dialogs[app->dialog_count - 1];
            app->entity_types[d->type].render(app, d);
        }

        // render screen overlay effects
        for (int i = 0; i < app->overlay_count; i++)
        {
            eg_entity *o = app->overlays[i];
            app->entity_types[o->type].render(app, o);
        }
    }

    //------------------------------------------------------------------------
    // debug layer

    if (app->debug.camera)
    {
        demo_draw_camera(app);
    }

    if (app->debug.overlay)
    {
        demo_draw_overlay(app);
    }
}

int demo_prepare(eg_app *app)
{
    // default values for entity types
    for (int i = 0; i < ENTITY_TYPE_MAX; i++)
    {
        entity_types[i].id = 0;
        entity_types[i].width = 0;
        entity_types[i].height = 0;
        entity_types[i].render = NULL;
        entity_types[i].update = NULL;
        entity_types[i].advance = NULL;
        entity_types[i].collide = NULL;
        entity_types[i].get_x_vel = default_get_x_vel;
        entity_types[i].get_y_vel = default_get_y_vel;
        entity_types[i].interactable = 0;
        entity_types[i].interact = NULL;
    }

    // default values of entities
    for (int i = 0; i < app->entity_cap; i++)
    {
        entities[i].present = 0;
        entities[i].type = 0;
        entities[i].x_pos = 0;
        entities[i].y_pos = 0;
        entities[i].x_vel = 0;
        entities[i].y_vel = 0;
        entities[i].x_acc = 0;
        entities[i].y_acc = 0;
        entities[i].x_t = 0;
        entities[i].y_t = 0;
        entities[i].flags = 0;
        entities[i].data = 0;
        entities[i].animation_ticks = 0;
        entities[i].ticks = 0;
        entities[i].iframes = 0;
        entities[i].carrier = NULL;
        entities[i].text = NULL;
        entities[i].text_len = 0;
        entities[i].tick_limit = 0;
        entities[i].result = 0;
        entities[i].cursor_x = 0;
        entities[i].cursor_y = 0;
    }

    app->entities = entities;
    app->entity_types = entity_types;
    app->update = update;
    app->draw = draw;

    // delta time (under construction)

    // initialize tetxures
    if (!demo_init_textures(app))
    {
        return 0;
    }

    // initialize fonts
    if (!demo_init_fonts(app))
    {
        return 0;
    }

    // initialize audio
    if (!demo_init_audio(app))
    {
        return 0;
    }

    // initialize menus
    if (!demo_init_menus(app))
    {
        return 0;
    }

    // initialize overlays
    app->overlays = &(overlays[0]);
    app->overlay_count = 0;

    // Initialize dialogs.
    demo_init_dialogs(app);

    // Initialize input.
    demo_init_input(app);

    // Register the entity types.
    player_demo_register(&(app->entity_types[ENTITY_TYPE_PLAYER]));
    corgi_demo_register(&(app->entity_types[ENTITY_TYPE_CORGI]));

    // platforms and other blocks
    block_demo_register(&(app->entity_types[ENTITY_TYPE_BLOCK]));
    block_demo_register_big(&(app->entity_types[ENTITY_TYPE_BLOCK_BIG]));
    block_demo_register_long(&(app->entity_types[ENTITY_TYPE_BLOCK_LONG]));
    throughblock_demo_register_long(&(app->entity_types[ENTITY_TYPE_THROUGHBLOCK_LONG]));
    block_demo_register_moving(&(app->entity_types[ENTITY_TYPE_BLOCK_MOVING]));
    block_demo_register_sloped(&(app->entity_types[ENTITY_TYPE_BLOCK_SLOPE]));
    block_demo_register_floor(&(app->entity_types[ENTITY_TYPE_FLOOR]));
    block_demo_register_wall(&(app->entity_types[ENTITY_TYPE_WALL]));

    // interactables
    sign_demo_register(&(app->entity_types[ENTITY_TYPE_SIGN]));   // npc dialog
    jimbo_demo_register(&(app->entity_types[ENTITY_TYPE_JIMBO])); // npc dialog
    billy_demo_register(&(app->entity_types[ENTITY_TYPE_BILLY])); // npc movement
    henry_demo_register(&(app->entity_types[ENTITY_TYPE_HENRY])); // hostile

    // scene transitions
    transition_demo_register(&(app->entity_types[ENTITY_TYPE_TRANSITION]));

    // menus
    pause_menu_demo_register(&(app->entity_types[ENTITY_TYPE_PAUSE_MENU]));
    fish_menu_demo_register(&(app->entity_types[ENTITY_TYPE_FISH_MENU]));
    info_menu_demo_register(&(app->entity_types[ENTITY_TYPE_INFO_MENU]));
    debug_menu_demo_register(&(app->entity_types[ENTITY_TYPE_DEBUG_MENU]));
    scene_menu_demo_register(&(app->entity_types[ENTITY_TYPE_SCENE_MENU]));
    input_menu_demo_register(&(app->entity_types[ENTITY_TYPE_INPUT_MENU]));

    // heads up display
    hud_demo_register(&(app->entity_types[ENTITY_TYPE_HUD]));

    // scenery
    forest_demo_register(&(app->entity_types[ENTITY_TYPE_FOREST]));

    // dialogs
    demo_dialog_demo_register(&(app->entity_types[ENTITY_TYPE_DEMO_DIALOG]));
    info_dialog_demo_register(&(app->entity_types[ENTITY_TYPE_INFO_DIALOG]));
    jimbo_dialog_demo_register(&(app->entity_types[ENTITY_TYPE_JIMBO_DIALOG]));
    sign_dialog_demo_register(&(app->entity_types[ENTITY_TYPE_SIGN_DIALOG]));

    // projectiles
    fireball_demo_register(&(app->entity_types[ENTITY_TYPE_FIREBALL]));
    note_demo_register(&(app->entity_types[ENTITY_TYPE_NOTE]));

    eg_push_input_handler(app, empty_input_handler);

    // Load the initial scene.
    load_forest_scene(app);
    eg_push_input_handler(app, tns_root_input_handler);

    // Play music
    // eg_play_sound(app, app->sounds[DEMO_SONG_FIELD]);

    return 1;
}
