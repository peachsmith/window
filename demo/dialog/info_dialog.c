#include "demo/dialog/dialog.h"
#include "demo/font/font.h"
#include "demo/util/util.h"
#include "demo/input/input.h"
#include "demo/menu/menu.h"

#include <stdio.h>

static eg_dialog info_dialog;

// 44 characters
static const char *info_dialog_panel_1 = "This software is an excersise in API design.";

// 29 characters
// static const char *info_dialog_panel_2 = "It is implemented using SDL2.";

static void update_info_dialog(eg_app *, eg_dialog *);
static void advance_info_dialog(eg_app *, eg_dialog *);

void demo_init_info_dialog(eg_app *app)
{
    info_dialog.position.x = 8;
    info_dialog.position.y = 108;
    info_dialog.speed_scale = 4;
    info_dialog.panel = 0;

    info_dialog.text = info_dialog_panel_1;
    info_dialog.text_len = 44;

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
    info_dialog.text_len = 44;
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
}

static void advance_info_dialog(eg_app *app, eg_dialog *dialog)
{
    if (dialog->panel >= 1)
    {
        printf("info result: %d\n", dialog->result);

        // Close the dialog.
        app->dialog_count--;
        eg_pop_input_handler(app);
        return;
    }

    eg_push_input_handler(app, info_input_handler);
    demo_open_info_menu(app);
    dialog->panel++;

    // dialog->text = info_dialog_panel_2;
    // dialog->text_len = 29;
    // dialog->ticks = 0;
    // dialog->tick_limit = dialog->text_len * dialog->speed_scale;
    // dialog->panel++;
}
