#include "example.h"
#include "colors.h"

#include <stdio.h>
#include <stdlib.h>

// defined in example_impl.c.
int eg_impl_init();
void eg_impl_term();
eg_impl *eg_impl_create(int, int);
void eg_impl_destroy(eg_impl *);
void eg_impl_process_events(eg_app *);
int eg_impl_delay(eg_app *);
void eg_impl_clear_screen(eg_app *);
void eg_impl_render_screen(eg_app *);
void eg_impl_set_color(eg_app *app, uint32_t color);
void eg_impl_draw_line(eg_app *app, eg_point *a, eg_point *b);
void eg_impl_draw_rect(eg_app *app, eg_rect *r, int filled);
int eg_impl_peek_key(eg_app *, int);
int eg_impl_consume_key(eg_app *, int);
int eg_impl_load_font(eg_app *, const char *, int);
void eg_impl_draw_text(eg_app *, const char *, int, int);
int eg_impl_load_sprite_sheet(eg_app *, const char *);
void eg_impl_draw_image(eg_app *, eg_rect *, eg_rect *);

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
    app->entities = NULL;
    app->input = NULL;
    app->update = default_update;
    app->draw = default_draw;

    // Create the implementation struct.
    app->impl = eg_impl_create(
        EG_DEFAULT_SCREEN_WIDTH,
        EG_DEFAULT_SCREEN_HEIGHT);
    if (app->impl == NULL)
    {
        free(app);
        return NULL;
    }

    // Initialize the key press flags to 0.
    for (int i = 0; i < EG_MAX_KEYCODE; i++)
    {
        app->key_captures[i] = 0;
    }

    // Set the main loop sentinel value to 0.
    app->done = 0;

    // Set the camera position to (0, 0).
    app->cam.x = 0;
    app->cam.y = 0;

    // TEMP: camera boudnaries for debugging.
    app->cl = 50;
    app->cr = 180;
    app->ct = 20;
    app->cb = 140;
    app->screen_width = EG_DEFAULT_SCREEN_WIDTH;
    app->screen_height = EG_DEFAULT_SCREEN_HEIGHT;

    app->pause = 0;
    app->menus = NULL;
    app->menu_count = 0;
    app->dialogs = NULL;
    app->dialog_count = 0;

    return app;
}

void eg_destroy_app(eg_app *app)
{
    // Destroy the entities.
    while (app->entities != NULL)
    {
        eg_entity *next = app->entities->next;
        eg_destroy_entity(app->entities);
        app->entities = next;
    }

    // Destroy the input handlers.
    while (app->input != NULL)
    {
        eg_input_handler *previous = app->input->previous;
        eg_destroy_input_handler(app->input);
        app->input = previous;
    }

    // Destroy the entity registry.
    eg_destroy_registry(app->registry);

    // Destroy the implementation.
    eg_impl_destroy(app->impl);

    // Free the memory allocated for the application struct.
    free(app);
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

eg_input_handler *eg_create_input_handler(eg_callback callback, eg_entity *target)
{
    eg_input_handler *handler = NULL;

    handler = (eg_input_handler *)malloc(sizeof(eg_input_handler));
    if (handler == NULL)
    {
        return NULL;
    }

    handler->previous = NULL;
    handler->callback = callback;
    handler->target = target;

    return handler;
}

void eg_destroy_input_handler(eg_input_handler *handler)
{
    free(handler);
}

void eg_push_input_handler(eg_app *app, eg_input_handler *handler)
{
    if (app == NULL || handler == NULL)
    {
        return;
    }

    // If the stack is empty, then the handler becomes the top of the stack.
    if (app->input == NULL)
    {
        app->input = handler;
        return;
    }

    // Save the reference to the current handler in the new handler.
    handler->previous = app->input;

    // Set the application's current input handler to be the new handler.
    app->input = handler;
}

eg_input_handler *eg_pop_input_handler(eg_app *app)
{
    // If the stack is empty, then return without doing anything.
    if (app == NULL || app->input == NULL)
    {
        return NULL;
    }

    // Get the top of the input handler stack.
    // This will be returned so that it can be saved or destroyed.
    eg_input_handler *current = app->input;

    // The previous input handler now becomes the current input handler.
    app->input = app->input->previous;

    return current;
}

//----------------------------------------------------------------------------
// entity functions

eg_entity_type *eg_create_registry(int n)
{
    eg_entity_type *reg = (eg_entity_type *)
        malloc(sizeof(eg_entity_type) * n);

    if (reg == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < n; i++)
    {
        reg[i].id = 0;
        reg[i].width = 0;
        reg[i].height = 0;
        reg[i].render = NULL;
        reg[i].update = NULL;
        reg[i].collide = NULL;
    }

    return reg;
}

void eg_destroy_registry(eg_entity_type *reg)
{
    if (reg == NULL)
    {
        return;
    }

    free(reg);
}

eg_entity *eg_create_entity()
{
    eg_entity *entity = NULL;

    entity = (eg_entity *)malloc(sizeof(eg_entity));
    if (entity == NULL)
    {
        return NULL;
    }

    entity->id = 0;
    entity->type = 0;
    entity->x_pos = 0;
    entity->y_pos = 0;
    entity->x_vel = 0;
    entity->y_vel = 0;
    entity->flags = 0;
    entity->ticks = 0;
    entity->carrier = NULL;
    entity->next = NULL;
    entity->previous = NULL;

    return entity;
}

void eg_destroy_entity(eg_entity *entity)
{
    if (entity == NULL)
    {
        return;
    }

    free(entity);
}

void eg_add_entity(eg_app *app, eg_entity *entity)
{
    if (app == NULL || entity == NULL)
    {
        return;
    }

    // If the entity list is empty, then simply point it to the newly added
    // entity.
    if (app->entities == NULL)
    {
        app->entities = entity;
        return;
    }

    // Add the entity to the front of the list.
    entity->next = app->entities;
    app->entities->previous = entity;

    // Update the entity list pointer in the app struct to point to the new
    // entity.
    app->entities = entity;
}

eg_entity *eg_remove_entity(eg_app *app, eg_entity *entity)
{
    if (app == NULL || entity == NULL)
    {
        return NULL;
    }

    // Get the entities before and after the current entity.
    eg_entity *previous = entity->previous;
    eg_entity *next = entity->next;

    // Set the current entity's next and previous pointers to NULL to remove
    // it from the list. For the remainder of this function, this entity will
    // be referred to as the "removed entity".
    entity->next = NULL;
    entity->previous = NULL;

    // If the next entity is not NULL, it's previous pointer receives the
    // value of the removed entity's previous pointer.
    if (next != NULL)
    {
        next->previous = previous;
    }

    // Only the first entity in the list should have a previous pointer with a
    // value of NULL. If we're removing the first entity in the list, then we
    // need to update the pointer in the app struct so the list isn't lost.
    if (previous == NULL)
    {
        // The next entity becomes the first entity in the list.
        // If the next pointer is NULL, then the entity list is now empty.
        app->entities = next;

        return entity;
    }

    // The removed entity's next pointer becomes the previous entity's next
    // pointer.
    previous->next = next;

    return entity;
}

int eg_check_flag(eg_entity *e, int f)
{
    // The flag index must be in the range [0, 7] since we're only dealing
    // with 8 bits.
    if (f < 0 || f > 8)
    {
        return 0;
    }

    // Convert the flag index to a bit value.
    uint8_t bit = (uint8_t)(1 << f);

    // Check if the flag is set.
    if (e->flags & bit)
    {
        return 1;
    }

    return 0;
}

void eg_set_flag(eg_entity *e, int f)
{
    // The flag index must be in the range [0, 7] since we're only dealing
    // with 8 bits.
    if (f < 0 || f > 8)
    {
        return;
    }

    // Convert the flag index to a bit value.
    uint8_t bit = (uint8_t)(1 << f);

    // Set the flag.
    e->flags |= bit;
}

void eg_clear_flag(eg_entity *e, int f)
{
    // The flag index must be in the range [0, 7] since we're only dealing
    // with 8 bits.
    if (f < 0 || f > 8)
    {
        return;
    }

    // Convert the flag index to a bit value.
    uint8_t bit = (uint8_t)(1 << f);

    // Clear the flag.
    e->flags &= ~bit;
}

void eg_toggle_flag(eg_entity *e, int f)
{
    // The flag index must be in the range [0, 7] since we're only dealing
    // with 8 bits.
    if (f < 0 || f > 8)
    {
        return;
    }

    // Convert the flag index to a bit value.
    uint8_t bit = (uint8_t)(1 << f);

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

int eg_load_font(eg_app *app, const char *path, int p)
{
    return eg_impl_load_font(app, path, p);
}

void eg_draw_text(eg_app *app, const char *msg, int x, int y)
{
    eg_impl_draw_text(app, msg, x, y);
}

//----------------------------------------------------------------------------
// image functions

int eg_load_sprite_sheet(eg_app *app, const char *path)
{
    return eg_impl_load_sprite_sheet(app, path);
}

void eg_draw_image(eg_app *app, eg_rect *src, eg_rect *dest)
{
    return eg_impl_draw_image(app, src, dest);
}
