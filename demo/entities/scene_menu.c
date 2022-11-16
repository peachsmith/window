#include "demo/entities/scene_menu.h"
#include "demo/entities/entity_types.h"
#include "demo/util/util.h"
#include "demo/util/ui.h"
#include "demo/texture/texture.h"
#include "demo/font/font.h"

#include <stdio.h>
#include <stdlib.h>

// scenes menu item text
static const char *item_1_text = "Scene 0";
static const char *item_2_text = "Scene 1";
static const char *item_3_text = "Scene 2";
static const char *item_4_text = "Scene 3";

static void render_scene_menu(eg_app *app, eg_entity *menu)
{
    int menu_x = menu->x_pos;
    int menu_y = menu->y_pos;
    eg_font *font = app->fonts[DEMO_FONT_POKEMON_FIRE_RED];
    int x_origin = 25;
    int y_origin = 5;
    int y_margin = 24;

    // Render the menu panel.
    ui_draw_panel(app, menu_x, menu_y, 80, 130);

    // Render menu items.
    eg_draw_text(app, font, item_1_text, menu_x + x_origin, menu_y + y_origin);
    eg_draw_text(app, font, item_2_text, menu_x + x_origin, menu_y + y_origin + y_margin);
    eg_draw_text(app, font, item_3_text, menu_x + x_origin, menu_y + y_origin + 2 * y_margin);
    eg_draw_text(app, font, item_4_text, menu_x + x_origin, menu_y + y_origin + 3 * y_margin);

    // Render the cursor.
    ui_draw_cursor(
        app,
        (menu_x + 5) + menu->cursor_x * 80,
        (menu_y + 5) + menu->cursor_y * 24);
}

static void update_scene_menu(eg_app *app, eg_entity *menu)
{
}

void scene_menu_demo_register(eg_entity_type *t)
{
    // The width and height will be determined in the render function.
    t->width = 10;
    t->height = 10;

    t->render = render_scene_menu;
    t->update = update_scene_menu;
}

eg_entity *scene_menu_demo_create()
{
    eg_entity *menu = NULL;

    menu = eg_create_entity();
    if (menu == NULL)
    {
        return NULL;
    }

    menu->x_pos = 140;
    menu->y_pos = 10;
    menu->type = ENTITY_TYPE_SCENE_MENU;
    eg_set_flag(menu, ENTITY_FLAG_PAUSE);
    eg_set_flag(menu, ENTITY_FLAG_MENU);

    return menu;
}
