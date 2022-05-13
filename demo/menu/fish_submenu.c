#include "demo/menu/menu.h"
#include "demo/util/util.h"

#include <stdio.h>

// fish submenu item callbacks
static void fish_item_1_callback(eg_app *app, eg_entity *target);
static void fish_item_2_callback(eg_app *app, eg_entity *target);
static void render_fish_submenu(eg_app *app, eg_menu *menu);

// fish submenu item text
static const char *item_1_text = "Trout";
static const char *item_2_text = "Salmon";

// fish submenu items
static eg_menu_item fish_item_1;
static eg_menu_item fish_item_2;
static eg_menu_item *fish_items[2];

static eg_menu fish_submenu;

static void fish_item_1_callback(eg_app *app, eg_entity *target)
{
    printf("[DEBUG] trout was selected\n");
}

static void fish_item_2_callback(eg_app *app, eg_entity *target)
{
    printf("[DEBUG] salmon was selected\n");
}

static void render_fish_submenu(eg_app *app, eg_menu *menu)
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
    eg_draw_image(app, &cusor_src, &cusor_dest);
}

void demo_init_fish_submenu(eg_app *app)
{
    // Initialize cursor position.
    fish_submenu.cursor.x = 0;
    fish_submenu.cursor.y = 0;

    // Initialize menu position.
    fish_submenu.position.x = 100;
    fish_submenu.position.y = 50;

    // Initialize menu items.

    // item 1
    fish_item_1.position.x = fish_submenu.position.x + 25;
    fish_item_1.position.y = fish_submenu.position.y + 9;
    fish_item_1.text = item_1_text;
    fish_item_1.callback = fish_item_1_callback;
    fish_items[0] = &fish_item_1;

    // item 2
    fish_item_2.position.x = fish_submenu.position.x + 25;
    fish_item_2.position.y = fish_submenu.position.y + 33;
    fish_item_2.text = item_2_text;
    fish_item_2.callback = fish_item_2_callback;
    fish_items[1] = &fish_item_2;

    // Give the fish menu a reference to its items.
    fish_submenu.items = &(fish_items[0]);
    fish_submenu.item_count = 2;

    // Render function.
    fish_submenu.render = render_fish_submenu;
}

void demo_open_fish_submenu(eg_app *app)
{
    app->menus[(app->menu_count)++] = &fish_submenu;
}
