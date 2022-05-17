#include "demo/menu/menu.h"
#include "demo/util/util.h"
#include "demo/texture/texture.h"
#include "demo/font/font.h"

#include <stdio.h>

// fish submenu
static eg_menu fish_menu;

// fish submenu item callbacks
static void fish_item_1_callback(eg_app *, eg_menu *);
static void fish_item_2_callback(eg_app *, eg_menu *);
static void render_fish_menu(eg_app *, eg_menu *);

// fish submenu item text
static const char *item_1_text = "Trout";
static const char *item_2_text = "Salmon";

// fish submenu items
static eg_menu_item fish_item_1;
static eg_menu_item fish_item_2;
static eg_menu_item *fish_items[2];

static void fish_item_1_callback(eg_app *app, eg_menu *menu)
{
    printf("[DEBUG] trout was selected\n");
}

static void fish_item_2_callback(eg_app *app, eg_menu *menu)
{
    printf("[DEBUG] salmon was selected\n");
}

static void render_fish_menu(eg_app *app, eg_menu *menu)
{
    // tile coordinates in the sprite sheet for the cursor
    int cursor_sheet_x = 5;
    int cursor_sheet_y = 26;

    // Render the menu panel.
    eg_rect rect = {
        .x = menu->position.x,
        .y = menu->position.y,
        .w = 5,
        .h = 4};
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
        .x = 107 + (menu->cursor.x * 80),
        .y = 59 + (menu->cursor.y * 24),
        .w = tile_w,
        .h = tile_h};
    eg_draw_texture(app,
                    app->textures[DEMO_TEXTURE_UI],
                    &cusor_src,
                    &cusor_dest);
}

void demo_init_fish_menu(eg_app *app)
{
    // Initialize cursor position.
    fish_menu.cursor.x = 0;
    fish_menu.cursor.y = 0;

    // Initialize menu position.
    fish_menu.position.x = 100;
    fish_menu.position.y = 50;

    // Initialize menu items.

    // item 1
    fish_item_1.position.x = fish_menu.position.x + 25;
    fish_item_1.position.y = fish_menu.position.y + 9;
    fish_item_1.text = item_1_text;
    fish_item_1.callback = fish_item_1_callback;
    fish_items[0] = &fish_item_1;

    // item 2
    fish_item_2.position.x = fish_menu.position.x + 25;
    fish_item_2.position.y = fish_menu.position.y + 33;
    fish_item_2.text = item_2_text;
    fish_item_2.callback = fish_item_2_callback;
    fish_items[1] = &fish_item_2;

    // Give the fish menu a reference to its items.
    fish_menu.items = &(fish_items[0]);
    fish_menu.item_count = 2;

    // Render function.
    fish_menu.render = render_fish_menu;
}

void demo_open_fish_menu(eg_app *app)
{
    app->menus[(app->menu_count)++] = &fish_menu;
}
