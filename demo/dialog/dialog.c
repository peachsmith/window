#include "demo/dialog/dialog.h"
#include "demo/input/input.h"
#include "demo/util/util.h"
#include "demo/util/ui.h"
#include "demo/font/font.h"
#include "demo/texture/texture.h"

#include <stdio.h>

static eg_dialog *dialogs[DEMO_MAX_DIALOGS];

void demo_init_dialogs(eg_app *app)
{
    demo_init_demo_dialog(app);
    demo_init_info_dialog(app);

    app->dialogs = &(dialogs[0]);
    app->dialog_count = 0;
}

void demo_render_dialog(eg_app *app, eg_dialog *dialog)
{
    char buffer[DEMO_DIALOG_BUFSIZE];
    int buf_len;
    int i;

    // Render the menu panel.
    ui_draw_panel(
        app,
        dialog->position.x,
        dialog->position.y,
        dialog->w,
        dialog->h);

    // Copy the dialog text into the buffer.
    buf_len = dialog->ticks / dialog->speed_scale;
    for (i = 0; i < buf_len; i++)
    {
        if (i < dialog->text_len && i < DEMO_DIALOG_BUFSIZE - 1)
        {
            buffer[i] = dialog->text[i];
        }
    }

    // Terminate the buffer with a NUL character.
    buffer[i] = '\0';

    // Render the dialog text.
    eg_rect bounds = {
        .x = dialog->position.x + 5,
        .y = dialog->position.y + 5,
        .w = 210,
        .h = 0};
    eg_draw_text_bounded(
        app,
        app->fonts[DEMO_FONT_POKEMON_FIRE_RED],
        buffer,
        &bounds);

    // Render the panel advance indicator.
    if (buf_len == dialog->text_len)
    {
        ui_draw_indicator(
            app,
            dialog->position.x + dialog->w - 18,
            dialog->position.y + dialog->h - 18);
    }
}
