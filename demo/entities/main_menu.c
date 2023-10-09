#include "demo/entities/main_menu.h"
#include "demo/entities/entity_types.h"
#include "demo/util/util.h"
#include "demo/util/ui.h"
#include "demo/texture/texture.h"
#include "demo/font/font.h"
#include "demo/texture/texture.h"

#include <stdio.h>
#include <stdlib.h>

static char *items[3] = {
    "START",
    "CONTROLS",
    "MEET THE CHARACTERS"};

#define ITEM_HEIGHT 20

static void render_main_menu(eg_app *app, eg_entity *menu)
{
    eg_font *font = app->fonts[DEMO_FONT_PRESS_START];
    int x_origin = 60;
    int y_origin = 90;

    int w = app->entity_types[menu->type].width;
    int h = app->entity_types[menu->type].height;

    // Render the menu panel.
    // For the main menu, this takes up the entire screen.
    eg_rect r = {
        .x = 0,
        .y = 0,
        .w = w,
        .h = h};
    eg_set_color(app, EG_COLOR_BLACK);
    eg_draw_rect(app, &r, 1);

    // Render the title.
    eg_rect src = {
        .x = 0,
        .y = 0,
        .w = 140,
        .h = 70};
    eg_rect dest = {
        .x = 45,
        .y = 5,
        .w = 140,
        .h = 70};
    eg_draw_texture(
        app,
        app->textures[TNS_TEXTURE_TITLE],
        &src,
        &dest,
        0);

    // Render menu items.
    int i;
    for (i = 0 + menu->scroll_y; i < 3; i++)
    {
        eg_draw_text(
            app,
            font,
            items[i],
            x_origin,
            y_origin + i * ITEM_HEIGHT);
    }

    // Render the cursor.
    ui_draw_cursor(
        app,
        x_origin - 20,
        y_origin - 3 + menu->cursor_y * ITEM_HEIGHT);
}

static void update_main_menu(eg_app *app, eg_entity *menu)
{
    menu->ticks++;

    // We don't have any reason to exceed 60 ticks, so we just reset the tick
    // count and start over whenever we reach that threshold. The number 60
    // was chosen because that's the desired framerate, but it doesn't really
    // matter.
    if (menu->ticks >= 60)
    {
        menu->ticks = 0;
    }
}

void tns_register_main_menu(eg_entity_type *t)
{
    t->width = 240;
    t->height = 160;

    t->render = render_main_menu;
    t->update = update_main_menu;
}

eg_entity *tns_create_main_menu(eg_app *app)
{
    eg_entity *menu = NULL;

    menu = eg_create_entity(app);
    if (menu == NULL)
    {
        return NULL;
    }

    menu->type = ENTITY_TYPE_MAIN_MENU;
    eg_set_flag(menu, ENTITY_FLAG_PAUSE);
    eg_set_flag(menu, ENTITY_FLAG_MENU);

    return menu;
}
