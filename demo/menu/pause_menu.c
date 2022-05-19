#include "demo/menu/menu.h"
#include "demo/input/input.h"
#include "demo/dialog/dialog.h"
#include "demo/util/util.h"
#include "demo/texture/texture.h"
#include "demo/font/font.h"

#include <stdio.h>

// pause menu
static eg_menu pause_menu;

// pause menu item callbacks
static void pause_item_1_callback(eg_app *, eg_menu *);
static void pause_item_2_callback(eg_app *, eg_menu *);
static void pause_item_3_callback(eg_app *, eg_menu *);
static void pause_item_4_callback(eg_app *, eg_menu *);
static void render_pause_menu(eg_app *, eg_menu *);

// pause menu item text
static const char *item_1_text = "Info";
static const char *item_2_text = "Quit";
static const char *item_3_text = "Submenu";
static const char *item_4_text = "Dialog";

// pause menu items
static eg_menu_item pause_item_1;
static eg_menu_item pause_item_2;
static eg_menu_item pause_item_3;
static eg_menu_item pause_item_4;
static eg_menu_item *pause_items[4];

static void pause_item_1_callback(eg_app *app, eg_menu *menu)
{
    demo_open_info_dialog(app);
}

static void pause_item_2_callback(eg_app *app, eg_menu *menu)
{
    app->done = 1;
}

static void pause_item_3_callback(eg_app *app, eg_menu *menu)
{
    // eg_push_input_handler(app, fish_menu_input_handler);
    demo_open_fish_menu(app);
}

static void pause_item_4_callback(eg_app *app, eg_menu *menu)
{
    demo_open_demo_dialog(app);
}

static void render_pause_menu(eg_app *app, eg_menu *menu)
{
    // tile coordinates in the sprite sheet for the cursor
    int cursor_sheet_x = 5;
    int cursor_sheet_y = 26;

    // Render the menu panel.
    eg_rect rect = {
        .x = menu->position.x,
        .y = menu->position.y,
        .w = 13,
        .h = 5};
    demo_draw_panel(app, &rect);

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
    int tile_w = 16;
    int tile_h = 16;
    eg_rect cusor_src = {
        .x = cursor_sheet_x * (tile_w + 2),
        .y = cursor_sheet_y * (tile_h + 2),
        .w = tile_w,
        .h = tile_h};
    eg_rect cusor_dest = {
        .x = 16 + (menu->cursor.x * 80),
        .y = 22 + (menu->cursor.y * 24),
        .w = tile_w,
        .h = tile_h};
    eg_draw_texture(app,
                    app->textures[DEMO_TEXTURE_UI],
                    &cusor_src,
                    &cusor_dest);
}

void demo_init_pause_menu(eg_app *app)
{
    // Initialize cursor position.
    pause_menu.cursor.x = 0;
    pause_menu.cursor.y = 0;

    // Initialize menu position.
    pause_menu.position.x = 5;
    pause_menu.position.y = 5;

    // Initialize menu items.

    // item 1
    pause_item_1.position.x = pause_menu.position.x + 30;
    pause_item_1.position.y = pause_menu.position.y + 17;
    pause_item_1.text = item_1_text;
    pause_item_1.callback = pause_item_1_callback;
    pause_items[0] = &pause_item_1;

    // item 2
    pause_item_2.position.x = pause_menu.position.x + 30;
    pause_item_2.position.y = pause_menu.position.y + 41;
    pause_item_2.text = item_2_text;
    pause_item_2.callback = pause_item_2_callback;
    pause_items[1] = &pause_item_2;

    // item 3
    pause_item_3.position.x = pause_menu.position.x + 110;
    pause_item_3.position.y = pause_menu.position.y + 17;
    pause_item_3.text = item_3_text;
    pause_item_3.callback = pause_item_3_callback;
    pause_items[2] = &pause_item_3;

    // item 4
    pause_item_4.position.x = pause_menu.position.x + 110;
    pause_item_4.position.y = pause_menu.position.y + 41;
    pause_item_4.text = item_4_text;
    pause_item_4.callback = pause_item_4_callback;
    pause_items[3] = &pause_item_4;

    // Give the pause menu a reference to its items.
    pause_menu.items = &(pause_items[0]);
    pause_menu.item_count = 4;

    // Render function.
    pause_menu.render = render_pause_menu;
}

void demo_open_pause_menu(eg_app *app)
{
    eg_push_input_handler(app, pause_menu_input_handler);

    pause_menu.cursor.x = 0;
    pause_menu.cursor.y = 0;

    app->menus[(app->menu_count)++] = &pause_menu;
}
