#include "demo/menu/menu.h"

#include <stdio.h>

// maximum number of menus
#define MAX_MENUS 10

static void pause_item_1_callback(eg_app *app, eg_entity *target);
static void pause_item_2_callback(eg_app *app, eg_entity *target);
static void pause_item_3_callback(eg_app *app, eg_entity *target);
static void pause_item_4_callback(eg_app *app, eg_entity *target);
static void render_pause_menu(eg_app *app, eg_menu *menu);

static const char *item_1_text = "Info";
static const char *item_2_text = "Quit";
static const char *item_3_text = "Submenu";
static const char *item_4_text = "Dialog";
static eg_menu_item pause_item_1;
static eg_menu_item pause_item_2;
static eg_menu_item pause_item_3;
static eg_menu_item pause_item_4;
static eg_menu_item *pause_items[4];
static eg_menu pause_menu;

// main menu list for the application
static eg_menu *menus[MAX_MENUS];

static void pause_item_1_callback(eg_app *app, eg_entity *target)
{
    printf("[DEBUG] this is the callback for pause menu item 1\n");
}

static void pause_item_2_callback(eg_app *app, eg_entity *target)
{
    app->done = 1;
}

static void pause_item_3_callback(eg_app *app, eg_entity *target)
{
    printf("[DEBUG] TODO: implement a submenu\n");
}

static void pause_item_4_callback(eg_app *app, eg_entity *target)
{
    printf("[DEBUG] TODO: implement a dialog\n");
}

static void render_pause_menu(eg_app *app, eg_menu *menu)
{
    eg_rect r;
    r.x = menu->position.x;
    r.y = menu->position.y;
    r.w = 200;
    r.h = 100;

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
        .x = 16 + (menu->cursor.x * 80),
        .y = 22 + (menu->cursor.y * 24),
        .w = 8,
        .h = 8};
    eg_draw_rect(app, &cusor_rect, 1);
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
    pause_item_1.position.x = 30;
    pause_item_1.position.y = 14;
    pause_item_1.text = item_1_text;
    pause_item_1.callback = pause_item_1_callback;
    pause_items[0] = &pause_item_1;

    // item 2
    pause_item_2.position.x = 30;
    pause_item_2.position.y = 38;
    pause_item_2.text = item_2_text;
    pause_item_2.callback = pause_item_2_callback;
    pause_items[1] = &pause_item_2;

    // item 3
    pause_item_3.position.x = 110;
    pause_item_3.position.y = 14;
    pause_item_3.text = item_3_text;
    pause_item_3.callback = pause_item_3_callback;
    pause_items[2] = &pause_item_3;

    // item 4
    pause_item_4.position.x = 110;
    pause_item_4.position.y = 38;
    pause_item_4.text = item_4_text;
    pause_item_4.callback = pause_item_4_callback;
    pause_items[3] = &pause_item_4;

    // Give the pause menu a reference to its items.
    pause_menu.items = &(pause_items[0]);
    pause_menu.item_count = 4;

    // Render function.
    pause_menu.render = render_pause_menu;

    // Add the pause menu to the menu array.
    menus[0] = &pause_menu;

    // Give the application a reference to the menu array.
    app->menus = &(menus[0]);
    app->menu_count = 1;
}
