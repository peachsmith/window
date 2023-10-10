#include "example.h"
#include "colors.h"
#include "impl/impl.h"

#include <stdio.h>
#include <stdlib.h>

//----------------------------------------------------------------------------
// core functions

int eg_initialize()
{
    return eg_impl_init();
}

void eg_terminate()
{
    eg_impl_term();
}

// default update function
static void default_update(eg_app *app)
{
}

// default draw function
static void default_draw(eg_app *app)
{
}

eg_app *eg_create_app()
{
    eg_app *app = NULL;

    // Create the app struct.
    app = (eg_app *)malloc(sizeof(eg_app));
    if (app == NULL)
    {
        return NULL;
    }

    // Ensure that all pointers are NULL or have a default value.
    app->impl = NULL;

    // Populate the debug structure with the default values.
    app->debug.overlay = 0;
    app->debug.hitboxes = 0;
    app->debug.camera = 0;
    app->debug.collisions = 0;
    app->debug.frame_len = 1;
    app->debug.frame_by_frame = 0;
    app->debug.fps = 0;

    app->scale = 3;

    app->time = TIMING_DELTA;

    app->done = 0;
    app->pause = 0;
    app->ticks = 0;
    app->scene = 0;

    app->update = default_update;
    app->draw = default_draw;

    app->entity_count = 0;
    app->entity_cap = 256;

    app->input = NULL;
    app->input_count = 0;

    app->fonts = NULL;
    app->font_count = 0;

    app->sounds = NULL;
    app->sound_count = 0;

    app->textures = NULL;
    app->texture_count = 0;

    app->menus = NULL;
    app->menu_count = 0;

    app->dialogs = NULL;
    app->dialog_count = 0;

    app->screen_width = EG_DEFAULT_SCREEN_WIDTH;
    app->screen_height = EG_DEFAULT_SCREEN_HEIGHT;

    app->cam.x = 0;
    app->cam.y = 0;
    app->cam.cl = 50;
    app->cam.cr = 180;
    app->cam.ct = 20;
    app->cam.cb = 140;

    // Create the implementation struct.
    app->impl = eg_impl_create(
        EG_DEFAULT_SCREEN_WIDTH,
        EG_DEFAULT_SCREEN_HEIGHT,
        app->scale);
    if (app->impl == NULL)
    {
        free(app);
        return NULL;
    }

    // Initialize the key press flags and actuation counters to 0.
    for (int i = 0; i < EG_MAX_KEYCODE; i++)
    {
        app->key_captures[i] = 0;
        app->actuation_counters[i] = 0;
    }

    app->frame_check = 0;

    // generic values
    app->primary = NULL;

    return app;
}

void eg_destroy_app(eg_app *app)
{
    // Destroy the fonts.
    for (int i = 0; i < app->font_count; i++)
    {
        eg_impl_destroy_font(app->fonts[i]);
    }

    // Destroy the textures.
    for (int i = 0; i < app->texture_count; i++)
    {
        eg_impl_destroy_texture(app->textures[i]);
    }

    // Destroy the sounds.
    for (int i = 0; i < app->sound_count; i++)
    {
        eg_impl_destroy_sound(app->sounds[i]);
    }

    // Destroy the implementation.
    eg_impl_destroy(app->impl);

    // Free the memory allocated for the application struct.
    free(app);
}

void eg_set_title(eg_app *app, const char *title)
{
    eg_impl_set_title(app, title);
}

void eg_begin_frame(eg_app *app)
{
    eg_impl_process_events(app);
    eg_impl_clear_screen(app);
}

void eg_end_frame(eg_app *app)
{
    eg_impl_render_screen(app);
    eg_impl_delay(app);
}

//----------------------------------------------------------------------------
// drawing functions

void eg_set_color(eg_app *app, eg_color color)
{
    eg_impl_set_color(app, color);
}

void eg_draw_line(eg_app *app, eg_point *a, eg_point *b)
{
    eg_impl_draw_line(app, a, b);
}

void eg_draw_rect(eg_app *app, eg_rect *r, int filled)
{
    eg_impl_draw_rect(app, r, filled);
}

//----------------------------------------------------------------------------
// input handling functions

int eg_peek_input(eg_app *app, int code)
{
    return eg_impl_peek_key(app, code);
}

int eg_consume_input(eg_app *app, int code)
{
    return eg_impl_consume_key(app, code);
}

void eg_push_input_handler(eg_app *app, eg_callback handler)
{
    if (app == NULL || app->input == NULL || handler == NULL)
    {
        return;
    }

    app->input[app->input_count++] = handler;
}

void eg_pop_input_handler(eg_app *app)
{
    // If the stack is empty, then return without doing anything.
    if (app == NULL || app->input == NULL)
    {
        return;
    }

    if (app->input_count < 1)
    {
        printf("[WARN] attempting to pop input handler from empty stack\n");
        return;
    }

    app->input_count--;
}

//----------------------------------------------------------------------------
// entity functions

eg_entity *eg_create_entity(eg_app *app)
{
    eg_entity *entity = NULL;

    // Look for the available entity slot.
    for (int i = 0; i < app->entity_cap && entity == NULL; i++)
    {
        if (!app->entities[i].present)
        {
            entity = &(app->entities[i]);
        }
    }

    if (entity == NULL)
    {
        printf("[WARN] entity list is full\n");
        return NULL;
    }

    entity->present = 1;
    entity->type = 0;
    entity->x_pos = 0;
    entity->y_pos = 0;
    entity->x_vel = 0;
    entity->y_vel = 0;
    entity->x_acc = 0;
    entity->y_acc = 0;
    entity->x_t = 0;
    entity->y_t = 0;
    entity->flags = 0;
    entity->data = 0;
    entity->animation_ticks = 0;
    entity->ticks = 0;
    entity->iframes = 0;
    entity->carrier = NULL;
    entity->text = NULL;
    entity->text_len = 0;
    entity->tick_limit = 0;
    entity->result = 0;
    entity->cursor_x = 0;
    entity->cursor_y = 0;
    entity->scroll_y = 0;
    entity->scroll_y = 0;

    return entity;
}

void eg_remove_entity(eg_app *app, eg_entity *entity)
{
    entity->present = 0;
}

int eg_check_flag(eg_entity *e, int f)
{
    // The flag index must be in the range [0, 15] since we're only dealing
    // with 16 bits.
    if (f < 0 || f >= 16)
    {
        return 0;
    }

    // Convert the flag index to a bit value.
    uint16_t bit = (uint16_t)(1 << f);

    // Check if the flag is set.
    if (e->flags & bit)
    {
        return 1;
    }

    return 0;
}

void eg_set_flag(eg_entity *e, int f)
{
    // The flag index must be in the range [0, 15] since we're only dealing
    // with 16 bits.
    if (f < 0 || f >= 16)
    {
        return;
    }

    // Convert the flag index to a bit value.
    uint16_t bit = (uint16_t)(1 << f);

    // Set the flag.
    e->flags |= bit;
}

void eg_clear_flag(eg_entity *e, int f)
{
    // The flag index must be in the range [0, 15] since we're only dealing
    // with 16 bits.
    if (f < 0 || f >= 16)
    {
        return;
    }

    // Convert the flag index to a bit value.
    uint16_t bit = (uint16_t)(1 << f);

    // Clear the flag.
    e->flags &= ~bit;
}

void eg_toggle_flag(eg_entity *e, int f)
{
    // The flag index must be in the range [0, 15] since we're only dealing
    // with 16 bits.
    if (f < 0 || f >= 16)
    {
        return;
    }

    // Convert the flag index to a bit value.
    uint16_t bit = (uint16_t)(1 << f);

    // If the flag is already set, then clear it.
    if (e->flags & bit)
    {
        e->flags &= ~bit;
        return;
    }

    // Set the flag.
    e->flags |= bit;
}

//----------------------------------------------------------------------------
// font functions

eg_font *eg_load_font(eg_app *app, const char *path, int p)
{
    return eg_impl_load_font(app, path, p);
}

void eg_draw_text(eg_app *app, eg_font *font, const char *msg, int x, int y)
{
    eg_impl_draw_text(app, font, msg, x, y);
}

void eg_draw_text_bounded(eg_app *app, eg_font *font, const char *msg, eg_rect *bounds, int *result)
{
    eg_impl_draw_text_bounded(app, font, msg, bounds, result);
}

//----------------------------------------------------------------------------
// texture functions

eg_texture *eg_load_texture(eg_app *app, const char *path)
{
    return eg_impl_load_texture(app, path);
}

void eg_draw_texture(eg_app *app, eg_texture *texture, eg_rect *src, eg_rect *dest, int mirror)
{
    eg_impl_draw_texture(app, texture, src, dest, mirror);
}

//----------------------------------------------------------------------------
// audio functions

eg_sound *eg_load_sound(eg_app *app, const char *path, int type)
{
    return eg_impl_load_sound(app, path, type);
}

void eg_play_sound(eg_app *app, eg_sound *sound)
{
    eg_impl_play_sound(app, sound);
}
