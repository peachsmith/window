#include "demo/entities/info_dialog.h"
#include "demo/input/input.h"
#include "demo/dialog/dialog.h"
#include "demo/entities/entity_types.h"
#include "demo/util/util.h"
#include "demo/util/ui.h"
#include "demo/texture/texture.h"
#include "demo/font/font.h"
#include "demo/menu/menu.h"

#include <stdio.h>
#include <stdlib.h>

static const char *info_dialog_panel_1 = "What kind of information would you like?";
#define PANEL_1_LEN 40

static const char *purpose_panel = "This software is an exercise in API design.";
#define PURPOSE_PANEL_LEN 43

static const char *implementation_panel = "This software was implemented using SDL2.";
#define IMPLEMENTATION_PANEL_LEN 41

static void update_info_dialog(eg_app *app, eg_entity *dialog)
{
    if (dialog->ticks < dialog->tick_limit)
    {
        dialog->ticks++;
    }

    // If we've reached the end of the first panel, open the info menu.
    if (dialog->data == 0 && dialog->ticks == dialog->tick_limit)
    {
        dialog->ticks++;
        // Locate the info menu.
        eg_entity *info_menu = NULL;
        for (int i = 0; i < app->entity_count; i++)
        {
            if (app->entity_array[i].type == ENTITY_TYPE_INFO_MENU)
            {
                info_menu = &(app->entity_array[i]);
            }
        }

        // Set the pause menu as the active menu.
        app->menus[app->menu_count++] = info_menu;

        eg_push_input_handler(app, info_menu_input_handler);
    }
}

static void advance_info_dialog(eg_app *app, eg_entity *dialog)
{
    dialog->data++;

    if (dialog->data >= 2)
    {
        // Close the dialog.
        app->dialog_count--;
        eg_pop_input_handler(app);
        return;
    }

    if (dialog->data == 1)
    {
        // If no option was selected, close the dialog.
        if (dialog->result == 0)
        {
            // Close the dialog.
            app->dialog_count--;
            eg_pop_input_handler(app);
            return;
        }

        if (dialog->result == 1)
        {
            dialog->text = purpose_panel;
            dialog->text_len = PURPOSE_PANEL_LEN;
            dialog->ticks = 0;
            dialog->tick_limit = dialog->text_len * DEMO_DIALOG_SPEED_SCALE;
            dialog->result = 0;
        }

        if (dialog->result == 2)
        {
            dialog->text = implementation_panel;
            dialog->text_len = IMPLEMENTATION_PANEL_LEN;
            dialog->ticks = 0;
            dialog->tick_limit = dialog->text_len * DEMO_DIALOG_SPEED_SCALE;
            dialog->result = 0;
        }

        return;
    }
}

void info_dialog_demo_register(eg_entity_type *t)
{
    // The width and height will be determined in the render function.
    t->width = 10;
    t->height = 10;

    t->render = demo_common_dialog_renderer;
    t->update = update_info_dialog;
    t->advance = advance_info_dialog;
}

eg_entity *info_dialog_demo_create()
{
    eg_entity *dialog = NULL;

    dialog = eg_create_entity();
    if (dialog == NULL)
    {
        return NULL;
    }

    dialog->x_pos = 8;
    dialog->y_pos = 108;
    dialog->type = ENTITY_TYPE_INFO_DIALOG;

    eg_set_flag(dialog, ENTITY_FLAG_PAUSE);
    eg_set_flag(dialog, ENTITY_FLAG_MENU);

    return dialog;
}

void info_dialog_demo_open(eg_app *app, eg_entity *dialog)
{
    // Reset the dialog.
    dialog->data = 0;
    dialog->text = info_dialog_panel_1;
    dialog->text_len = PANEL_1_LEN;
    dialog->ticks = 0;
    dialog->tick_limit = dialog->text_len * DEMO_DIALOG_SPEED_SCALE;

    app->dialogs[app->dialog_count++] = dialog;
}
