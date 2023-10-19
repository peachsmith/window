#include "demo/demo.h"
#include "demo/assets.h"
#include "demo/entities/entity_types.h"
#include "demo/entities/pause_menu.h"
#include "demo/util/ui.h"

#include "common/util.h"

// pause menu item text
static const char *item_1_text = "Info";
static const char *item_2_text = "Quit";
static const char *item_3_text = "Submenu";
static const char *item_4_text = "Dialog";

static void render_pause_menu(cr_app *app, cr_entity *menu)
{
    // Render the menu panel.
    ui_draw_panel(
        app,
        menu->x_pos,
        menu->y_pos,
        172,
        77);

    // Render the menu items.
    cr_draw_text(
        app,
        app->fonts[DEMO_FONT_POKEMON_FIRE_RED],
        item_1_text,
        menu->x_pos + 30,
        menu->y_pos + 17);

    cr_draw_text(
        app,
        app->fonts[DEMO_FONT_POKEMON_FIRE_RED],
        item_2_text,
        menu->x_pos + 30,
        menu->y_pos + 41);

    cr_draw_text(
        app,
        app->fonts[DEMO_FONT_POKEMON_FIRE_RED],
        item_3_text,
        menu->x_pos + 110,
        menu->y_pos + 17);

    cr_draw_text(
        app,
        app->fonts[DEMO_FONT_POKEMON_FIRE_RED],
        item_4_text,
        menu->x_pos + 110,
        menu->y_pos + 41);

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
        cursor_x = 1;
        cursor_y = 0;
        break;

    case 3:
        cursor_x = 0;
        cursor_y = 1;
        break;

    case 4:
        cursor_x = 1;
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
        16 + cursor_x * 80,
        22 + cursor_y * 24);
}

static void update_pause_menu(cr_app *app, cr_entity *menu)
{
}

void demo_register_pause_menu(cr_entity_type *t)
{
    t->width = 172;
    t->height = 77;
    t->render = render_pause_menu;
    t->update = update_pause_menu;
}

cr_entity *demo_create_pause_menu(cr_app* app)
{
    cr_entity *menu = NULL;

    menu = cr_create_entity(app);
    if (menu == NULL)
    {
        return NULL;
    }

    menu->x_pos = 5;
    menu->y_pos = 5;
    menu->type = ENTITY_TYPE_PAUSE_MENU;
    menu->data = 1;
    cr_set_flag(menu, ENTITY_FLAG_PAUSE);
    cr_set_flag(menu, ENTITY_FLAG_MENU);

    return menu;
}
