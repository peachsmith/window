#include "demo/menu/menu.h"
#include "demo/util/util.h"
#include "demo/util/ui.h"
#include "demo/texture/texture.h"
#include "demo/font/font.h"
#include "demo/input/input.h"
#include "demo/scenes/scenes.h"
#include "demo/entities/entity_types.h"

#include <stdio.h>

// scenes menu
static eg_menu scenes_menu;

// scenes submenu item callbacks
static void scenes_item_1_callback(eg_app *, eg_menu *);
static void scenes_item_2_callback(eg_app *, eg_menu *);
static void scenes_item_3_callback(eg_app *, eg_menu *);
static void scenes_item_4_callback(eg_app *, eg_menu *);
static void render_scenes_menu(eg_app *, eg_menu *);

// scenes submenu item text
static const char *item_1_text = "Scene 0";
static const char *item_2_text = "Scene 1";
static const char *item_3_text = "Scene 2";
static const char *item_4_text = "Scene 3";

// scenes submenu items
static eg_menu_item scenes_item_1;
static eg_menu_item scenes_item_2;
static eg_menu_item scenes_item_3;
static eg_menu_item scenes_item_4;
static eg_menu_item *scenes_items[4];

static void scenes_item_1_callback(eg_app *app, eg_menu *menu)
{
    // Close the scenes menu.
    app->menu_count--;
    eg_pop_input_handler(app);

    // Close the debug menu.
    app->menu_count--;
    eg_pop_input_handler(app);

    // Start the screen transition.
    eg_entity *e = app->entities;
    while (e != NULL)
    {
        if (e->type == ENTITY_TYPE_TRANSITION)
        {
            e->data = 1;
        }
        e = e->next;
    }

    // Clear the current scene and load the next scene.
    // clear_scene(app);
    // load_scene_0(app);

    // Unpause the application.
    app->pause = 0;
}

static void scenes_item_2_callback(eg_app *app, eg_menu *menu)
{
    // Close the scenes menu.
    app->menu_count--;
    eg_pop_input_handler(app);

    // Close the debug menu.
    app->menu_count--;
    eg_pop_input_handler(app);

    // Clear the current scene and load the next scene.
    clear_scene(app);
    load_scene_1(app);

    // Unpause the application.
    app->pause = 0;
}

static void scenes_item_3_callback(eg_app *app, eg_menu *menu)
{
    // Close the scenes menu.
    app->menu_count--;
    eg_pop_input_handler(app);

    // Close the debug menu.
    app->menu_count--;
    eg_pop_input_handler(app);

    // Clear the current scene and load the next scene.
    clear_scene(app);
    load_scene_2(app);

    // Unpause the application.
    app->pause = 0;
}

static void scenes_item_4_callback(eg_app *app, eg_menu *menu)
{
    // Close the scenes menu.
    app->menu_count--;
    eg_pop_input_handler(app);

    // Close the debug menu.
    app->menu_count--;
    eg_pop_input_handler(app);

    // Clear the current scene and load the next scene.
    clear_scene(app);
    load_scene_3(app);

    // Unpause the application.
    app->pause = 0;
}

static void render_scenes_menu(eg_app *app, eg_menu *menu)
{
    // Render the menu panel.
    ui_draw_panel(app, menu->position.x, menu->position.y, menu->w, menu->h);

    // Render menu items.
    for (int i = 0; i < menu->item_count; i++)
    {
        eg_draw_text(app,
                     app->fonts[DEMO_FONT_POKEMON_FIRE_RED],
                     menu->items[i]->text,
                     menu->items[i]->position.x,
                     menu->items[i]->position.y);
    }

    // Render the cursor.
    ui_draw_cursor(
        app,
        (menu->position.x + 5) + menu->cursor.x * 80,
        (menu->position.y + 5) + menu->cursor.y * 24);
}

void demo_init_scenes_menu(eg_app *app)
{
    // Initialize cursor position.
    scenes_menu.cursor.x = 0;
    scenes_menu.cursor.y = 0;

    // Initialize menu position.
    scenes_menu.position.x = 140;
    scenes_menu.position.y = 10;

    scenes_menu.w = 80;
    scenes_menu.h = 130;

    // Initialize menu items.

    int x_origin = 25;
    int y_origin = 5;
    int y_margin = 24;

    // item 1
    scenes_item_1.position.x = scenes_menu.position.x + x_origin;
    scenes_item_1.position.y = scenes_menu.position.y + y_origin;
    scenes_item_1.text = item_1_text;
    scenes_item_1.callback = scenes_item_1_callback;
    scenes_items[0] = &scenes_item_1;

    // item 2
    scenes_item_2.position.x = scenes_menu.position.x + x_origin;
    scenes_item_2.position.y = scenes_menu.position.y + y_origin + y_margin;
    scenes_item_2.text = item_2_text;
    scenes_item_2.callback = scenes_item_2_callback;
    scenes_items[1] = &scenes_item_2;

    // item 3
    scenes_item_3.position.x = scenes_menu.position.x + x_origin;
    scenes_item_3.position.y = scenes_menu.position.y + y_origin + 2 * y_margin;
    scenes_item_3.text = item_3_text;
    scenes_item_3.callback = scenes_item_3_callback;
    scenes_items[2] = &scenes_item_3;

    // item 4
    scenes_item_4.position.x = scenes_menu.position.x + x_origin;
    scenes_item_4.position.y = scenes_menu.position.y + y_origin + 3 * y_margin;
    scenes_item_4.text = item_4_text;
    scenes_item_4.callback = scenes_item_4_callback;
    scenes_items[3] = &scenes_item_4;

    // Give the scenes menu a reference to its items.
    scenes_menu.items = &(scenes_items[0]);
    scenes_menu.item_count = 4;

    // Render function.
    scenes_menu.render = render_scenes_menu;
    scenes_menu.update = NULL;
}

void demo_open_scenes_menu(eg_app *app)
{
    eg_push_input_handler(app, scenes_menu_input_handler);

    scenes_menu.cursor.x = 0;
    scenes_menu.cursor.y = 0;

    app->menus[(app->menu_count)++] = &scenes_menu;
}
