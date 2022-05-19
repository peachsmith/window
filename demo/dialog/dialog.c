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
    // Render the menu panel.
    ui_draw_panel(app, dialog->position.x, dialog->position.y, dialog->w, dialog->h);
    // eg_rect rect = {
    //     .x = dialog->position.x,
    //     .y = dialog->position.y,
    //     .w = 14,
    //     .h = 3};
    // demo_draw_panel(app, &rect);

    // Render the text.
    char buffer[DEMO_DIALOG_BUFSIZE];
    buffer[0] = '\0';

    // Copy the dialog text into the buffer.
    int i;
    int buf_len = dialog->ticks / dialog->speed_scale;
    for (i = 0; i < buf_len; i++)
    {
        if (i < dialog->text_len && i < DEMO_DIALOG_BUFSIZE - 1)
        {
            buffer[i] = dialog->text[i];
        }
    }

    // Append a string terminator NUL character.
    buffer[i] = '\0';

    // Render the dialog text.
    eg_rect bounds = {
        .x = dialog->position.x + 5,
        .y = dialog->position.y + 5,
        .w = 210,
        .h = 0};
    eg_draw_text_bounded(app,
                         app->fonts[DEMO_FONT_POKEMON_FIRE_RED],
                         buffer,
                         &bounds);

    // Render the panel advance indicator.
    if (buf_len == dialog->text_len)
    {
        eg_rect ind_src = {
            .x = 18 * 5,
            .y = 18 * 10,
            .w = 16,
            .h = 16};
        eg_rect ind_dest = {
            .x = dialog->position.x + 206,
            .y = dialog->position.y + 30,
            .w = 16,
            .h = 16};
        eg_draw_texture(
            app,
            app->textures[DEMO_TEXTURE_UI],
            &ind_src,
            &ind_dest);
    }
}
