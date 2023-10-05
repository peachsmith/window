#include "demo/entities/scene_menu.h"
#include "demo/entities/entity_types.h"
#include "demo/util/util.h"
#include "demo/util/ui.h"
#include "demo/texture/texture.h"
#include "demo/font/font.h"

#include <stdio.h>
#include <stdlib.h>

// scenes menu item text
// static const char *item_1_text = "Scene 0";
// static const char *item_2_text = "Scene 1";
// static const char *item_3_text = "Scene 2";
// static const char *item_4_text = "Scene 3";
// static const char *item_5_text = "Forest";
// static const char *item_6_text = "Movement";

static char *items[7] = {
    "Scene 0",
    "Scene 1",
    "Scene 2",
    "Scene 3",
    "Forest",
    "Movement"};

#define ITEM_HEIGHT 24
#define SCROLL_LIMIT 4

static void render_scene_menu(eg_app *app, eg_entity *menu)
{
    int menu_x = menu->x_pos;
    int menu_y = menu->y_pos;
    eg_font *font = app->fonts[DEMO_FONT_POKEMON_FIRE_RED];
    int x_origin = 25;
    int y_origin = 5;

    // Render the menu panel.
    ui_draw_panel(app, menu_x, menu_y, 90, 100);

    // Render menu items.
    int i;
    for (i = 0 + menu->scroll_y; i < SCROLL_LIMIT + menu->scroll_y; i++)
    {
        eg_draw_text(
            app,
            font,
            items[i],
            menu_x + x_origin,
            menu_y + y_origin + (i - menu->scroll_y) * ITEM_HEIGHT);
    }

    // Render the cursor.
    ui_draw_cursor(
        app,
        (menu_x + 5) + menu->cursor_x * 80,
        (menu_y + 5) + menu->cursor_y * ITEM_HEIGHT);

    // Render the scroll indicators.
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

    if (menu->scroll_y > 0)
    {
        ui_draw_indicator(app, 212, 14 - offset, UI_INDICATOR_SCROLL_UP);
    }

    if (menu->scroll_y < 2)
    {
        ui_draw_indicator(app, 212, 88 + offset, UI_INDICATOR_SCROLL_DOWN);
    }
}

static void update_scene_menu(eg_app *app, eg_entity *menu)
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

void scene_menu_demo_register(eg_entity_type *t)
{
    // The width and height will be determined in the render function.
    t->width = 10;
    t->height = 10;

    t->render = render_scene_menu;
    t->update = update_scene_menu;
}

eg_entity *scene_menu_demo_create(eg_app *app)
{
    eg_entity *menu = NULL;

    menu = eg_create_entity(app);
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
