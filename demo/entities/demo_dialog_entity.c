#include "demo/entities/demo_dialog_entity.h"
#include "demo/entities/entity_types.h"
#include "demo/util/util.h"
#include "demo/util/ui.h"
#include "demo/texture/texture.h"
#include "demo/font/font.h"

#include <stdio.h>
#include <stdlib.h>

static const char *demo_dialog_panel_1 = "This is a dialog. This is a test of the line\nbreak functionality. How long can we go?";
#define PANEL_1_LEN 85

static const char *demo_dialog_panel_2 = "This is the second panel of text in the demo dialog.";
#define PANEL_2_LEN 52

// The speed scale determines how quickly we will render the text of
// a dialog panel. This may be configured at the application level later.
#define SPEED_SCALE 4

// Limit of the demo dialog text buffer
#define DEMO_DIALOG_ENTITY_BUFSIZE 256

// common dimensions of a dialog panel
#define DEMO_DIALOG_WIDTH 224
#define DEMO_DIALOG_HEIGHT 48

static void render_demo_dialog_entity(eg_app *app, eg_entity *dialog)
{
    char buffer[DEMO_DIALOG_ENTITY_BUFSIZE];
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
    buf_len = dialog->ticks / SPEED_SCALE;
    for (i = 0; i < buf_len; i++)
    {
        if (i < dialog->text_len && i < DEMO_DIALOG_ENTITY_BUFSIZE - 1)
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
        &bounds);

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

static void update_demo_dialog_entity(eg_app *app, eg_entity *dialog)
{
    if (dialog->ticks < dialog->tick_limit)
    {
        dialog->ticks++;
    }
}

static void advance_demo_dialog_entity(eg_app *app, eg_entity *dialog)
{
    // We currently use the data field to represent which panel of content
    // should be rendered in the dialog.

    if (dialog->data >= 1)
    {
        // Close the dialog.
        app->dialog_count--;
        eg_pop_input_handler(app);
        return;
    }

    dialog->text = demo_dialog_panel_2;
    dialog->text_len = PANEL_2_LEN;

    dialog->ticks = 0;
    dialog->tick_limit = dialog->text_len * SPEED_SCALE;

    dialog->data++;
}

void demo_dialog_entity_demo_register(eg_entity_type *t)
{
    // The width and height will be determined in the render function.
    t->width = 10;
    t->height = 10;

    t->render = render_demo_dialog_entity;
    t->update = update_demo_dialog_entity;
    t->advance = advance_demo_dialog_entity;
}

eg_entity *demo_dialog_entity_demo_create()
{
    eg_entity *dialog = NULL;

    dialog = eg_create_entity();
    if (dialog == NULL)
    {
        return NULL;
    }

    dialog->x_pos = 8;
    dialog->y_pos = 108;
    dialog->type = ENTITY_TYPE_DEMO_DIALOG;

    eg_set_flag(dialog, ENTITY_FLAG_PAUSE);
    eg_set_flag(dialog, ENTITY_FLAG_MENU);

    return dialog;
}

void demo_dialog_entity_open(eg_app *app, eg_entity *dialog)
{
    // Reset the dialog.
    dialog->data = 0;
    dialog->text = demo_dialog_panel_1;
    dialog->text_len = PANEL_1_LEN;
    dialog->ticks = 0;
    dialog->tick_limit = dialog->text_len * SPEED_SCALE;

    app->dialog_entities[app->dialog_count++] = dialog;
}
