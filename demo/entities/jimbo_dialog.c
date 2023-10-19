#include "demo/demo.h"
#include "demo/assets.h"
#include "demo/input/input.h"
#include "demo/entities/jimbo_dialog.h"
#include "demo/entities/entity_types.h"
#include "demo/util/ui.h"

#include "common/util.h"
#include "common/dialog.h"

static const char *panel_1_text = "Howdy!\nI'm an NPC.";
#define PANEL_1_LEN 18

static void update_jimbo_dialog(cr_app *app, cr_entity *dialog)
{
    if (dialog->ticks < dialog->tick_limit)
    {
        dialog->ticks++;
    }
}

static void advance_jimbo_dialog(cr_app *app, cr_entity *dialog)
{
    dialog->data++;

    if (dialog->data >= 1)
    {
        // Close the dialog.
        app->dialog_count--;
        cr_pop_input_handler(app);
        app->pause = 0;
        return;
    }
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

void demo_register_jimbo_dialog(cr_entity_type *t)
{
    // The width and height will be determined in the render function.
    t->width = 10;
    t->height = 10;

    t->render = render_dialog;
    t->update = update_jimbo_dialog;
    t->advance = advance_jimbo_dialog;
}

cr_entity *demo_create_jimbo_dialog(cr_app* app)
{
    cr_entity *dialog = NULL;

    dialog = cr_create_entity(app);
    if (dialog == NULL)
    {
        return NULL;
    }

    dialog->x_pos = 8;
    dialog->y_pos = 108;
    dialog->type = ENTITY_TYPE_JIMBO_DIALOG;

    cr_set_flag(dialog, ENTITY_FLAG_PAUSE);
    cr_set_flag(dialog, ENTITY_FLAG_MENU);

    return dialog;
}

void demo_open_jimbo_dialog(cr_app *app, cr_entity *dialog)
{
    // Reset the dialog.
    dialog->data = 0;
    dialog->text = panel_1_text;
    dialog->text_len = PANEL_1_LEN;
    dialog->ticks = 0;
    dialog->tick_limit = dialog->text_len * COMMON_DIALOG_SPEED_SCALE;

    app->dialogs[app->dialog_count++] = dialog;
}
