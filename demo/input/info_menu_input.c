#include "demo/demo.h"
#include "demo/input/input.h"
#include "demo/entities/entity_types.h"

void demo_info_menu_input(cr_app *app)
{
    if (cr_consume_input(app, CR_KEYCODE_X))
    {
        app->menu_count--;
        cr_pop_input_handler(app);

        if (app->dialog_count > 0)
        {
            cr_entity *d = app->dialogs[app->dialog_count - 1];
            d->result = 0;
            app->entity_types[d->type].advance(app, d);
        }

        return;
    }

    cr_entity *menu_entity = app->menus[app->menu_count - 1];
    if (menu_entity == NULL)
    {
        return;
    }

    if (cr_consume_input(app, CR_KEYCODE_UP))
    {
        if (menu_entity->data > 1)
        {
            menu_entity->data--;
        }
    }

    if (cr_consume_input(app, CR_KEYCODE_DOWN))
    {
        if (menu_entity->data < 2)
        {
            menu_entity->data++;
        }
    }

    // menu item selection
    if (cr_consume_input(app, CR_KEYCODE_Z))
    {
        switch (menu_entity->data)
        {
        case 1:
            app->menu_count--;
            cr_pop_input_handler(app);

            if (app->dialog_count > 0)
            {
                cr_entity *d = app->dialogs[app->dialog_count - 1];
                d->result = 1;
                app->entity_types[d->type].advance(app, d);
            }
            break;

        case 2:
            app->menu_count--;
            cr_pop_input_handler(app);

            if (app->dialog_count > 0)
            {
                cr_entity *d = app->dialogs[app->dialog_count - 1];
                d->result = 2;
                app->entity_types[d->type].advance(app, d);
            }
            break;

        default:
            break;
        }
    }
}