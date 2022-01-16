#include "example.h"

#include <stdio.h>
#include <stdlib.h>

//----------------------------------------------------------------------------
// core functions

int eg_initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(
            stderr,
            "failed to initialize SDL. error: %s\n",
            SDL_GetError());
        return 0;
    }

    return 1;
}

void eg_terminate()
{
    SDL_Quit();
}

eg_app *eg_create_app()
{
    eg_app *app = NULL;

    // Allocate memory for the eg_app struct.
    app = (eg_app *)malloc(sizeof(eg_app));

    // Verify memory allocation.
    if (app == NULL)
    {
        return NULL;
    }

    // Ensure that all pointers are NULL.
    app->window = NULL;
    app->renderer = NULL;
    app->keystates = NULL;
    app->entities = NULL;
    app->input_handlers = NULL;

    // Create the window.
    app->window = SDL_CreateWindow(
        "Example",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        240,
        160,
        SDL_WINDOW_SHOWN);

    // Verify window creation.
    if (app->window == NULL)
    {
        fprintf(
            stderr,
            "failed to create window. error: %s\n",
            SDL_GetError());
        free(app);
        return NULL;
    }

    // Create the renderer.
    app->renderer = SDL_CreateRenderer(
        app->window,
        -1,
        SDL_RENDERER_ACCELERATED);

    // Verify renderer creation.
    if (app->renderer == NULL)
    {
        fprintf(
            stderr,
            "failed to create renderer. error: %s\n",
            SDL_GetError());
        SDL_DestroyWindow(app->window);
        free(app);
        return NULL;
    }

    // Set the main loop sentinel value to 0.
    app->done = 0;

    // Since the desired framerate is 60 frames per second, we set the
    // approximate milliseconds per frame to 16.
    // This is the truncated result of 1000 / 16.
    app->frame_len = 16;

    // Get the keyboard state.
    app->keystates = SDL_GetKeyboardState(NULL);

    // Initialize the key press flags to 0.
    for (int i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        app->key_captures[i] = 0;
    }

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
    while (app->input_handlers != NULL)
    {
        eg_input_handler *previous = app->input_handlers->previous;
        eg_destroy_input_handler(app->input_handlers);
        app->input_handlers = previous;
    }

    // Destroy the renderer, the window, then the wrapper structure.
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
    free(app);
}

void eg_process_events(eg_app *app)
{
    // First, we get the current tick count for regulating framerate.
    app->ticks = SDL_GetTicks64();

    // According to the wiki, it is common practice to process all events in
    // the event queue at the beginning of each iteration of the main loop.
    // The SDL_PollEvent function also calls SDL_PumpEvents, which updates the
    // keyboard state array.
    while (SDL_PollEvent(&(app->event)))
    {
        if (app->event.type == SDL_QUIT)
        {
            app->done = 1;
        }

        // Clear the key press capture flags.
        if (app->event.type == SDL_KEYUP)
        {
            app->key_captures[app->event.key.keysym.scancode] = 0;
        }
    }
}

void eg_handle_input(eg_app *app)
{
    // Call the top input handler's callback function.
    app->input_handlers->callback(app, app->input_handlers->target);
}

void eg_update(eg_app *app)
{
    eg_entity *ent = app->entities;
    while (ent != NULL)
    {
        if (app->registry[ent->id].update != NULL)
        {
            app->registry[ent->id].update(app, ent);
        }
        ent = ent->next;
    }
}

void eg_draw(eg_app *app)
{
    // Clear the contents of the previous frame.
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);

    // Render each entity.
    eg_entity *ent = app->entities;
    while (ent != NULL)
    {
        if (app->registry[ent->id].render != NULL)
        {
            app->registry[ent->id].render(app, ent);
        }
        ent = ent->next;
    }

    // Show the contents of the current frame.
    SDL_RenderPresent(app->renderer);
}

void eg_delay(eg_app *app)
{
    // Get the approximate number of milliseconds since the beginning of the
    // current iteration of the main loop.
    Uint64 elapsed = SDL_GetTicks64() - app->ticks;

    // If the frame length is greater than the elapsed milliseconds since the
    // beginning of the frame, wait for the duration of the difference.
    if (app->frame_len > elapsed)
    {
        // The SDL_Delay function expects a Uint32 as its argument.
        // Converting from Uint64 to Uint32 will truncate the value, but the
        // difference between the frame length and the elapsed milliseconds
        // should never be greater than UINT32_MAX.
        SDL_Delay((Uint32)(app->frame_len - elapsed));
    }
}

//----------------------------------------------------------------------------
// input handling functions

int eg_peek_input(eg_app *app, int code)
{
    if (code >= SDL_NUM_SCANCODES)
    {
        return 0;
    }

    if (app->keystates[code])
    {
        return 1;
    }

    return 0;
}

int eg_consume_input(eg_app *app, int code)
{
    if (code >= SDL_NUM_SCANCODES)
    {
        return 0;
    }

    if (app->key_captures[code])
    {
        return 0;
    }

    if (app->keystates[code])
    {
        app->key_captures[code] = 1;
        return 1;
    }

    return 0;
}

eg_input_handler *eg_create_input_handler(eg_callback callback)
{
    eg_input_handler *handler = NULL;

    handler = (eg_input_handler *)malloc(sizeof(eg_input_handler));
    if (handler == NULL)
    {
        return NULL;
    }

    handler->previous = NULL;
    handler->callback = callback;
    handler->target = NULL;

    return handler;
}

void eg_destroy_input_handler(eg_input_handler *handler)
{
    free(handler);
}

void eg_push_input_handler(eg_app *app, eg_input_handler *handler)
{
    // If the stack is empty, then the handler becomes the top of the stack.
    if (app->input_handlers == NULL)
    {
        app->input_handlers = handler;
        return;
    }

    // Save the reference to the current handler in the new handler.
    handler->previous = app->input_handlers;

    // Set the application's current input handler to be the new handler.
    app->input_handlers = handler;
}

eg_input_handler *eg_pop_input_handler(eg_app *app)
{
    // If the stack is empty, then return without doing anything.
    if (app->input_handlers == NULL)
    {
        return NULL;
    }

    // Get the top of the input handler stack.
    // This will be returned so that it can be saved or destroyed.
    eg_input_handler *current = app->input_handlers;

    // The previous input handler now becomes the current input handler.
    app->input_handlers = app->input_handlers->previous;

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

    return reg;
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
    entity->x_pos = 0;
    entity->y_pos = 0;
    entity->x_vel = 0;
    entity->y_vel = 0;
    entity->next = NULL;
    entity->previous = NULL;

    return entity;
}

void eg_destroy_entity(eg_entity *entity)
{
    free(entity);
}

void eg_add_entity(eg_app *app, eg_entity *entity)
{
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
