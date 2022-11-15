#include "demo/entities/sign_dialog.h"
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

static const char *panel_1_text = "This is a very informative sign.\nTrust me.";
#define PANEL_1_LEN 42

static void update_sign_dialog(eg_app *app, eg_entity *dialog)
{
    if (dialog->ticks < dialog->tick_limit)
    {
        dialog->ticks++;
    }
}

static void advance_sign_dialog(eg_app *app, eg_entity *dialog)
{
    dialog->data++;

    if (dialog->data >= 1)
    {
        // Close the dialog.
        app->dialog_count--;
        eg_pop_input_handler(app);
        app->pause = 0;
        return;
    }
}

void sign_dialog_demo_register(eg_entity_type *t)
{
    // The width and height will be determined in the render function.
    t->width = 10;
    t->height = 10;

    t->render = demo_common_dialog_renderer;
    t->update = update_sign_dialog;
    t->advance = advance_sign_dialog;
}

eg_entity *sign_dialog_demo_create()
{
    eg_entity *dialog = NULL;

    dialog = eg_create_entity();
    if (dialog == NULL)
    {
        return NULL;
    }

    dialog->x_pos = 8;
    dialog->y_pos = 108;
    dialog->type = ENTITY_TYPE_SIGN_DIALOG;

    eg_set_flag(dialog, ENTITY_FLAG_PAUSE);
    eg_set_flag(dialog, ENTITY_FLAG_MENU);

    return dialog;
}

void sign_dialog_demo_open(eg_app *app, eg_entity *dialog)
{
    // Reset the dialog.
    dialog->data = 0;
    dialog->text = panel_1_text;
    dialog->text_len = PANEL_1_LEN;
    dialog->ticks = 0;
    dialog->tick_limit = dialog->text_len * DEMO_DIALOG_SPEED_SCALE;

    app->dialog_entities[app->dialog_count++] = dialog;
}
