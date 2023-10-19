#include "demo/demo.h"
#include "demo/assets.h"
#include "demo/entities/entity_types.h"
#include "demo/entities/demo_dialog.h"
#include "demo/util/ui.h"

#include "common/util.h"
#include "common/dialog.h"

static const char *demo_dialog_panel_1 = "This is a dialog. This is a test of the line\nbreak functionality. How long can we go?";
#define PANEL_1_LEN 85

static const char *demo_dialog_panel_2 = "This is the second panel of text in the demo\ndialog.";
#define PANEL_2_LEN 52

static void update_demo_dialog(cr_app *app, cr_entity *dialog)
{
    if (dialog->ticks < dialog->tick_limit)
    {
        dialog->ticks++;
    }
}

static void advance_demo_dialog(cr_app *app, cr_entity *dialog)
{
    // We currently use the data field to represent which panel of content
    // should be rendered in the dialog.

    if (dialog->data >= 1)
    {
        // Close the dialog.
        app->dialog_count--;
        cr_pop_input_handler(app);
        return;
    }

    dialog->text = demo_dialog_panel_2;
    dialog->text_len = PANEL_2_LEN;

    dialog->ticks = 0;
    dialog->tick_limit = dialog->text_len * COMMON_DIALOG_SPEED_SCALE;

    dialog->data++;
}

static void render_dialog_panel(cr_app *app, cr_entity *dialog)
{
    ui_draw_panel(
        app,
        dialog->x_pos,
        dialog->y_pos,
        COMMON_DIALOG_WIDTH,
        COMMON_DIALOG_HEIGHT);
}

static void render_dialog_indicator(cr_app *app, cr_entity *dialog)
{
    ui_draw_indicator(
        app,
        dialog->x_pos + COMMON_DIALOG_WIDTH - 18,
        dialog->y_pos + COMMON_DIALOG_HEIGHT - 18,
        UI_INDICATOR_ADVANCE);
}

static void render_dialog(cr_app *app, cr_entity *dialog)
{
    common_dialog_renderer(
        app,
        dialog,
        app->fonts[DEMO_FONT_POKEMON_FIRE_RED],
        render_dialog_panel,
        render_dialog_indicator);
}

void demo_dialog_demo_register(cr_entity_type *t)
{
    // The width and height will be determined in the render function.
    t->width = 10;
    t->height = 10;

    t->render = render_dialog;
    t->update = update_demo_dialog;
    t->advance = advance_demo_dialog;
}

cr_entity *demo_create_demo_dialog(cr_app *app)
{
    cr_entity *dialog = NULL;

    dialog = cr_create_entity(app);
    if (dialog == NULL)
    {
        return NULL;
    }

    dialog->x_pos = 8;
    dialog->y_pos = 108;
    dialog->type = ENTITY_TYPE_DEMO_DIALOG;

    cr_set_flag(dialog, ENTITY_FLAG_PAUSE);
    cr_set_flag(dialog, ENTITY_FLAG_MENU);

    return dialog;
}

void demo_open_demo_dialog(cr_app *app, cr_entity *dialog)
{
    // Reset the dialog.
    dialog->data = 0;
    dialog->text = demo_dialog_panel_1;
    dialog->text_len = PANEL_1_LEN;
    dialog->ticks = 0;
    dialog->tick_limit = dialog->text_len * COMMON_DIALOG_SPEED_SCALE;

    app->dialogs[app->dialog_count++] = dialog;
}
