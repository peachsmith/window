#include "demo/dialog/dialog.h"
#include "demo/input/input.h"
#include "demo/util/util.h"

#include <stdio.h>

#define MAX_DIALOGS 10
#define DIALOG_BUFSIZE 256

// The tick limit for a single panel of dialog.
#define DEMO_DIALOG_TICK_MAX 68

static const char *demo_dialog_text = "This is a dialog.";

static void render_demo_dialog(eg_app *app, eg_dialog *dialog);
static void update_demo_dialog(eg_app *app, eg_dialog *dialog);

static void render_demo_dialog(eg_app *app, eg_dialog *dialog)
{
    // Render the menu panel.
    eg_rect rect = {
        .x = dialog->position.x,
        .y = dialog->position.y,
        .w = 14,
        .h = 3};
    demo_draw_panel(app, &rect);

    // Render the text.
    char buffer[DIALOG_BUFSIZE];
    buffer[0] = '\0';

    // Copy the dialog text into the buffer.
    int i;
    for (i = 0; i < dialog->ticks / 4; i++)
    {
        if (i < 81 && i < DIALOG_BUFSIZE - 1)
        {
            buffer[i] = demo_dialog_text[i];
        }
    }

    // Append a string terminator NUL character.
    buffer[i] = '\0';

    // Render the dialog text.
    eg_draw_text(app,
                 buffer,
                 dialog->position.x + 5,
                 dialog->position.y + 5);
}

static void update_demo_dialog(eg_app *app, eg_dialog *dialog)
{
    if (dialog->ticks < DEMO_DIALOG_TICK_MAX)
    {
        dialog->ticks++;
    }
}

static eg_dialog demo_dialog;

static eg_dialog *dialogs[MAX_DIALOGS];

void demo_init_dialogs(eg_app *app)
{
    //----------------------------------
    // BEGIN init demo dialog

    demo_dialog.position.x = 8;
    demo_dialog.position.y = 108;
    demo_dialog.render = render_demo_dialog;
    demo_dialog.update = update_demo_dialog;
    demo_dialog.text = demo_dialog_text;
    demo_dialog.ticks = 0;

    // END init demo dialog
    //----------------------------------

    app->dialogs = &(dialogs[0]);
    app->dialog_count = 0;
}

void demo_open_demo_dialog(eg_app *app)
{
    eg_input_handler *handler = eg_create_input_handler(demo_dialog_input_callback, NULL);
    eg_push_input_handler(app, handler);

    // Reset the dialog's tick counter.
    demo_dialog.ticks = 0;

    app->dialogs[app->dialog_count] = &demo_dialog;
    app->dialog_count++;
}
