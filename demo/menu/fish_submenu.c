#include "demo/menu/menu.h"

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
    eg_rect r;
    r.x = menu->position.x;
    r.y = menu->position.y;
    r.w = 100;
    r.h = 80;

    // background color
    eg_set_color(app, 0xFF103030);
    eg_draw_rect(app, &r, 1);

    // border color
    eg_set_color(app, 0xFFE0E0E0);
    eg_draw_rect(app, &r, 0);

    // Render menu items.
    for (int i = 0; i < menu->item_count; i++)
    {
        eg_draw_text(app,
                     menu->items[i]->text,
                     menu->items[i]->position.x,
                     menu->items[i]->position.y);
    }

    // Render the cursor.
    eg_rect cusor_rect = {
        .x = menu->position.x + 11,
        .y = menu->position.y + 16 + (menu->cursor.y * 24),
        .w = 8,
        .h = 8};
    eg_draw_rect(app, &cusor_rect, 1);
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
