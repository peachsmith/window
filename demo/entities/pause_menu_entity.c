#include "demo/entities/pause_menu_entity.h"
#include "demo/entities/entity_types.h"
#include "demo/util/util.h"
#include "demo/util/ui.h"
#include "demo/texture/texture.h"
#include "demo/font/font.h"

#include <stdio.h>
#include <stdlib.h>

static void render_pause_menu_entity(eg_app *app, eg_entity *menu)
{
    // Render the menu panel.
    ui_draw_panel(
        app,
        menu->x_pos,
        menu->y_pos,
        app->registry[menu->type].width,
        app->registry[menu->type].height);

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

static void update_pause_menu_entity(eg_app *app, eg_entity *menu)
{
}

void pause_menu_entity_demo_register(eg_entity_type *t)
{
    t->width = 172;
    t->height = 77;
    t->render = render_pause_menu_entity;
    t->update = update_pause_menu_entity;
}

eg_entity *pause_menu_entity_demo_create()
{
    eg_entity *menu = NULL;

    menu = eg_create_entity();
    if (menu == NULL)
    {
        return NULL;
    }

    menu->x_pos = 5;
    menu->y_pos = 5;
    menu->type = ENTITY_TYPE_MENU;
    menu->data = 1;
    eg_set_flag(menu, ENTITY_FLAG_PAUSE);

    return menu;
}
