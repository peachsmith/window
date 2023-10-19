#include "demo/demo.h"

#include "demo/assets.h"

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

#include "demo/input/input.h"
#include "demo/scenes/scenes.h"
#include "demo/util/sprite.h"
#include "demo/util/overlay.h"

#include "common/util.h"
#include "common/collision.h"
#include "common/dialog.h"

static cr_entity entities[DEMO_MAX_ENTITIES];
static cr_func input_handlers[DEMO_MAX_INPUT_HANDLERS];
static cr_entity *menus[DEMO_MAX_MENUS];
static cr_entity *dialogs[DEMO_MAX_DIALOGS];
static cr_entity *overlays[DEMO_MAX_OVERLAYS];
static cr_texture *textures[DEMO_MAX_TEXTURES];
static cr_font *fonts[DEMO_MAX_FONTS];
static cr_sound *sounds[DEMO_MAX_SOUNDS];
static cr_entity_type entity_types[ENTITY_TYPE_MAX];

static cr_entity *entity_handles[DEMO_MAX_ENTITY_HANDLES];
static cr_extension extension;

static int default_get_x_vel(cr_entity *e)
{
    return e->x_vel;
}

static int default_get_y_vel(cr_entity *e)
{
    return e->y_vel;
}

static void default_input(cr_app *app)
{
}

/**
 * Implmentation of the update function.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 */
static void update(cr_app *app)
{
    if (app->time == TIMING_DELTA)
    {
        if (app->frame_check < 1)
        {
            return;
        }
    }

    app->ticks++;

    // Handle input.
    if (app->input != NULL && app->input_count > 0)
    {
        app->input[app->input_count - 1](app);
    }

    // update dialogs
    if (app->dialog_count > 0)
    {
        cr_entity *d = app->dialogs[app->dialog_count - 1];
        app->entity_types[d->type].update(app, d);
    }

    // update menus
    if (app->menu_count > 0)
    {
        cr_entity *m = app->menus[app->menu_count - 1];
        app->entity_types[m->type].update(app, m);
    }

    // update overlays
    if (app->overlay_count > 0)
    {
        cr_entity *o = app->overlays[app->overlay_count - 1];
        app->entity_types[o->type].update(app, o);
    }

    // handle collisions
    if (!app->pause)
    {
        common_handle_collisions(app);
    }

    // main update loop
    for (int i = 0; i < app->entity_cap; i++)
    {
        cr_entity *ent = &(app->entities[i]);
        cr_entity_type t = app->entity_types[ent->type];
        int pause_flag = cr_check_flag(ent, ENTITY_FLAG_PAUSE);
        int menu_flag = cr_check_flag(ent, ENTITY_FLAG_MENU);
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

    // Here is where scene updates happen.
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
 *   cr_app* - a pointer to an app struct
 */
static void draw(cr_app *app)
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
        cr_entity *ent = &(app->entities[i]);
        cr_entity_type t = app->entity_types[ent->type];
        int pause_flag = cr_check_flag(ent, ENTITY_FLAG_PAUSE);
        int menu_flag = cr_check_flag(ent, ENTITY_FLAG_MENU);
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
            cr_entity *m = app->menus[i];
            app->entity_types[m->type].render(app, m);
        }

        // only render the dialog on the top of the stack
        if (app->dialog_count > 0)
        {
            cr_entity *d = app->dialogs[app->dialog_count - 1];
            app->entity_types[d->type].render(app, d);
        }

        // render screen overlay effects
        for (int i = 0; i < app->overlay_count; i++)
        {
            cr_entity *o = app->overlays[i];
            app->entity_types[o->type].render(app, o);
        }
    }

    //------------------------------------------------------------------------
    // debug layer

    if (app->debug.camera)
    {
        util_draw_camera(app);
    }

    if (app->debug.overlay)
    {
        demo_draw_overlay(app);
    }
}

int demo_init_app(cr_app *app)
{
    cr_set_title(app, "Demo");

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
        entity_types[i].control = 0;
        entity_types[i].spur = 0;
        entity_types[i].move = 0;
        entity_types[i].slope = 0;
    }

    // default values of entities
    for (int i = 0; i < DEMO_MAX_ENTITIES; i++)
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
        entities[i].scroll_x = 0;
        entities[i].scroll_y = 0;
    }

    app->entity_cap = DEMO_MAX_ENTITIES;
    app->entities = entities;
    app->input = &(input_handlers[0]);
    app->menus = &(menus[0]);
    app->dialogs = &(dialogs[0]);
    app->overlays = &(overlays[0]);
    app->textures = &(textures[0]);
    app->fonts = &(fonts[0]);
    app->sounds = &(sounds[0]);
    app->entity_types = entity_types;
    app->update = update;
    app->draw = draw;

    // asset loading
    if (!demo_load_all_assets(app))
    {
        return 0;
    }

    // extension initialization
    extension.entity_handles = entity_handles;
    app->extension = &extension;

    // entity type registration
    demo_register_player(&(app->entity_types[ENTITY_TYPE_PLAYER]));
    demo_register_block(&(app->entity_types[ENTITY_TYPE_BLOCK]));
    demo_register_big_block(&(app->entity_types[ENTITY_TYPE_BLOCK_BIG]));
    demo_register_long_block(&(app->entity_types[ENTITY_TYPE_BLOCK_LONG]));
    demo_register_long_throughblock(&(app->entity_types[ENTITY_TYPE_THROUGHBLOCK_LONG]));
    demo_register_moving_block(&(app->entity_types[ENTITY_TYPE_BLOCK_MOVING]));
    demo_register_sloped_block(&(app->entity_types[ENTITY_TYPE_BLOCK_SLOPE]));
    demo_register_sign(&(app->entity_types[ENTITY_TYPE_SIGN]));
    demo_register_jimbo(&(app->entity_types[ENTITY_TYPE_JIMBO]));
    demo_register_billy(&(app->entity_types[ENTITY_TYPE_BILLY]));
    demo_register_henry(&(app->entity_types[ENTITY_TYPE_HENRY]));
    demo_register_transition(&(app->entity_types[ENTITY_TYPE_TRANSITION]));
    demo_register_pause_menu(&(app->entity_types[ENTITY_TYPE_PAUSE_MENU]));
    demo_register_fish_menu(&(app->entity_types[ENTITY_TYPE_FISH_MENU]));
    demo_register_info_menu(&(app->entity_types[ENTITY_TYPE_INFO_MENU]));
    demo_register_debug_menu(&(app->entity_types[ENTITY_TYPE_DEBUG_MENU]));
    demo_register_scene_menu(&(app->entity_types[ENTITY_TYPE_SCENE_MENU]));
    demo_register_input_menu(&(app->entity_types[ENTITY_TYPE_INPUT_MENU]));
    demo_dialog_demo_register(&(app->entity_types[ENTITY_TYPE_DEMO_DIALOG]));
    demo_register_info_dialog(&(app->entity_types[ENTITY_TYPE_INFO_DIALOG]));
    demo_register_jimbo_dialog(&(app->entity_types[ENTITY_TYPE_JIMBO_DIALOG]));
    demo_register_sign_dialog(&(app->entity_types[ENTITY_TYPE_SIGN_DIALOG]));
    demo_register_fireball(&(app->entity_types[ENTITY_TYPE_FIREBALL]));

    // push the default input handler
    cr_push_input_handler(app, default_input);

    // Load the initial scene.
    demo_load_scene_0(app);
    cr_push_input_handler(app, demo_root_input);

    // Play music
    // cr_play_sound(app, app->sounds[DEMO_SONG_FIELD]);

    // Seed random number generation.
    time_t t;
    srand((unsigned int)(time(&t) & INT_MAX));

    return 1;
}
