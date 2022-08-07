#include "demo/dialog/dialog.h"
#include "demo/font/font.h"
#include "demo/util/util.h"
#include "demo/input/input.h"
#include "demo/menu/menu.h"

#include <stdio.h>

static eg_dialog jimbo_dialog;

static const char *panel_1_text = "Howdy!\nI'm an NPC.";
#define PANEL_1_LEN 18

static void update_jimbo_dialog(eg_app *, eg_dialog *);
static void advance_jimbo_dialog(eg_app *, eg_dialog *);

void demo_init_jimbo_dialog(eg_app *app)
{
    jimbo_dialog.position.x = 8;
    jimbo_dialog.position.y = 108;
    jimbo_dialog.w = 224;
    jimbo_dialog.h = 48;
    jimbo_dialog.speed_scale = 4;
    jimbo_dialog.panel = 0;
    jimbo_dialog.result = 0;

    jimbo_dialog.text = panel_1_text;
    jimbo_dialog.text_len = PANEL_1_LEN;

    jimbo_dialog.ticks = 0;
    jimbo_dialog.tick_limit = jimbo_dialog.text_len * jimbo_dialog.speed_scale;

    jimbo_dialog.render = demo_render_dialog;
    jimbo_dialog.update = update_jimbo_dialog;
    jimbo_dialog.advance = advance_jimbo_dialog;
}

void demo_open_jimbo_dialog(eg_app *app)
{
    eg_push_input_handler(app, common_dialog_input_handler);

    // Reset the dialog.
    jimbo_dialog.panel = 0;
    jimbo_dialog.text = panel_1_text;
    jimbo_dialog.text_len = PANEL_1_LEN;
    jimbo_dialog.ticks = 0;
    jimbo_dialog.tick_limit = jimbo_dialog.text_len * jimbo_dialog.speed_scale;
    jimbo_dialog.result = 0;

    app->dialogs[app->dialog_count++] = &jimbo_dialog;
}

static void update_jimbo_dialog(eg_app *app, eg_dialog *dialog)
{
    if (dialog->ticks < dialog->tick_limit)
    {
        dialog->ticks++;
    }
}

static void advance_jimbo_dialog(eg_app *app, eg_dialog *dialog)
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
}
