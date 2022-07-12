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
static void debug_item_3_callback(eg_app *, eg_menu *);
static void render_debug_menu(eg_app *, eg_menu *);

// debug submenu item text
static const char *item_1_text = "Scenes";
static const char *item_2_text = "Frame Length";
static const char *item_3_text = "Frame by Frame";
static const char *item_4_text = "Hit Boxes";
static const char *item_5_text = "Draw Collisions";

// debug submenu items
static eg_menu_item debug_item_1;
static eg_menu_item debug_item_2;
static eg_menu_item debug_item_3;
static eg_menu_item debug_item_4;
static eg_menu_item debug_item_5;
static eg_menu_item *debug_items[5];

// used to keep track of animated horizontal arrow indicators
static int *toggle_counter = NULL;

static void debug_item_1_callback(eg_app *app, eg_menu *menu)
{
    printf("[DEBUG] \"Scenes\" was selected\n");
    demo_open_scenes_menu(app);
}

static void debug_item_3_callback(eg_app *app, eg_menu *menu)
{
    printf("[DEBUG] \"Frame by Frame\" was selected\n");
}

static void update_debug_menu(eg_app *app, eg_menu *menu)
{
    if (toggle_counter == NULL)
    {
        return;
    }

    // Update the scroll indicator counter.
    // app->counters[0] += 2;
    *toggle_counter += 2;

    if (*toggle_counter >= 120)
    {
        *toggle_counter = 0;
    }
}

static void render_debug_menu(eg_app *app, eg_menu *menu)
{
    int menu_x = menu->position.x;
    int menu_y = menu->position.y;
    int cursor_x = menu->cursor.x;
    int cursor_y = menu->cursor.y;
    eg_font *font = app->fonts[DEMO_FONT_POKEMON_FIRE_RED];

    // Render the menu panel.
    ui_draw_panel(app, menu_x, menu_y, menu->w, menu->h);

    // Render menu items.
    for (int i = 0; i < menu->item_count; i++)
    {
        int item_x = menu->items[i]->position.x;
        int item_y = menu->items[i]->position.y;

        eg_draw_text(app, font, menu->items[i]->text, item_x, item_y);

        if (i == 1)
        {
            // Render the selectable options.
            eg_draw_text(app, font, "1", item_x + 100, item_y);
            eg_draw_text(app, font, "2", item_x + 120, item_y);
            eg_draw_text(app, font, "4", item_x + 140, item_y);

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
                                  item_x + 80 - offset,
                                  item_y,
                                  UI_INDICATOR_SCROLL_LEFT);

                ui_draw_indicator(app,
                                  item_x + 160 + offset,
                                  item_y,
                                  UI_INDICATOR_SCROLL_RIGHT);
            }

            // Show which option is currently selected.
            int selection = 100;
            eg_set_color(app, EG_COLOR_RED);
            eg_rect r = {.x = item_x + selection - 5,
                         .y = item_y,
                         .w = 16,
                         .h = 16};
            eg_draw_rect(app, &r, 0);
        }

        if (i == 3)
        {
            if (menu->cursor.y == 3)
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
                                  item_x + 80 - offset,
                                  item_y,
                                  UI_INDICATOR_SCROLL_LEFT);

                ui_draw_indicator(app,
                                  item_x + 160 + offset,
                                  item_y,
                                  UI_INDICATOR_SCROLL_RIGHT);
            }

            eg_draw_text(
                app,
                font,
                app->debug.hitboxes ? "ON" : "OFF",
                item_x + 120,
                item_y);
        }

        if (i == 4)
        {
            if (menu->cursor.y == 4)
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
                                  item_x + 80 - offset,
                                  item_y,
                                  UI_INDICATOR_SCROLL_LEFT);

                ui_draw_indicator(app,
                                  item_x + 160 + offset,
                                  item_y,
                                  UI_INDICATOR_SCROLL_RIGHT);
            }

            eg_draw_text(
                app,
                font,
                app->debug.collisions ? "ON" : "OFF",
                item_x + 120,
                item_y);
        }
    }

    // Render the cursor.
    ui_draw_cursor(
        app,
        (menu_x + 5) + cursor_x * 80,
        (menu_y + 5) + cursor_y * 24);
}

int demo_init_debug_menu(eg_app *app)
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

    // item 1 (Scenes)
    debug_item_1.position.x = debug_menu.position.x + x_origin;
    debug_item_1.position.y = debug_menu.position.y + y_origin;
    debug_item_1.text = item_1_text;
    debug_item_1.callback = debug_item_1_callback;
    debug_items[0] = &debug_item_1;

    // item 2 (Frame Length)
    debug_item_2.position.x = debug_menu.position.x + x_origin;
    debug_item_2.position.y = debug_menu.position.y + y_origin + y_margin;
    debug_item_2.text = item_2_text;
    debug_item_2.callback = NULL;
    debug_items[1] = &debug_item_2;

    // item 3 (Frame by FRame)
    debug_item_3.position.x = debug_menu.position.x + x_origin;
    debug_item_3.position.y = debug_menu.position.y + y_origin + 2 * y_margin;
    debug_item_3.text = item_3_text;
    debug_item_3.callback = debug_item_3_callback;
    debug_items[2] = &debug_item_3;

    // item 4 (Hitboxes)
    debug_item_4.position.x = debug_menu.position.x + x_origin;
    debug_item_4.position.y = debug_menu.position.y + y_origin + 3 * y_margin;
    debug_item_4.text = item_4_text;
    debug_item_4.callback = NULL;
    debug_items[3] = &debug_item_4;

    // item 5 (Draw Collisions)
    debug_item_5.position.x = debug_menu.position.x + x_origin;
    debug_item_5.position.y = debug_menu.position.y + y_origin + 4 * y_margin;
    debug_item_5.text = item_5_text;
    debug_item_5.callback = NULL;
    debug_items[4] = &debug_item_5;

    // Give the debug menu a reference to its items.
    debug_menu.items = &(debug_items[0]);
    debug_menu.item_count = 5;

    // Render function.
    debug_menu.render = render_debug_menu;
    debug_menu.update = update_debug_menu;

    toggle_counter = eg_reserve_counter(app);
    if (toggle_counter == NULL)
    {
        printf("[ERROR] failed to reserve counter\n");
        return 0;
    }

    return 1;
}

void demo_open_debug_menu(eg_app *app)
{
    eg_push_input_handler(app, debug_menu_input_handler);

    debug_menu.cursor.x = 0;
    debug_menu.cursor.y = 0;

    app->menus[(app->menu_count)++] = &debug_menu;
}
