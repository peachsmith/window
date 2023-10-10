#include "demo/dialog/dialog.h"
#include "demo/input/input.h"
#include "demo/util/util.h"
#include "demo/util/ui.h"
#include "demo/font/font.h"
#include "demo/texture/texture.h"

#include <stdio.h>

static eg_entity *dialogs[DEMO_MAX_DIALOGS];

void demo_init_dialogs(eg_app *app)
{
    app->dialogs = &(dialogs[0]);
    app->dialog_count = 0;
}

void demo_common_dialog_renderer(eg_app *app, eg_entity *dialog)
{
    char buffer[DEMO_DIALOG_BUFSIZE];
    int buf_len;
    int i;

    // Render the dialog panel.
    ui_draw_panel(
        app,
        dialog->x_pos,
        dialog->y_pos,
        DEMO_DIALOG_WIDTH,
        DEMO_DIALOG_HEIGHT);

    // Copy the dialog text into the buffer.
    buf_len = dialog->ticks / DEMO_DIALOG_SPEED_SCALE;
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
        .x = dialog->x_pos + 5,
        .y = dialog->y_pos + 5,
        .w = 210,
        .h = 0};
    eg_draw_text_bounded(
        app,
        app->fonts[DEMO_FONT_POKEMON_FIRE_RED],
        buffer,
        &bounds,
        NULL);

    // Render the panel advance indicator.
    if (buf_len == dialog->text_len)
    {
        ui_draw_indicator(
            app,
            dialog->x_pos + DEMO_DIALOG_WIDTH - 18,
            dialog->y_pos + DEMO_DIALOG_HEIGHT - 18,
            UI_INDICATOR_ADVANCE);
    }
}
