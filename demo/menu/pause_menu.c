#include "demo/menu/menu.h"
#include "demo/input/input.h"
#include "demo/dialog/dialog.h"

#include <stdio.h>

// pause menu
static eg_menu pause_menu;

// pause menu item callbacks
static void pause_item_1_callback(eg_app *app, eg_entity *target);
static void pause_item_2_callback(eg_app *app, eg_entity *target);
static void pause_item_3_callback(eg_app *app, eg_entity *target);
static void pause_item_4_callback(eg_app *app, eg_entity *target);
static void render_pause_menu(eg_app *app, eg_menu *menu);

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
    eg_input_handler *handler = eg_create_input_handler(fish_input_callback, NULL);
    eg_push_input_handler(app, handler);
    demo_open_fish_submenu(app);
}

static void pause_item_4_callback(eg_app *app, eg_entity *target)
{
    demo_open_demo_dialog(app);
}

static void render_pause_menu(eg_app *app, eg_menu *menu)
{
    int cursor_sheet_x = 5;
    int cursor_sheet_y = 26;
    int menu_sheet_x = 20;
    int menu_sheet_y = 20;
    int tile_w = 16;
    int tile_h = 16;

    eg_rect r;
    r.x = menu->position.x;
    r.y = menu->position.y;
    r.w = 208; // 16 * 13
    r.h = 80;  // 16 * 5

    // Render the menu panel.
    eg_rect menu_src = {
        .x = menu_sheet_x * (tile_w + 2),
        .y = menu_sheet_y * (tile_h + 2),
        .w = tile_w,
        .h = tile_h};
    eg_rect menu_dest = {
        .x = menu->position.x,
        .y = menu->position.y,
        .w = tile_w,
        .h = tile_h};

    for (int r = menu->position.y; r < tile_h * 5; r += tile_h)
    {
        for (int c = menu->position.x; c < tile_w * 13; c += tile_w)
        {
            // top side
            if (r == menu->position.y)
            {
                // top left corner
                if (c == menu->position.x)
                {
                    menu_sheet_x = 3;
                    menu_sheet_y = 18;
                }

                // top middle
                if (c > menu->position.x && c < menu->position.x + tile_w * 12)
                {
                    menu_sheet_x = 4;
                    menu_sheet_y = 18;
                }

                // top right corner
                if (c == menu->position.x + tile_w * 12)
                {
                    menu_sheet_x = 5;
                    menu_sheet_y = 18;
                }
            }

            // middle
            if (r > menu->position.y && r < menu->position.y + tile_h * 4)
            {
                // left side
                if (c == menu->position.x)
                {
                    menu_sheet_x = 3;
                    menu_sheet_y = 19;
                }

                // middle
                if (c > menu->position.x && c < menu->position.x + tile_w * 12)
                {
                    menu_sheet_x = 4;
                    menu_sheet_y = 19;
                }

                // right side
                if (c == menu->position.x + tile_w * 12)
                {
                    menu_sheet_x = 5;
                    menu_sheet_y = 19;
                }
            }

            // bottom
            if (r == menu->position.y + tile_h * 4)
            {
                // bottom left corner
                if (c == menu->position.x)
                {
                    menu_sheet_x = 3;
                    menu_sheet_y = 20;
                }

                // middle
                if (c > menu->position.x && c < menu->position.x + tile_w * 12)
                {
                    menu_sheet_x = 4;
                    menu_sheet_y = 20;
                }

                // bottom right corner
                if (c == menu->position.x + tile_w * 12)
                {
                    menu_sheet_x = 5;
                    menu_sheet_y = 20;
                }
            }

            menu_src.x = menu_sheet_x * (tile_w + 2);
            menu_src.y = menu_sheet_y * (tile_h + 2);
            menu_dest.x = c;
            menu_dest.y = r;
            eg_draw_image(app, &menu_src, &menu_dest);
        }
    }

    // background color
    // eg_set_color(app, 0xFF103030);
    // eg_draw_rect(app, &r, 1);

    // // border color
    // eg_set_color(app, 0xFFE0E0E0);
    // eg_draw_rect(app, &r, 0);

    // Render menu items.
    for (int i = 0; i < menu->item_count; i++)
    {
        eg_draw_text(app,
                     menu->items[i]->text,
                     menu->items[i]->position.x,
                     menu->items[i]->position.y);
    }

    // Render the cursor.
    eg_rect cusor_src = {
        .x = cursor_sheet_x * (tile_w + 2),
        .y = cursor_sheet_y * (tile_h + 2),
        .w = tile_w,
        .h = tile_h};
    eg_rect cusor_dest = {
        .x = 16 + (menu->cursor.x * 80),
        .y = 22 + (menu->cursor.y * 24),
        .w = 16,
        .h = 16};

    eg_draw_image(app, &cusor_src, &cusor_dest);
    // eg_draw_rect(app, &cusor_dest, 1);
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
    app->menus[(app->menu_count)++] = &pause_menu;
}
