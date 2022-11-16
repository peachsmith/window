#include "demo/menu/menu.h"
#include "demo/util/util.h"
#include "demo/util/ui.h"
#include "demo/texture/texture.h"
#include "demo/font/font.h"
#include "demo/input/input.h"
#include "colors.h"

#include <stdio.h>

// input actuation debugger menu
static eg_menu input_menu;

// input actuation menu item text
static const char *item_1_text = "W";
static const char *item_2_text = "A";
static const char *item_3_text = "S";
static const char *item_4_text = "D";
static const char *item_5_text = "Space";

// input actuation menu items
static eg_menu_item input_item_1;
static eg_menu_item input_item_2;
static eg_menu_item input_item_3;
static eg_menu_item input_item_4;
static eg_menu_item input_item_5;
static eg_menu_item *input_items[5];

static void render_input_menu(eg_app *app, eg_menu *menu)
{
    int menu_x = menu->position.x;
    int menu_y = menu->position.y;
    eg_font *font = app->fonts[DEMO_FONT_POKEMON_FIRE_RED];

    // Render the menu panel.
    ui_draw_panel(app, menu_x, menu_y, menu->w, menu->h);

    // Render the menu title.
    eg_draw_text(
        app,
        font,
        "Input  Actuation",
        menu_x + 75,
        menu_y + 5);

    // Render menu items.
    for (int i = 0; i < menu->item_count; i++)
    {
        int item_x = menu->items[i]->position.x;
        int item_y = menu->items[i]->position.y;

        eg_draw_text(app, font, menu->items[i]->text, item_x, item_y);

        int pressed = 0;
        int count = 0;
        eg_keycode key = EG_KEYCODE_W;

        switch (i)
        {
        case 0:
            key = EG_KEYCODE_W;
            break;
        case 1:
            key = EG_KEYCODE_A;
            break;
        case 2:
            key = EG_KEYCODE_S;
            break;
        case 3:
            key = EG_KEYCODE_D;
            break;
        case 4:
            key = EG_KEYCODE_SPACE;
            break;

        default:
            break;
        }

        pressed = eg_peek_input(app, key);
        count = app->actuation_counters[key];

        char buffer[24];
        int n = 23;

        int res = snprintf(
            buffer,
            n,
            "Pressed:  %s",
            pressed ? "YES" : "NO");

        if (res < 0 || res >= n)
        {
            return;
        }

        // Render actuation states.
        eg_draw_text(app,
                     font,
                     buffer,
                     item_x + 50,
                     item_y);

        res = snprintf(
            buffer,
            n,
            "Count:  %d",
            count);

        if (res < 0 || res >= n)
        {
            return;
        }

        // Render actuation states.
        eg_draw_text(app,
                     font,
                     buffer,
                     item_x + 140,
                     item_y);
    }
}

void demo_init_input_menu(eg_app *app)
{
    // Initialize cursor position.
    input_menu.cursor.x = 0;
    input_menu.cursor.y = 0;

    // Initialize menu position.
    input_menu.position.x = 5;
    input_menu.position.y = 5;

    input_menu.w = 230;
    input_menu.h = 150;

    // Initialize menu items.

    int x_origin = 25;
    int y_origin = 22;
    int y_margin = 24;

    // item 1 (space key press)
    input_item_1.position.x = input_menu.position.x + x_origin;
    input_item_1.position.y = input_menu.position.y + y_origin;
    input_item_1.text = item_1_text;
    input_item_1.callback = NULL;
    input_items[0] = &input_item_1;

    // item 2 (W key press)
    input_item_2.position.x = input_menu.position.x + x_origin;
    input_item_2.position.y = input_menu.position.y + y_origin + y_margin;
    input_item_2.text = item_2_text;
    input_item_2.callback = NULL;
    input_items[1] = &input_item_2;

    // item 3 (A key press)
    input_item_3.position.x = input_menu.position.x + x_origin;
    input_item_3.position.y = input_menu.position.y + y_origin + 2 * y_margin;
    input_item_3.text = item_3_text;
    input_item_3.callback = NULL;
    input_items[2] = &input_item_3;

    // item 4 (S key press)
    input_item_4.position.x = input_menu.position.x + x_origin;
    input_item_4.position.y = input_menu.position.y + y_origin + 3 * y_margin;
    input_item_4.text = item_4_text;
    input_item_4.callback = NULL;
    input_items[3] = &input_item_4;

    // item 5 (D key press)
    input_item_5.position.x = input_menu.position.x + x_origin;
    input_item_5.position.y = input_menu.position.y + y_origin + 4 * y_margin;
    input_item_5.text = item_5_text;
    input_item_5.callback = NULL;
    input_items[4] = &input_item_5;

    // Give the input menu a reference to its items.
    input_menu.items = &(input_items[0]);
    input_menu.item_count = 5;

    // Render function.
    input_menu.render = render_input_menu;
    input_menu.update = NULL;
}

void demo_open_input_menu(eg_app *app)
{
    eg_push_input_handler(app, input_menu_input_handler);

    input_menu.cursor.x = 0;
    input_menu.cursor.y = 0;

    app->menus[(app->menu_count)++] = &input_menu;
}