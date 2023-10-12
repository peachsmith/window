#include "tootnsploot/tootnsploot.h"

#include "tootnsploot/input/input.h"

#include "tootnsploot/entities/entity_types.h"
#include "tootnsploot/entities/transition.h"
#include "tootnsploot/entities/hud.h"
#include "tootnsploot/entities/forest.h"
#include "tootnsploot/entities/corgi.h"
#include "tootnsploot/entities/note.h"
#include "tootnsploot/entities/critter.h"
#include "tootnsploot/entities/floor.h"
#include "tootnsploot/entities/wall.h"
#include "tootnsploot/entities/main_menu.h"
#include "tootnsploot/entities/controls_menu.h"
#include "tootnsploot/entities/characters_menu.h"
#include "tootnsploot/entities/pause_menu.h"

#include "tootnsploot/scenes/scenes.h"

#include "tootnsploot/util/sprite.h"
#include "tootnsploot/util/overlay.h"

#include "common/util.h"
#include "common/collision.h"
#include "common/menu.h"
#include "common/dialog.h"
#include "common/texture.h"
#include "common/font.h"
#include "common/audio.h"

// Capacity of entity array
// This must match the value of the app->entity_cap field.
#define ENTITY_MAX 256

// array of entities that will replace the linked list.
// TODO: determine a better time and place to allocate this.
static cr_entity entities[ENTITY_MAX];

// array of entity types
static cr_entity_type entity_types[ENTITY_TYPE_MAX];

// 0 notes
// 1 critters
// 2 score
// [3:10] critter slots (8 slots)
static int counters[TNS_COUNTER_MAX];

// maximum number of overlays
#define MAX_OVERLAYS 10

// list of active overlays entities
static cr_entity *overlays[MAX_OVERLAYS];

static int default_get_x_vel(cr_entity *e)
{
    return e->x_vel;
}

static int default_get_y_vel(cr_entity *e)
{
    return e->y_vel;
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

    // scene behavior
    // TODO: move scene behavior functions into more appropriate locations.
    if (!app->pause && app->scene == TNS_SCENE_FOREST)
    {
        if (!(app->ticks % 120) && app->counters[TNS_COUNTER_CRITTERS] < 4)
        {
            // Generate random number from 0 to 7. This is the critter slot.
            // If the slot is unoccupied, then create a critter and mark the
            // slot as occupied.
            int slot = rand() % 8;
            if (!app->counters[slot + TNS_COUNTER_CRITTER_SLOT_OFFSET])
            {
                cr_entity *critter = tns_create_critter(app, 4 + slot * 20 + slot * 10, 25);
                app->counters[TNS_COUNTER_CRITTERS]++;
                app->counters[slot + TNS_COUNTER_CRITTER_SLOT_OFFSET] = 1;
                critter->data = slot + TNS_COUNTER_CRITTER_SLOT_OFFSET;
                if (slot > 3)
                {
                    cr_set_flag(critter, ENTITY_FLAG_MIRROR);
                }
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
        tns_draw_overlay(app);
    }
}

int tns_prepare(cr_app *app)
{
    cr_set_title(app, "Toot n Sploot");

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
        entity_types[i].slope = 0;
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

    // initialize counters
    for (int i = 0; i < TNS_COUNTER_MAX; i++)
    {
        counters[i] = 0;
    }
    app->counters = counters;

    // initialize overlays
    app->overlays = &(overlays[0]);
    app->overlay_count = 0;

    app->entities = entities;
    app->entity_types = entity_types;
    app->update = update;
    app->draw = draw;

    // initialize tetxures
    if (!common_init_textures(app))
    {
        return 0;
    }
    if (cr_load_texture(app, "assets/images/ui.png") == NULL)
    {
        fprintf(stderr, "failed to load ui image\n");
        return 0;
    }
    if (cr_load_texture(app, "assets/images/tootnsploot.png") == NULL)
    {
        fprintf(stderr, "failed to load Toot n Sploot sprite sheet\n");
        return 0;
    }
    if (cr_load_texture(app, "assets/images/title.png") == NULL)
    {
        fprintf(stderr, "failed to load Toot n Sploot sprite sheet\n");
        return 0;
    }

    // initialize fonts
    if (!common_init_fonts(app))
    {
        return 0;
    }

    // initialize audio
    if (!common_init_audio(app))
    {
        return 0;
    }
    if (cr_load_sound(app, "assets/audio/confirmation_002.ogg", AUDIO_TYPE_SOUND_EFFECT) == NULL)
    {
        fprintf(stderr, "failed to load confirmation_002.ogg\n");
        return 0;
    }

    // initialize menus
    if (!common_init_menus(app))
    {
        return 0;
    }

    // Initialize input.
    tns_init_input(app);

    tns_register_transition(&(app->entity_types[ENTITY_TYPE_TRANSITION]));
    tns_register_hud(&(app->entity_types[ENTITY_TYPE_HUD]));
    tns_register_note(&(app->entity_types[ENTITY_TYPE_NOTE]));
    tns_register_critter(&(app->entity_types[ENTITY_TYPE_CRITTER]));
    tns_register_corgi(&(app->entity_types[ENTITY_TYPE_CORGI]));
    tns_register_floor(&(app->entity_types[ENTITY_TYPE_FLOOR]));
    tns_register_wall(&(app->entity_types[ENTITY_TYPE_WALL]));
    tns_register_main_menu(&(app->entity_types[ENTITY_TYPE_MAIN_MENU]));
    tns_register_controls_menu(&(app->entity_types[ENTITY_TYPE_CONTROLS_MENU]));
    tns_register_characters_menu(&(app->entity_types[ENTITY_TYPE_CHARACTERS_MENU]));
    tns_register_pause_menu(&(app->entity_types[ENTITY_TYPE_PAUSE_MENU]));
    tns_register_forest(&(app->entity_types[ENTITY_TYPE_FOREST]));

    // push the default input handler
    cr_push_input_handler(app, default_input_handler);

    // Load the initial scene.
    load_title_screen(app);
    cr_push_input_handler(app, main_menu_input_handler);

    // Seed random number generation.
    time_t t;
    srand((unsigned int)(time(&t) & INT_MAX));

    return 1;
}
