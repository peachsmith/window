#include "demo/demo.h"
#include "demo/assets.h"
#include "demo/entities/entity_types.h"
#include "demo/entities/fish_menu.h"
#include "demo/util/ui.h"

#include "common/util.h"

// fish menu item text
static const char *item_1_text = "Trout";
static const char *item_2_text = "Salmon";

static void render_fish_menu(cr_app *app, cr_entity *menu)
{
    // Render the menu panel.
    ui_draw_panel(
        app,
        menu->x_pos,
        menu->y_pos,
        80,
        64);

    // Render the menu items.
    cr_draw_text(
        app,
        app->fonts[DEMO_FONT_POKEMON_FIRE_RED],
        item_1_text,
        menu->x_pos + 25,
        menu->y_pos + 9);

    cr_draw_text(
        app,
        app->fonts[DEMO_FONT_POKEMON_FIRE_RED],
        item_2_text,
        menu->x_pos + 25,
        menu->y_pos + 33);

    // Determine where to render the cursor.
    int cursor_x = 8;
    int cursor_y = 8;

    switch (menu->data)
    {
    case 1:
        cursor_x = 0;
        cursor_y = 0;
        break;

    case 2:
        cursor_x = 0;
        cursor_y = 1;
        break;

    default:
        cursor_x = 0;
        cursor_y = 0;
        break;
    }

    // Render the cursor.
    ui_draw_cursor(
        app,
        107 + cursor_x * 80,
        59 + cursor_y * 24);
}

static void update_fish_menu(cr_app *app, cr_entity *menu)
{
}

void demo_register_fish_menu(cr_entity_type *t)
{
    // The width and height will be determined in the render function.
    t->width = 10;
    t->height = 10;

    t->render = render_fish_menu;
    t->update = update_fish_menu;
}

cr_entity *demo_create_fish_menu(cr_app* app)
{
    cr_entity *menu = NULL;

    menu = cr_create_entity(app);
    if (menu == NULL)
    {
        return NULL;
    }

    menu->x_pos = 100;
    menu->y_pos = 50;
    menu->type = ENTITY_TYPE_FISH_MENU;
    menu->data = 1;
    cr_set_flag(menu, ENTITY_FLAG_PAUSE);
    cr_set_flag(menu, ENTITY_FLAG_MENU);

    return menu;
}
