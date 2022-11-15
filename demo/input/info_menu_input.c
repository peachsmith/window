#include "demo/input/input.h"
#include "demo/entities/entity_types.h"
#include "demo/menu/menu.h"

#include <stdlib.h>

void info_menu_input_handler(eg_app *app)
{
    if (eg_consume_input(app, EG_KEYCODE_X))
    {
        app->menu_count--;
        eg_pop_input_handler(app);

        if (app->dialog_count > 0)
        {
            eg_entity *d = app->dialog_entities[app->dialog_count - 1];
            d->result = 0;
            app->registry[d->type].advance(app, d);
        }

        return;
    }

    eg_entity *menu_entity = app->menu_entities[app->menu_count - 1];
    if (menu_entity == NULL)
    {
        return;
    }

    if (eg_consume_input(app, EG_KEYCODE_UP))
    {
        if (menu_entity->data > 1)
        {
            menu_entity->data--;
        }
    }

    if (eg_consume_input(app, EG_KEYCODE_DOWN))
    {
        if (menu_entity->data < 2)
        {
            menu_entity->data++;
        }
    }

    // menu item selection
    if (eg_consume_input(app, EG_KEYCODE_Z))
    {
        switch (menu_entity->data)
        {
        case 1:
            app->menu_count--;
            eg_pop_input_handler(app);

            if (app->dialog_count > 0)
            {
                eg_entity *d = app->dialog_entities[app->dialog_count - 1];
                d->result = 1;
                app->registry[d->type].advance(app, d);
            }
            break;

        case 2:
            app->menu_count--;
            eg_pop_input_handler(app);

            if (app->dialog_count > 0)
            {
                eg_entity *d = app->dialog_entities[app->dialog_count - 1];
                d->result = 2;
                app->registry[d->type].advance(app, d);
            }
            break;

        default:
            break;
        }
    }
}