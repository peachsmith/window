#include "demo/menu/menu.h"
#include "demo/util/util.h"
#include "demo/util/ui.h"
#include "demo/texture/texture.h"
#include "demo/font/font.h"
#include "demo/input/input.h"
#include "colors.h"

#include <stdio.h>

// debug menu
static eg_menu debug_menu;

// debug submenu item callbacks
static void debug_item_1_callback(eg_app *, eg_menu *);
static void debug_item_2_callback(eg_app *, eg_menu *);
static void debug_item_3_callback(eg_app *, eg_menu *);
static void debug_item_4_callback(eg_app *, eg_menu *);
static void render_debug_menu(eg_app *, eg_menu *);

// debug submenu item text
static const char *item_1_text = "Scenes";
static const char *item_2_text = "Frame Length";
static const char *item_3_text = "Frame by Frame";
static const char *item_4_text = "Hit Boxes";

// debug submenu items
static eg_menu_item debug_item_1;
static eg_menu_item debug_item_2;
static eg_menu_item debug_item_3;
static eg_menu_item debug_item_4;
static eg_menu_item *debug_items[4];

static void debug_item_1_callback(eg_app *app, eg_menu *menu)
{
    printf("[DEBUG] \"Scenes\" was selected\n");
    demo_open_scenes_menu(app);
}

static void debug_item_2_callback(eg_app *app, eg_menu *menu)
{
    printf("[DEBUG] \"Frame Length\" was selected\n");
}

static void debug_item_3_callback(eg_app *app, eg_menu *menu)
{
    printf("[DEBUG] \"Frame by Frame\" was selected\n");
}

static void debug_item_4_callback(eg_app *app, eg_menu *menu)
{
    printf("[DEBUG] \"Hit Boxes\" was selected\n");
}

static void update_debug_menu(eg_app *app, eg_menu *menu)
{
    // Update the scroll indicator counter.
    app->counters[0] += 2;

    if (app->counters[0] >= 120)
    {
        app->counters[0] = 0;
    }
}

static void render_debug_menu(eg_app *app, eg_menu *menu)
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

        if (i == 1)
        {
            // Render the selectable options.
            eg_draw_text(app,
                         app->fonts[DEMO_FONT_POKEMON_FIRE_RED],
                         "1",
                         menu->items[i]->position.x + 100,
                         menu->items[i]->position.y);

            eg_draw_text(app,
                         app->fonts[DEMO_FONT_POKEMON_FIRE_RED],
                         "2",
                         menu->items[i]->position.x + 120,
                         menu->items[i]->position.y);

            eg_draw_text(app,
                         app->fonts[DEMO_FONT_POKEMON_FIRE_RED],
                         "4",
                         menu->items[i]->position.x + 140,
                         menu->items[i]->position.y);

            if (menu->cursor.y == 1)
            {
                // Get the scroll indicator counter.
                int count = app->counters[0];
                int offset = 0;
                if (count < 40)
                {
                    offset = count / 10;
                }
                else if (count < 60)
                {
                    offset = 4;
                }
                else if (count < 100)
                {
                    offset = 4 - ((count - 60) / 10);
                }

                // Render the left and right indicators.
                ui_draw_indicator(app,
                                  menu->items[i]->position.x + 80 - offset,
                                  menu->items[i]->position.y,
                                  UI_INDICATOR_SCROLL_LEFT);

                ui_draw_indicator(app,
                                  menu->items[i]->position.x + 160 + offset,
                                  menu->items[i]->position.y,
                                  UI_INDICATOR_SCROLL_RIGHT);
            }

            // Show which option is currently selected.
            int selection = 100;
            eg_set_color(app, EG_COLOR_RED);
            eg_rect r = {.x = menu->items[i]->position.x + selection - 5,
                         .y = menu->items[i]->position.y,
                         .w = 16,
                         .h = 16};
            eg_draw_rect(app, &r, 0);
        }
    }

    // Render the cursor.
    ui_draw_cursor(
        app,
        (menu->position.x + 5) + menu->cursor.x * 80,
        (menu->position.y + 5) + menu->cursor.y * 24);
}

void demo_init_debug_menu(eg_app *app)
{
    // Initialize cursor position.
    debug_menu.cursor.x = 0;
    debug_menu.cursor.y = 0;

    // Initialize menu position.
    debug_menu.position.x = 5;
    debug_menu.position.y = 5;

    debug_menu.w = 230;
    debug_menu.h = 150;

    // Initialize menu items.

    int x_origin = 25;
    int y_origin = 5;
    int y_margin = 24;

    // item 1
    debug_item_1.position.x = debug_menu.position.x + x_origin;
    debug_item_1.position.y = debug_menu.position.y + y_origin;
    debug_item_1.text = item_1_text;
    debug_item_1.callback = debug_item_1_callback;
    debug_items[0] = &debug_item_1;

    // item 2
    debug_item_2.position.x = debug_menu.position.x + x_origin;
    debug_item_2.position.y = debug_menu.position.y + y_origin + y_margin;
    debug_item_2.text = item_2_text;
    debug_item_2.callback = debug_item_2_callback;
    debug_items[1] = &debug_item_2;

    // item 3
    debug_item_3.position.x = debug_menu.position.x + x_origin;
    debug_item_3.position.y = debug_menu.position.y + y_origin + 2 * y_margin;
    debug_item_3.text = item_3_text;
    debug_item_3.callback = debug_item_3_callback;
    debug_items[2] = &debug_item_3;

    // item 4
    debug_item_4.position.x = debug_menu.position.x + x_origin;
    debug_item_4.position.y = debug_menu.position.y + y_origin + 3 * y_margin;
    debug_item_4.text = item_4_text;
    debug_item_4.callback = debug_item_4_callback;
    debug_items[3] = &debug_item_4;

    // Give the debug menu a reference to its items.
    debug_menu.items = &(debug_items[0]);
    debug_menu.item_count = 4;

    // Render function.
    debug_menu.render = render_debug_menu;
    debug_menu.update = update_debug_menu;
}

void demo_open_debug_menu(eg_app *app)
{
    eg_push_input_handler(app, debug_menu_input_handler);

    debug_menu.cursor.x = 0;
    debug_menu.cursor.y = 0;

    app->menus[(app->menu_count)++] = &debug_menu;
}
