#include "demo/entities/demo_dialog.h"
#include "demo/dialog/dialog.h"
#include "demo/entities/entity_types.h"
#include "demo/util/util.h"
#include "demo/util/ui.h"
#include "demo/texture/texture.h"
#include "demo/font/font.h"

#include <stdio.h>
#include <stdlib.h>

static const char *demo_dialog_panel_1 = "This is a dialog. This is a test of the line\nbreak functionality. How long can we go?";
#define PANEL_1_LEN 85

static const char *demo_dialog_panel_2 = "This is the second panel of text in the demo dialog.";
#define PANEL_2_LEN 52

static void update_demo_dialog(eg_app *app, eg_entity *dialog)
{
    if (dialog->ticks < dialog->tick_limit)
    {
        dialog->ticks++;
    }
}

static void advance_demo_dialog(eg_app *app, eg_entity *dialog)
{
    // We currently use the data field to represent which panel of content
    // should be rendered in the dialog.

    if (dialog->data >= 1)
    {
        // Close the dialog.
        app->dialog_count--;
        eg_pop_input_handler(app);
        return;
    }

    dialog->text = demo_dialog_panel_2;
    dialog->text_len = PANEL_2_LEN;

    dialog->ticks = 0;
    dialog->tick_limit = dialog->text_len * DEMO_DIALOG_SPEED_SCALE;

    dialog->data++;
}

void demo_dialog_demo_register(eg_entity_type *t)
{
    // The width and height will be determined in the render function.
    t->width = 10;
    t->height = 10;

    t->render = demo_common_dialog_renderer;
    t->update = update_demo_dialog;
    t->advance = advance_demo_dialog;
}

eg_entity *demo_dialog_demo_create()
{
    eg_entity *dialog = NULL;

    dialog = eg_create_entity();
    if (dialog == NULL)
    {
        return NULL;
    }

    dialog->x_pos = 8;
    dialog->y_pos = 108;
    dialog->type = ENTITY_TYPE_DEMO_DIALOG;

    eg_set_flag(dialog, ENTITY_FLAG_PAUSE);
    eg_set_flag(dialog, ENTITY_FLAG_MENU);

    return dialog;
}

void demo_dialog_demo_open(eg_app *app, eg_entity *dialog)
{
    // Reset the dialog.
    dialog->data = 0;
    dialog->text = demo_dialog_panel_1;
    dialog->text_len = PANEL_1_LEN;
    dialog->ticks = 0;
    dialog->tick_limit = dialog->text_len * DEMO_DIALOG_SPEED_SCALE;

    app->dialog_entities[app->dialog_count++] = dialog;
}
