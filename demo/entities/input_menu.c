#include "demo/demo.h"
#include "demo/assets.h"
#include "demo/entities/input_menu.h"
#include "demo/entities/entity_types.h"
#include "demo/util/ui.h"

#include "common/util.h"

// input actuation menu item text
static const char *item_1_text = "W";
static const char *item_2_text = "A";
static const char *item_3_text = "S";
static const char *item_4_text = "D";
static const char *item_5_text = "Space";

static void render_input_menu(cr_app *app, cr_entity *menu)
{
    int menu_x = menu->x_pos;
    int menu_y = menu->y_pos;
    cr_font *font = app->fonts[DEMO_FONT_POKEMON_FIRE_RED];
    int x_origin = 25;
    int y_origin = 22;
    int y_margin = 24;

    // for rendering actuation counter as text
    char buffer[24];
    int n = 23;

    // Render the menu panel.
    ui_draw_panel(app, menu_x, menu_y, 230, 150);

    // Render the menu title.
    cr_draw_text(
        app,
        font,
        "Input  Actuation",
        menu_x + 75,
        menu_y + 5);

    //-------------------------------------------------------------
    // W actuation
    cr_draw_text(app, font, item_1_text, menu_x + x_origin, menu_y + y_origin);
    int pressed = cr_peek_input(app, CR_KEYCODE_W);
    int count = app->actuation_counters[CR_KEYCODE_W];
    int res = snprintf(buffer, n, "Pressed:  %s", pressed ? "YES" : "NO");
    if (res < 0 || res >= n)
    {
        return;
    }
    // Render actuation states.
    cr_draw_text(app, font, buffer, menu_x + x_origin + 50, menu_y + y_origin);

    res = snprintf(buffer, n, "Count:  %d", count);
    if (res < 0 || res >= n)
    {
        return;
    }
    cr_draw_text(app, font, buffer, menu_x + x_origin + 140, menu_y + y_origin);

    //-------------------------------------------------------------
    // A actuation
    cr_draw_text(app, font, item_2_text, menu_x + x_origin, menu_y + y_origin + y_margin);
    pressed = cr_peek_input(app, CR_KEYCODE_A);
    count = app->actuation_counters[CR_KEYCODE_A];
    res = snprintf(buffer, n, "Pressed:  %s", pressed ? "YES" : "NO");
    if (res < 0 || res >= n)
    {
        return;
    }
    // Render actuation states.
    cr_draw_text(app, font, buffer, menu_x + x_origin + 50, menu_y + y_origin + y_margin);

    res = snprintf(buffer, n, "Count:  %d", count);
    if (res < 0 || res >= n)
    {
        return;
    }
    cr_draw_text(app, font, buffer, menu_x + x_origin + 140, menu_y + y_origin + y_margin);

    //-------------------------------------------------------------
    // S actuation
    cr_draw_text(app, font, item_3_text, menu_x + x_origin, menu_y + y_origin + 2 * y_margin);
    pressed = cr_peek_input(app, CR_KEYCODE_S);
    count = app->actuation_counters[CR_KEYCODE_S];
    res = snprintf(buffer, n, "Pressed:  %s", pressed ? "YES" : "NO");
    if (res < 0 || res >= n)
    {
        return;
    }
    // Render actuation states.
    cr_draw_text(app, font, buffer, menu_x + x_origin + 50, menu_y + y_origin + 2 * y_margin);

    res = snprintf(buffer, n, "Count:  %d", count);
    if (res < 0 || res >= n)
    {
        return;
    }
    cr_draw_text(app, font, buffer, menu_x + x_origin + 140, menu_y + y_origin + 2 * y_margin);

    //-------------------------------------------------------------
    // D actuation
    cr_draw_text(app, font, item_4_text, menu_x + x_origin, menu_y + y_origin + 3 * y_margin);
    pressed = cr_peek_input(app, CR_KEYCODE_D);
    count = app->actuation_counters[CR_KEYCODE_D];
    res = snprintf(buffer, n, "Pressed:  %s", pressed ? "YES" : "NO");
    if (res < 0 || res >= n)
    {
        return;
    }
    // Render actuation states.
    cr_draw_text(app, font, buffer, menu_x + x_origin + 50, menu_y + y_origin + 3 * y_margin);

    res = snprintf(buffer, n, "Count:  %d", count);
    if (res < 0 || res >= n)
    {
        return;
    }
    cr_draw_text(app, font, buffer, menu_x + x_origin + 140, menu_y + y_origin + 3 * y_margin);

    //-------------------------------------------------------------
    // space actuation
    cr_draw_text(app, font, item_5_text, menu_x + x_origin, menu_y + y_origin + 4 * y_margin);
    pressed = cr_peek_input(app, CR_KEYCODE_SPACE);
    count = app->actuation_counters[CR_KEYCODE_SPACE];
    res = snprintf(buffer, n, "Pressed:  %s", pressed ? "YES" : "NO");
    if (res < 0 || res >= n)
    {
        return;
    }
    // Render actuation states.
    cr_draw_text(app, font, buffer, menu_x + x_origin + 50, menu_y + y_origin + 4 * y_margin);

    res = snprintf(buffer, n, "Count:  %d", count);
    if (res < 0 || res >= n)
    {
        return;
    }
    cr_draw_text(app, font, buffer, menu_x + x_origin + 140, menu_y + y_origin + 4 * y_margin);

    // // Render menu items.
    // for (int i = 0; i < menu->item_count; i++)
    // {
    //     int item_x = menu->items[i]->position.x;
    //     int item_y = menu->items[i]->position.y;

    //     cr_draw_text(app, font, menu->items[i]->text, item_x, item_y);

    //     int pressed = 0;
    //     int count = 0;
    //     cr_keycode key = CR_KEYCODE_W;

    //     switch (i)
    //     {
    //     case 0:
    //         key = CR_KEYCODE_W;
    //         break;
    //     case 1:
    //         key = CR_KEYCODE_A;
    //         break;
    //     case 2:
    //         key = CR_KEYCODE_S;
    //         break;
    //     case 3:
    //         key = CR_KEYCODE_D;
    //         break;
    //     case 4:
    //         key = CR_KEYCODE_SPACE;
    //         break;

    //     default:
    //         break;
    //     }

    //     pressed = cr_peek_input(app, key);
    //     count = app->actuation_counters[key];

    //     char buffer[24];
    //     int n = 23;

    //     int res = snprintf(
    //         buffer,
    //         n,
    //         "Pressed:  %s",
    //         pressed ? "YES" : "NO");

    //     if (res < 0 || res >= n)
    //     {
    //         return;
    //     }

    //     // Render actuation states.
    //     cr_draw_text(app,
    //                  font,
    //                  buffer,
    //                  item_x + 50,
    //                  item_y);

    //     res = snprintf(
    //         buffer,
    //         n,
    //         "Count:  %d",
    //         count);

    //     if (res < 0 || res >= n)
    //     {
    //         return;
    //     }

    //     // Render actuation states.
    //     cr_draw_text(app,
    //                  font,
    //                  buffer,
    //                  item_x + 140,
    //                  item_y);
    // }
}

static void update_input_menu(cr_app *app, cr_entity *menu)
{
}

void demo_register_input_menu(cr_entity_type *t)
{
    // The width and height will be determined in the render function.
    t->width = 10;
    t->height = 10;

    t->render = render_input_menu;
    t->update = update_input_menu;
}

cr_entity *demo_create_input_menu(cr_app* app)
{
    cr_entity *menu = NULL;

    menu = cr_create_entity(app);
    if (menu == NULL)
    {
        return NULL;
    }

    menu->x_pos = 5;
    menu->y_pos = 5;
    menu->type = ENTITY_TYPE_INPUT_MENU;
    cr_set_flag(menu, ENTITY_FLAG_PAUSE);
    cr_set_flag(menu, ENTITY_FLAG_MENU);

    return menu;
}
