#include "demo/demo.h"
#include "demo/assets.h"
#include "demo/entities/entity_types.h"
#include "demo/entities/debug_menu.h"
#include "demo/util/ui.h"

#include "common/util.h"

// debug menu item text
static const char *item_1_text = "Scenes";
static const char *item_2_text = "Frame Length";
static const char *item_3_text = "Hit Boxes";
static const char *item_4_text = "Draw Collisions";
static const char *item_5_text = "Input Actuation";

static void render_debug_menu(cr_app *app, cr_entity *menu)
{
    int menu_x = menu->x_pos;
    int menu_y = menu->y_pos;
    int x_origin = 25;
    int y_origin = 5;
    int y_margin = 24;
    cr_font *font = app->fonts[DEMO_FONT_POKEMON_FIRE_RED];

    // Render the menu panel.
    ui_draw_panel(app, menu_x, menu_y, 230, 150);

    //-----------------------------------------------------
    // BEGIN Scenes
    cr_draw_text(app, font,
                 item_1_text,
                 menu_x + x_origin,
                 menu_y + y_origin);
    // END Scenes
    //-----------------------------------------------------

    //-----------------------------------------------------
    // BEGIN Frame Length
    cr_draw_text(app, font,
                 item_2_text,
                 menu_x + x_origin,
                 menu_y + y_origin + y_margin);
    cr_draw_text(app, font, "1", menu_x + x_origin + 107, menu_y + y_origin + y_margin);
    cr_draw_text(app, font, "2", menu_x + x_origin + 126, menu_y + y_origin + y_margin);
    cr_draw_text(app, font, "4", menu_x + x_origin + 146, menu_y + y_origin + y_margin);
    cr_draw_text(app, font, "8", menu_x + x_origin + 166, menu_y + y_origin + y_margin);

    if (menu->cursor_y == 1)
    {
        // Get the scroll indicator counter.
        int count = menu->ticks;
        int offset = 0;
        if (count < 20)
        {
            offset = count / 5;
        }
        else if (count < 30)
        {
            offset = 4;
        }
        else if (count < 50)
        {
            offset = 4 - ((count - 30) / 5);
        }

        // Render the left and right indicators.
        ui_draw_indicator(app,
                          menu_x + x_origin + 80 - offset,
                          menu_y + y_origin + y_margin,
                          UI_INDICATOR_SCROLL_LEFT);

        ui_draw_indicator(app,
                          menu_x + x_origin + 180 + offset,
                          menu_y + y_origin + y_margin,
                          UI_INDICATOR_SCROLL_RIGHT);
    }

    // Show which option is currently selected.
    int selection = 100;

    switch (app->debug.frame_len)
    {
    case 1:
        selection -= 1;
        break;

    case 2:
        selection += 20;
        break;

    case 4:
        selection += 40;
        break;

    case 8:
        selection += 60;
        break;

    default:
        break;
    }

    cr_set_color(app, CR_COLOR_VINIK_RED);
    cr_rect r = {.x = menu_x + x_origin + selection + 1,
                 .y = menu_y + y_origin + y_margin,
                 .w = 16,
                 .h = 16};
    cr_draw_rect(app, &r, 0);
    // END Frame Length
    //-----------------------------------------------------

    //-----------------------------------------------------
    // BEGIN Hitbox Rendering
    cr_draw_text(app, font,
                 item_3_text,
                 menu_x + x_origin,
                 menu_y + y_origin + 2 * y_margin);

    if (menu->cursor_y == 2)
    {
        // Get the scroll indicator counter.
        int count = menu->ticks;
        int offset = 0;
        if (count < 20)
        {
            offset = count / 5;
        }
        else if (count < 30)
        {
            offset = 4;
        }
        else if (count < 50)
        {
            offset = 4 - ((count - 30) / 5);
        }

        // Render the left and right indicators.
        ui_draw_indicator(app,
                          menu_x + x_origin + 80 - offset,
                          menu_y + y_origin + 2 * y_margin,
                          UI_INDICATOR_SCROLL_LEFT);

        ui_draw_indicator(app,
                          menu_x + x_origin + 160 + offset,
                          menu_y + y_origin + 2 * y_margin,
                          UI_INDICATOR_SCROLL_RIGHT);
    }

    cr_draw_text(
        app,
        font,
        app->debug.hitboxes ? "ON" : "OFF",
        menu_x + x_origin + 120,
        menu_y + y_origin + 2 * y_margin);
    // END Hitbox Rendering
    //-----------------------------------------------------

    //-----------------------------------------------------
    // BEGIN Collision Rendering
    cr_draw_text(app, font,
                 item_4_text,
                 menu_x + x_origin,
                 menu_y + y_origin + 3 * y_margin);

    if (menu->cursor_y == 3)
    {
        // Get the scroll indicator counter.
        int count = menu->ticks;
        int offset = 0;
        if (count < 20)
        {
            offset = count / 5;
        }
        else if (count < 30)
        {
            offset = 4;
        }
        else if (count < 50)
        {
            offset = 4 - ((count - 30) / 5);
        }

        // Render the left and right indicators.
        ui_draw_indicator(app,
                          menu_x + x_origin + 80 - offset,
                          menu_y + y_origin + 3 * y_margin,
                          UI_INDICATOR_SCROLL_LEFT);

        ui_draw_indicator(app,
                          menu_x + x_origin + 160 + offset,
                          menu_y + y_origin + 3 * y_margin,
                          UI_INDICATOR_SCROLL_RIGHT);
    }

    cr_draw_text(
        app,
        font,
        app->debug.collisions ? "ON" : "OFF",
        menu_x + x_origin + 120,
        menu_y + y_origin + 3 * y_margin);
    // END Collision Rendering
    //-----------------------------------------------------

    //-----------------------------------------------------
    // BEGIN Input Actuation
    cr_draw_text(app, font,
                 item_5_text,
                 menu_x + x_origin,
                 menu_y + y_origin + 4 * y_margin);
    // END Input Actuation
    //-----------------------------------------------------

    // Render the cursor.
    ui_draw_cursor(
        app,
        (menu_x + 5) + menu->cursor_x * 80,
        (menu_y + 5) + menu->cursor_y * 24);
}

static void update_debug_menu(cr_app *app, cr_entity *menu)
{
    menu->ticks++;

    // Reset the tick count when we reach some arbitrary limit.
    // The debug menu doesn't have any use for a tick count over 100,
    // so we pick some value greater than 100. 200 sounds nice.
    if (menu->ticks >= 60)
    {
        menu->ticks = 0;
    }
}

void demo_register_debug_menu(cr_entity_type *t)
{
    // The width and height will be determined in the render function.
    t->width = 10;
    t->height = 10;

    t->render = render_debug_menu;
    t->update = update_debug_menu;
}

cr_entity *demo_create_debug_menu(cr_app *app)
{
    cr_entity *menu = NULL;

    menu = cr_create_entity(app);
    if (menu == NULL)
    {
        return NULL;
    }

    menu->x_pos = 5;
    menu->y_pos = 5;
    menu->cursor_x = 0;
    menu->cursor_y = 0;
    menu->type = ENTITY_TYPE_DEBUG_MENU;
    cr_set_flag(menu, ENTITY_FLAG_PAUSE);
    cr_set_flag(menu, ENTITY_FLAG_MENU);

    return menu;
}
