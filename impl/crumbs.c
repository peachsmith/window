#include "crumbs.h"
#include "impl/impl.h"

//----------------------------------------------------------------------------
// core functions

int cr_initialize()
{
    return cr_impl_init();
}

void cr_terminate()
{
    cr_impl_term();
}

// default update function
static void default_update(cr_app *app)
{
}

// default draw function
static void default_draw(cr_app *app)
{
}

cr_app *cr_create_app()
{
    cr_app *app = NULL;

    // Create the app struct.
    app = (cr_app *)malloc(sizeof(cr_app));
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

    app->screen_width = CR_DEFAULT_SCREEN_WIDTH;
    app->screen_height = CR_DEFAULT_SCREEN_HEIGHT;

    app->cam.x = 0;
    app->cam.y = 0;
    app->cam.cl = 50;
    app->cam.cr = 180;
    app->cam.ct = 20;
    app->cam.cb = 140;

    // Create the implementation struct.
    app->impl = cr_impl_create(
        CR_DEFAULT_SCREEN_WIDTH,
        CR_DEFAULT_SCREEN_HEIGHT,
        app->scale);
    if (app->impl == NULL)
    {
        free(app);
        return NULL;
    }

    // Initialize the key press flags and actuation counters to 0.
    for (int i = 0; i < CR_MAX_KEYCODE; i++)
    {
        app->key_captures[i] = 0;
        app->actuation_counters[i] = 0;
    }

    app->frame_check = 0;

    // generic values
    app->primary = NULL;

    return app;
}

void cr_destroy_app(cr_app *app)
{
    // Destroy the fonts.
    for (int i = 0; i < app->font_count; i++)
    {
        cr_impl_destroy_font(app->fonts[i]);
    }

    // Destroy the textures.
    for (int i = 0; i < app->texture_count; i++)
    {
        cr_impl_destroy_texture(app->textures[i]);
    }

    // Destroy the sounds.
    for (int i = 0; i < app->sound_count; i++)
    {
        cr_impl_destroy_sound(app->sounds[i]);
    }

    // Destroy the implementation.
    cr_impl_destroy(app->impl);

    // Free the memory allocated for the application struct.
    free(app);
}

void cr_set_title(cr_app *app, const char *title)
{
    cr_impl_set_title(app, title);
}

void cr_begin_frame(cr_app *app)
{
    cr_impl_process_events(app);
    cr_impl_clear_screen(app);
}

void cr_end_frame(cr_app *app)
{
    cr_impl_render_screen(app);
    cr_impl_delay(app);
}

//----------------------------------------------------------------------------
// drawing functions

void cr_set_color(cr_app *app, cr_color color)
{
    cr_impl_set_color(app, color);
}

void cr_draw_line(cr_app *app, cr_point *a, cr_point *b)
{
    cr_impl_draw_line(app, a, b);
}

void cr_draw_rect(cr_app *app, cr_rect *r, int filled)
{
    cr_impl_draw_rect(app, r, filled);
}

//----------------------------------------------------------------------------
// input handling functions

int cr_peek_input(cr_app *app, int code)
{
    return cr_impl_peek_key(app, code);
}

int cr_consume_input(cr_app *app, int code)
{
    return cr_impl_consume_key(app, code);
}

void cr_push_input_handler(cr_app *app, cr_func handler)
{
    if (app == NULL || app->input == NULL || handler == NULL)
    {
        return;
    }

    app->input[app->input_count++] = handler;
}

void cr_pop_input_handler(cr_app *app)
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

cr_entity *cr_create_entity(cr_app *app)
{
    cr_entity *entity = NULL;

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

void cr_remove_entity(cr_app *app, cr_entity *entity)
{
    entity->present = 0;
}

int cr_check_flag(cr_entity *e, int f)
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

void cr_set_flag(cr_entity *e, int f)
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

void cr_clear_flag(cr_entity *e, int f)
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

void cr_toggle_flag(cr_entity *e, int f)
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

cr_font *cr_load_font(cr_app *app, const char *path, int p)
{
    return cr_impl_load_font(app, path, p);
}

void cr_draw_text(cr_app *app, cr_font *font, const char *msg, int x, int y)
{
    cr_impl_draw_text(app, font, msg, x, y);
}

void cr_draw_text_bounded(cr_app *app, cr_font *font, const char *msg, cr_rect *bounds, int *result)
{
    cr_impl_draw_text_bounded(app, font, msg, bounds, result);
}

//----------------------------------------------------------------------------
// texture functions

cr_texture *cr_load_texture(cr_app *app, const char *path)
{
    return cr_impl_load_texture(app, path);
}

void cr_draw_texture(cr_app *app, cr_texture *texture, cr_rect *src, cr_rect *dest, int mirror)
{
    cr_impl_draw_texture(app, texture, src, dest, mirror);
}

//----------------------------------------------------------------------------
// audio functions

cr_sound *cr_load_sound(cr_app *app, const char *path, int type)
{
    return cr_impl_load_sound(app, path, type);
}

void cr_play_sound(cr_app *app, cr_sound *sound)
{
    cr_impl_play_sound(app, sound);
}
