#include "demo/menu/menu.h"
#include "demo/util/util.h"
#include "demo/util/ui.h"
#include "demo/texture/texture.h"
#include "demo/font/font.h"
#include "demo/input/input.h"

#include <stdio.h>

// info submenu
static eg_menu info_menu;

// info submenu item callbacks
static void info_item_1_callback(eg_app *, eg_menu *);
static void info_item_2_callback(eg_app *, eg_menu *);
static void render_info_menu(eg_app *, eg_menu *);

// info submenu item text
static const char *item_1_text = "Purpose";
static const char *item_2_text = "Implementation";

// info submenu items
static eg_menu_item info_item_1;
static eg_menu_item info_item_2;
static eg_menu_item *info_items[2];

static void info_item_1_callback(eg_app *app, eg_menu *menu)
{
    app->menu_count--;
    eg_pop_input_handler(app);

    if (app->dialog_count > 0)
    {
        eg_dialog *d = app->dialogs[app->dialog_count - 1];
        d->result = 1;
        d->advance(app, d);
    }
}

static void info_item_2_callback(eg_app *app, eg_menu *menu)
{
    app->menu_count--;
    eg_pop_input_handler(app);

    if (app->dialog_count > 0)
    {
        eg_dialog *d = app->dialogs[app->dialog_count - 1];
        d->result = 2;
        d->advance(app, d);
    }
}

static void render_info_menu(eg_app *app, eg_menu *menu)
{
    // tile coordinates in the sprite sheet for the cursor
    int cursor_sheet_x = 5;
    int cursor_sheet_y = 26;

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
    int tile_w = 16;
    int tile_h = 16;
    eg_rect cusor_src = {
        .x = cursor_sheet_x * (tile_w + 2),
        .y = cursor_sheet_y * (tile_h + 2),
        .w = tile_w,
        .h = tile_h};
    eg_rect cusor_dest = {
        .x = 127 + (menu->cursor.x * 80),
        .y = 52 + (menu->cursor.y * 24),
        .w = tile_w,
        .h = tile_h};
    eg_draw_texture(app,
                    app->textures[DEMO_TEXTURE_UI],
                    &cusor_src,
                    &cusor_dest);
}

void demo_init_info_menu(eg_app *app)
{
    // Initialize cursor position.
    info_menu.cursor.x = 0;
    info_menu.cursor.y = 0;

    // Initialize menu position.
    info_menu.position.x = 120;
    info_menu.position.y = 42;

    info_menu.w = 112;
    info_menu.h = 64;

    // Initialize menu items.

    // item 1
    info_item_1.position.x = info_menu.position.x + 25;
    info_item_1.position.y = info_menu.position.y + 9;
    info_item_1.text = item_1_text;
    info_item_1.callback = info_item_1_callback;
    info_items[0] = &info_item_1;

    // item 2
    info_item_2.position.x = info_menu.position.x + 25;
    info_item_2.position.y = info_menu.position.y + 33;
    info_item_2.text = item_2_text;
    info_item_2.callback = info_item_2_callback;
    info_items[1] = &info_item_2;

    // Give the info menu a reference to its items.
    info_menu.items = &(info_items[0]);
    info_menu.item_count = 2;

    // Render function.
    info_menu.render = render_info_menu;
}

void demo_open_info_menu(eg_app *app)
{
    eg_push_input_handler(app, info_menu_input_handler);

    info_menu.cursor.x = 0;
    info_menu.cursor.y = 0;

    app->menus[(app->menu_count)++] = &info_menu;
}
