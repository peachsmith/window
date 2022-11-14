/**
 * Deprecated
 * This file is in the process of being replaced by the fish menu entity.
 * 
 */

// #include "demo/menu/menu.h"
// #include "demo/util/util.h"
// #include "demo/util/ui.h"
// #include "demo/texture/texture.h"
// #include "demo/font/font.h"
// #include "demo/input/input.h"

// #include <stdio.h>

// // fish submenu
// static eg_menu fish_menu;

// // fish submenu item callbacks
// static void fish_item_1_callback(eg_app *, eg_menu *);
// static void fish_item_2_callback(eg_app *, eg_menu *);
// static void render_fish_menu(eg_app *, eg_menu *);

// // // fish submenu item text
// // static const char *item_1_text = "Trout";
// // static const char *item_2_text = "Salmon";

// // fish submenu items
// static eg_menu_item fish_item_1;
// static eg_menu_item fish_item_2;
// static eg_menu_item *fish_items[2];

// static void fish_item_1_callback(eg_app *app, eg_menu *menu)
// {
//     printf("[DEBUG] trout was selected\n");
// }

// static void fish_item_2_callback(eg_app *app, eg_menu *menu)
// {
//     printf("[DEBUG] salmon was selected\n");
// }

// static void render_fish_menu(eg_app *app, eg_menu *menu)
// {
//     // Render the menu panel.
//     ui_draw_panel(app, menu->position.x, menu->position.y, menu->w, menu->h);

//     // Render menu items.
//     for (int i = 0; i < menu->item_count; i++)
//     {
//         eg_draw_text(app,
//                      app->fonts[DEMO_FONT_POKEMON_FIRE_RED],
//                      menu->items[i]->text,
//                      menu->items[i]->position.x,
//                      menu->items[i]->position.y);
//     }

//     // Render the cursor.
//     ui_draw_cursor(
//         app,
//         107 + menu->cursor.x * 80,
//         59 + menu->cursor.y * 24);
// }

// void demo_init_fish_menu(eg_app *app)
// {
//     // Initialize cursor position.
//     fish_menu.cursor.x = 0;
//     fish_menu.cursor.y = 0;

//     // Initialize menu position.
//     fish_menu.position.x = 100;
//     fish_menu.position.y = 50;

//     fish_menu.w = 80;
//     fish_menu.h = 64;

//     // Initialize menu items.

//     // item 1
//     fish_item_1.position.x = fish_menu.position.x + 25;
//     fish_item_1.position.y = fish_menu.position.y + 9;
//     // fish_item_1.text = item_1_text;
//     fish_item_1.callback = fish_item_1_callback;
//     fish_items[0] = &fish_item_1;

//     // item 2
//     fish_item_2.position.x = fish_menu.position.x + 25;
//     fish_item_2.position.y = fish_menu.position.y + 33;
//     // fish_item_2.text = item_2_text;
//     fish_item_2.callback = fish_item_2_callback;
//     fish_items[1] = &fish_item_2;

//     // Give the fish menu a reference to its items.
//     fish_menu.items = &(fish_items[0]);
//     fish_menu.item_count = 2;

//     // Render function.
//     fish_menu.render = NULL; // render_fish_menu;
//     fish_menu.update = NULL;
// }

// void demo_open_fish_menu(eg_app *app)
// {
//     eg_push_input_handler(app, fish_menu_input_handler);

//     // fish_menu.cursor.x = 0;
//     // fish_menu.cursor.y = 0;

//     // app->menus[(app->menu_count)++] = &fish_menu;
// }
