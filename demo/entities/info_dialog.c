#include "demo/demo.h"
#include "demo/assets.h"
#include "demo/input/input.h"
#include "demo/entities/entity_types.h"
#include "demo/entities/info_dialog.h"
#include "demo/util/ui.h"

#include "common/util.h"
#include "common/dialog.h"

static const char *info_dialog_panel_1 = "What kind of information would you like?";
#define PANEL_1_LEN 40

static const char *purpose_panel = "This software is an exercise in API design.";
#define PURPOSE_PANEL_LEN 43

static const char *implementation_panel = "This software was implemented using\nsomething.";
#define IMPLEMENTATION_PANEL_LEN 46

static void update_info_dialog(cr_app *app, cr_entity *dialog)
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
        cr_entity *info_menu = NULL;
        for (int i = 0; i < app->entity_cap; i++)
        {
            if (app->entities[i].type == ENTITY_TYPE_INFO_MENU && app->entities[i].present)
            {
                info_menu = &(app->entities[i]);
            }
        }

        // Set the pause menu as the active menu.
        app->menus[app->menu_count++] = info_menu;

        cr_push_input_handler(app, demo_info_menu_input);
    }
}

static void advance_info_dialog(cr_app *app, cr_entity *dialog)
{
    dialog->data++;

    if (dialog->data >= 2)
    {
        // Close the dialog.
        app->dialog_count--;
        cr_pop_input_handler(app);
        return;
    }

    if (dialog->data == 1)
    {
        // If no option was selected, close the dialog.
        if (dialog->result == 0)
        {
            // Close the dialog.
            app->dialog_count--;
            cr_pop_input_handler(app);
            return;
        }

        if (dialog->result == 1)
        {
            dialog->text = purpose_panel;
            dialog->text_len = PURPOSE_PANEL_LEN;
            dialog->ticks = 0;
            dialog->tick_limit = dialog->text_len * COMMON_DIALOG_SPEED_SCALE;
            dialog->result = 0;
        }

        if (dialog->result == 2)
        {
            dialog->text = implementation_panel;
            dialog->text_len = IMPLEMENTATION_PANEL_LEN;
            dialog->ticks = 0;
            dialog->tick_limit = dialog->text_len * COMMON_DIALOG_SPEED_SCALE;
            dialog->result = 0;
        }

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

void demo_register_info_dialog(cr_entity_type *t)
{
    // The width and height will be determined in the render function.
    t->width = 10;
    t->height = 10;

    t->render = render_dialog;
    t->update = update_info_dialog;
    t->advance = advance_info_dialog;
}

cr_entity *demo_create_info_dialog(cr_app* app)
{
    cr_entity *dialog = NULL;

    dialog = cr_create_entity(app);
    if (dialog == NULL)
    {
        return NULL;
    }

    dialog->x_pos = 8;
    dialog->y_pos = 108;
    dialog->type = ENTITY_TYPE_INFO_DIALOG;

    cr_set_flag(dialog, ENTITY_FLAG_PAUSE);
    cr_set_flag(dialog, ENTITY_FLAG_MENU);

    return dialog;
}

void demo_open_info_dialog(cr_app *app, cr_entity *dialog)
{
    // Reset the dialog.
    dialog->data = 0;
    dialog->text = info_dialog_panel_1;
    dialog->text_len = PANEL_1_LEN;
    dialog->ticks = 0;
    dialog->tick_limit = dialog->text_len * COMMON_DIALOG_SPEED_SCALE;

    app->dialogs[app->dialog_count++] = dialog;
}
