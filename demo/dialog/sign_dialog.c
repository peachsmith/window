#include "demo/dialog/dialog.h"
#include "demo/font/font.h"
#include "demo/util/util.h"
#include "demo/input/input.h"
#include "demo/menu/menu.h"

#include <stdio.h>

static eg_dialog sign_dialog;

static const char *panel_1_text = "This is a very informative sign.\nTrust me.";
#define PANEL_1_LEN 42

// static const char *panel_2_text = "What are you looking at?";
// #define PANEL_2_LEN 24

static void update_sign_dialog(eg_app *, eg_dialog *);
static void advance_sign_dialog(eg_app *, eg_dialog *);

void demo_init_sign_dialog(eg_app *app)
{
    sign_dialog.position.x = 8;
    sign_dialog.position.y = 108;
    sign_dialog.w = 224;
    sign_dialog.h = 48;
    sign_dialog.speed_scale = 4;
    sign_dialog.panel = 0;
    sign_dialog.result = 0;

    sign_dialog.text = panel_1_text;
    sign_dialog.text_len = PANEL_1_LEN;

    sign_dialog.ticks = 0;
    sign_dialog.tick_limit = sign_dialog.text_len * sign_dialog.speed_scale;

    sign_dialog.render = demo_render_dialog;
    sign_dialog.update = update_sign_dialog;
    sign_dialog.advance = advance_sign_dialog;
}

void demo_open_sign_dialog(eg_app *app)
{
    eg_push_input_handler(app, common_dialog_input_handler);

    // Reset the dialog.
    sign_dialog.panel = 0;
    sign_dialog.text = panel_1_text;
    sign_dialog.text_len = PANEL_1_LEN;
    sign_dialog.ticks = 0;
    sign_dialog.tick_limit = sign_dialog.text_len * sign_dialog.speed_scale;
    sign_dialog.result = 0;

    app->dialogs[app->dialog_count++] = &sign_dialog;
}

static void update_sign_dialog(eg_app *app, eg_dialog *dialog)
{
    if (dialog->ticks < dialog->tick_limit)
    {
        dialog->ticks++;
    }
}

static void advance_sign_dialog(eg_app *app, eg_dialog *dialog)
{
    dialog->panel++;

    if (dialog->panel >= 1)
    {
        // Close the dialog.
        app->dialog_count--;
        eg_pop_input_handler(app);
        app->pause = 0;
        return;
    }

    // if (dialog->panel == 1)
    // {
    //     sign_dialog.text = panel_2_text;
    //     sign_dialog.text_len = PANEL_2_LEN;
    //     sign_dialog.ticks = 0;
    //     sign_dialog.tick_limit = sign_dialog.text_len * sign_dialog.speed_scale;
    //     sign_dialog.result = 0;

    //     return;
    // }
}
