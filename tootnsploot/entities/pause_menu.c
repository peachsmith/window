#include "tootnsploot/tootnsploot.h"
#include "tootnsploot/assets.h"
#include "tootnsploot/entities/entity_types.h"
#include "tootnsploot/entities/main_menu.h"
#include "tootnsploot/util/ui.h"
#include "tootnsploot/util/sprite.h"

#include "common/util.h"
#include "common/texture.h"
#include "common/font.h"

#define ITEM_COUNT 3
#define ITEM_HEIGHT 20

// a list of all the pause in the game
static char *character_names[ITEM_COUNT] = {
    "Resume",
    "Main Menu",
    "Quit"};

static void render_pause_menu(eg_app *app, eg_entity *menu)
{
    eg_font *font = app->fonts[TNS_FONT_PRESS_START];
    int x_origin = menu->x_pos;
    int y_origin = menu->y_pos;

    int w = app->entity_types[menu->type].width;
    int h = app->entity_types[menu->type].height;

    // Render the menu panel.
    // For the main menu, this takes up the entire screen.
    eg_rect r = {
        .x = x_origin,
        .y = y_origin,
        .w = w,
        .h = h};
    eg_set_color(app, EG_COLOR_VINIK_BLACK);
    eg_draw_rect(app, &r, 1);

    // Render the menu title.
    eg_draw_text(
        app,
        font,
        "PAUSED",
        x_origin + 40,
        y_origin + 5);

    // Render menu items.
    int i;
    for (i = 0; i < ITEM_COUNT; i++)
    {
        eg_draw_text(
            app,
            font,
            character_names[i],
            x_origin + 25,
            y_origin + 23 + i * ITEM_HEIGHT);
    }

    // Render the cursor.
    ui_draw_cursor(
        app,
        x_origin + 5,
        y_origin + 20 + menu->cursor_y * ITEM_HEIGHT);
}

static void update_pause_menu(eg_app *app, eg_entity *menu)
{
}

void tns_register_pause_menu(eg_entity_type *t)
{
    t->width = 120;
    t->height = 80;

    t->render = render_pause_menu;
    t->update = update_pause_menu;
}

eg_entity *tns_create_pause_menu(eg_app *app)
{
    eg_entity *menu = NULL;

    menu = eg_create_entity(app);
    if (menu == NULL)
    {
        return NULL;
    }

    menu->x_pos = 55;
    menu->y_pos = 50;
    menu->type = ENTITY_TYPE_PAUSE_MENU;
    eg_set_flag(menu, ENTITY_FLAG_PAUSE);
    eg_set_flag(menu, ENTITY_FLAG_MENU);

    return menu;
}
