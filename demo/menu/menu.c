#include "demo/menu/menu.h"

#include <stdio.h>

// maximum number of menus
#define MAX_MENUS 10

static void pause_item_1_callback(eg_app *app, eg_entity *target);
static void render_pause_menu(eg_app *app, const eg_menu *menu);

static const char *item_1_text = "Info";
static eg_menu_item pause_item_1;
static eg_menu_item *pause_items[4];
static eg_menu pause_menu;

// main menu list for the application
static const eg_menu *menus[MAX_MENUS];

static void pause_item_1_callback(eg_app *app, eg_entity *target)
{
    printf("[DEBUG] this is the callback for pause menu item 1\n");
}

static void render_pause_menu(eg_app *app, const eg_menu *menu)
{
    eg_rect r;
    r.x = menu->position.x;
    r.y = menu->position.y;
    r.w = 100;
    r.h = 50;

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
    pause_item_1.position.x = 9;
    pause_item_1.position.y = 9;
    pause_item_1.text = item_1_text;
    pause_item_1.callback = pause_item_1_callback;
    pause_items[0] = &pause_item_1;

    // Give the pause menu a reference to its items.
    pause_menu.items = &(pause_items[0]);
    pause_menu.item_count = 1;

    // Render function.
    pause_menu.render = render_pause_menu;

    // Add the pause menu to the menu array.
    menus[0] = &pause_menu;

    // Give the application a reference to the menu array.
    app->menus = &(menus[0]);
    app->menu_count = 1;
}
