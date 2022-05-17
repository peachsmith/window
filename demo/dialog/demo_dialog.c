#include "demo/dialog/dialog.h"
#include "demo/font/font.h"
#include "demo/util/util.h"
#include "demo/input/input.h"

#include <stdio.h>

static eg_dialog demo_dialog;

// 85 characters
static const char *demo_dialog_panel_1 = "This is a dialog. This is a test of the line break functionality. How long can we go?";

// 52 characters
static const char *demo_dialog_panel_2 = "This is the second panel of text in the demo dialog.";

static void update_demo_dialog(eg_app *, eg_dialog *);
static void advance_demo_dialog(eg_app *, eg_dialog *);

void demo_init_demo_dialog(eg_app *app)
{
    demo_dialog.position.x = 8;
    demo_dialog.position.y = 108;
    demo_dialog.speed_scale = 4;
    demo_dialog.panel = 0;

    demo_dialog.text = demo_dialog_panel_1;
    demo_dialog.text_len = 85;

    demo_dialog.ticks = 0;
    demo_dialog.tick_limit = demo_dialog.text_len * demo_dialog.speed_scale;

    demo_dialog.render = demo_render_dialog;
    demo_dialog.update = update_demo_dialog;
    demo_dialog.advance = advance_demo_dialog;
}

void demo_open_demo_dialog(eg_app *app)
{
    eg_push_input_handler(app, demo_dialog_input_handler);

    // Reset the dialog.
    demo_dialog.panel = 0;
    demo_dialog.text = demo_dialog_panel_1;
    demo_dialog.text_len = 85;
    demo_dialog.ticks = 0;
    demo_dialog.tick_limit = demo_dialog.text_len * demo_dialog.speed_scale;

    app->dialogs[app->dialog_count] = &demo_dialog;
    app->dialog_count++;
}

static void update_demo_dialog(eg_app *app, eg_dialog *dialog)
{
    if (dialog->ticks < dialog->tick_limit)
    {
        dialog->ticks++;
    }
}

static void advance_demo_dialog(eg_app *app, eg_dialog *dialog)
{
    if (dialog->panel >= 1)
    {
        // Close the dialog.
        app->dialog_count--;
        eg_pop_input_handler(app);
        return;
    }

    dialog->text = demo_dialog_panel_2;
    dialog->text_len = 52;
    dialog->ticks = 0;
    dialog->tick_limit = dialog->text_len * dialog->speed_scale;
    dialog->panel++;
}
