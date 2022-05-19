#include "demo/dialog/dialog.h"
#include "demo/font/font.h"
#include "demo/util/util.h"
#include "demo/input/input.h"
#include "demo/menu/menu.h"

#include <stdio.h>

static eg_dialog info_dialog;

static const char *info_dialog_panel_1 = "What kind of information would you like?";
#define PANEL_1_LEN 40

static const char *purpose_panel = "This software is an exercise in API design.";
#define PURPOSE_PANEL_LEN 43

static const char *implementation_panel = "This software was implemented using SDL2.";
#define IMPLEMENTATION_PANEL_LEN 41

static void update_info_dialog(eg_app *, eg_dialog *);
static void advance_info_dialog(eg_app *, eg_dialog *);

void demo_init_info_dialog(eg_app *app)
{
    info_dialog.position.x = 8;
    info_dialog.position.y = 108;
    info_dialog.speed_scale = 4;
    info_dialog.panel = 0;
    info_dialog.result = 0;

    info_dialog.text = info_dialog_panel_1;
    info_dialog.text_len = PANEL_1_LEN;

    info_dialog.ticks = 0;
    info_dialog.tick_limit = info_dialog.text_len * info_dialog.speed_scale;

    info_dialog.render = demo_render_dialog;
    info_dialog.update = update_info_dialog;
    info_dialog.advance = advance_info_dialog;
}

void demo_open_info_dialog(eg_app *app)
{
    eg_push_input_handler(app, demo_dialog_input_handler);

    // Reset the dialog.
    info_dialog.panel = 0;
    info_dialog.text = info_dialog_panel_1;
    info_dialog.text_len = PANEL_1_LEN;
    info_dialog.ticks = 0;
    info_dialog.tick_limit = info_dialog.text_len * info_dialog.speed_scale;
    info_dialog.result = 0;

    app->dialogs[app->dialog_count++] = &info_dialog;
}

static void update_info_dialog(eg_app *app, eg_dialog *dialog)
{
    if (dialog->ticks < dialog->tick_limit)
    {
        dialog->ticks++;
    }

    // If we've reached the end of the first panel, open the info menu.
    if (dialog->panel == 0 && dialog->ticks == dialog->tick_limit)
    {
        dialog->ticks++;
        demo_open_info_menu(app);
    }
}

static void advance_info_dialog(eg_app *app, eg_dialog *dialog)
{
    dialog->panel++;

    if (dialog->panel >= 2)
    {
        // Close the dialog.
        app->dialog_count--;
        eg_pop_input_handler(app);
        return;
    }

    if (dialog->panel == 1)
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
            info_dialog.text = purpose_panel;
            info_dialog.text_len = PURPOSE_PANEL_LEN;
            info_dialog.ticks = 0;
            info_dialog.tick_limit = info_dialog.text_len * info_dialog.speed_scale;
            info_dialog.result = 0;
        }

        if (dialog->result == 2)
        {
            info_dialog.text = implementation_panel;
            info_dialog.text_len = IMPLEMENTATION_PANEL_LEN;
            info_dialog.ticks = 0;
            info_dialog.tick_limit = info_dialog.text_len * info_dialog.speed_scale;
            info_dialog.result = 0;
        }

        return;
    }
}
