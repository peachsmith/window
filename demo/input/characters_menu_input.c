#include "demo/input/input.h"
#include "demo/menu/menu.h"
#include "demo/entities/entity_types.h"
#include "demo/scenes/scenes.h"

#include <stdio.h>

void tns_characters_menu_input_handler(eg_app *app)
{
    // Locate the characters menu.
    eg_entity *menu_entity = app->menus[app->menu_count - 1];
    if (menu_entity == NULL)
    {
        return;
    }

    // If either the escape key or the x key is pressed, return to the main menu.
    if (eg_consume_input(app, EG_KEYCODE_ESCAPE) || eg_consume_input(app, EG_KEYCODE_X))
    {
        if (menu_entity->data)
        {
            menu_entity->data = 0;
            return;
        }

        app->menu_count--;
        eg_pop_input_handler(app);
        return;
    }

    // If a character is selected, stop processing input here.
    if (menu_entity->data)
    {
        return;
    }

    if (eg_consume_input(app, EG_KEYCODE_UP))
    {
        if (menu_entity->cursor_y > 0)
        {
            menu_entity->cursor_y--;
        }
    }

    if (eg_consume_input(app, EG_KEYCODE_DOWN))
    {
        if (menu_entity->cursor_y < 5)
        {
            menu_entity->cursor_y++;
        }
    }

    // menu item selection
    // Skippy
    // Squirrelton
    // Squirrelina
    // Thomas H. Foolery
    // Byarf Squirrelson
    // Throbgar the Sumptuous
    if (eg_consume_input(app, EG_KEYCODE_Z))
    {
        switch (menu_entity->cursor_y)
        {
        case 0:
            menu_entity->data = 1;
            break;
        case 1:
            menu_entity->data = 2;
            break;
        case 2:
            menu_entity->data = 3;
            break;
        case 3:
            menu_entity->data = 4;
            break;
        case 4:
            menu_entity->data = 5;
            break;
        case 5:
            menu_entity->data = 6;
            break;
        default:
            break;
        }
    }
}
