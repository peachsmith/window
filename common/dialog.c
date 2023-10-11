#include "common/util.h"
#include "common/dialog.h"
#include "common/texture.h"
#include "common/font.h"

static eg_entity *dialogs[COMMON_MAX_DIALOGS];

void common_init_dialogs(eg_app *app)
{
    app->dialogs = &(dialogs[0]);
    app->dialog_count = 0;
}

void common_dialog_renderer(
    eg_app *app,
    eg_entity *dialog,
    eg_font *font,
    eg_entity_callback draw_panel,
    eg_entity_callback draw_indicator)
{
    char buffer[COMMON_DIALOG_BUFSIZE];
    int buf_len;
    int i;

    // Render the dialog panel.
    if (draw_panel != NULL)
    {
        draw_panel(app, dialog);
    }

    // Copy the dialog text into the buffer.
    buf_len = dialog->ticks / COMMON_DIALOG_SPEED_SCALE;
    for (i = 0; i < buf_len; i++)
    {
        if (i < dialog->text_len && i < COMMON_DIALOG_BUFSIZE - 1)
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
        font, // app->fonts[COMMON_FONT_POKEMON_FIRE_RED],
        buffer,
        &bounds,
        NULL);

    // Render the panel advance indicator.
    if (buf_len == dialog->text_len && draw_indicator != NULL)
    {
        draw_indicator(app, dialog);
    }
}
