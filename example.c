#include "example.h"
#include "collision.h"

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

    // Destroy the entity registry.
    eg_destroy_registry(app->registry);

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

/**
 * Bounding box collision detection.
 *
 * @return int
 */
static void has_collided(
    eg_app *app,
    eg_entity *a,
    eg_entity *b,
    eg_collision_result *res)
{
    // Get the width and height of both entities.
    int aw = app->registry[a->id].width;
    int ah = app->registry[a->id].height;
    int bw = app->registry[b->id].width;
    int bh = app->registry[b->id].height;

    // Get the x and y position of both entities.
    // These will be the position of the left and right edges.
    int a_left = a->x_pos;
    int a_top = a->y_pos;
    int b_left = b->x_pos;
    int b_top = b->y_pos;

    // Get the positions of the right and bottom edges.
    int a_right = a_left + aw;
    int a_bottom = a_top + ah;
    int b_right = b_left + bw;
    int b_bottom = b_top + bh;

    // Clear the result data.
    res->collided = 0;
    res->dx0 = 0;
    res->dx1 = 0;
    res->dy0 = 0;
    res->dy1 = 0;

    int overlap_x = 0;
    int overlap_y = 0;

    // Check if the left edge of A is in between the left and right edges
    // of b.
    if (a_left > b_left && a_left < b_right)
    {
        res->dx0 = a_right - b_left;
        res->dx1 = b_right - a_left;
        overlap_x = 1;
    }

    // Check if the right edge of a is in between the left and right edges
    // of B.
    if (a_right > b_left && a_right < b_right)
    {
        res->dx0 = a_right - b_left;
        res->dx1 = b_right - a_left;
        overlap_x = 1;
    }

    // Check if B is inside of A.
    if (b_left >= a_left && b_left <= a_right &&
        b_right >= a_left && b_right <= a_right)
    {
        overlap_x = 1;
    }

    // Check if the top edge of A is in between the top and bottom edges
    // of B.
    if (a_top > b_top && a_top < b_bottom)
    {
        res->dy0 = a_bottom - b_top;
        res->dy1 = b_bottom - a_top;
        overlap_y = 1;
    }

    // Check if the bottom edge of A is in between the top and bottom
    // edges of B.
    if (a_bottom > b_top && a_bottom < b_bottom)
    {
        res->dy0 = a_bottom - b_top;
        res->dy1 = b_bottom - a_top;
        overlap_y = 1;
    }

    // Check if B is inside of A.
    if (b_top >= a_top && b_top <= a_bottom &&
        b_bottom >= a_top && b_bottom <= a_bottom)
    {
        overlap_y = 1;
    }

    // If the boundaries of the two entities overlap in both directions, then
    // a there is a collision.
    if (overlap_x && overlap_y)
    {
        res->collided = 1;
    }
}

void eg_update(eg_app *app)
{
    //--------------------------------------------------------
    // TODO: remove this once collision detection is done.
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);
    int mouse_x, mouse_y;
    Uint32 mouse_buttons;
    mouse_buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

    SDL_Rect mouse_rect = {
        .x = mouse_x - 10,
        .y = mouse_y - 10,
        .w = 20,
        .h = 20};

    SDL_SetRenderDrawColor(app->renderer, 0, 255, 0, 255);
    SDL_RenderDrawRect(app->renderer, &mouse_rect);
    eg_entity *player = NULL;
    eg_entity *block = NULL;
    //--------------------------------------------------------

    eg_entity *ent = app->entities;
    eg_entity *a = app->entities;

    // Update state.
    while (ent != NULL)
    {
        //---------------------------------------------
        // Get the references to the player and the block.
        if (ent->id == 0)
        {
            player = ent;
        }
        else if (ent->id == 5)
        {
            block = ent;
        }
        //---------------------------------------------

        if (app->registry[ent->id].update != NULL)
        {
            app->registry[ent->id].update(app, ent);
        }
        ent = ent->next;
    }

    if (player != NULL && block != NULL)
    {
        eg_rect target = {
            .p = {.x = block->x_pos, .y = block->y_pos},
            .w = app->registry[block->id].width,
            .h = app->registry[block->id].height};
        eg_point p = {.x = player->x_pos, .y = player->y_pos};
        eg_point d = {.x = mouse_x - p.x, .y = mouse_y - p.y};
        eg_point cp;                                      // contact point
        eg_point cn = {.x = 0xDEADBEEF, .y = 0xDEADBEEF}; // contact normal
        float t; // shortest time to collision (not real time)

        if (eg_ray_v_rect(&p, &d, &target, &t, &cp, &cn))
        {
            // Set the draw color to cyan for drawing the contact point.
            SDL_SetRenderDrawColor(app->renderer, 0, 255, 255, 255);

            // Draw the contact point.
            SDL_Rect c_rect = {.x = cp.x - 5, .y = cp.y - 5, .w = 10, .h = 10};
            SDL_RenderFillRect(app->renderer, &c_rect);

            // Set the draw color to red for drawing the contact point.
            SDL_SetRenderDrawColor(app->renderer, 255, 0, 0, 255);

            // Draw the contact normal.
            SDL_RenderDrawLine(
                app->renderer,
                cp.x,
                cp.y,
                cp.x + cn.x * 20,
                cp.y + cn.y * 20);

            if (cn.x > 1 || cn.x < -1 || cn.y > 1 || cn.y < -1)
            {
                fprintf(stderr, "[ERROR] invalid contact normal: (%X, %X), "
                                "contact point: (%d, %d), "
                                "t: %.2f, "
                                "P: (%d, %d), "
                                "D: (%d, %d)\n",
                        cn.x,
                        cn.y,
                        cp.x,
                        cp.y,
                        t,
                        p.x,
                        p.y,
                        d.x,
                        d.y);

                SDL_Delay(2000);
                app->done = 1;
            }

            // Set the draw color to yellow for drawing the ray.
            SDL_SetRenderDrawColor(app->renderer, 255, 255, 0, 255);
        }

        SDL_RenderDrawLine(
            app->renderer,
            p.x,
            p.y,
            mouse_x,
            mouse_y);
    }

    // collision detection
    // while (a != NULL)
    // {
    //     eg_entity *b = a->next;
    //     while (b != NULL)
    //     {
    //         eg_collision_result res;
    //         has_collided(app, a, b, &res);
    //         if (res.collided)
    //         {
    //             eg_collider cola = app->registry[a->id].collide;
    //             eg_collider colb = app->registry[b->id].collide;
    //             if (cola != NULL)
    //             {
    //                 cola(app, a, b, &res, 0);
    //             }
    //             if (colb != NULL)
    //             {
    //                 colb(app, b, a, &res, 1);
    //             }
    //         }
    //         b = b->next;
    //     }
    //     a = a->next;
    // }

    // TODO: remove this once collision detection is done.
    // Draw a line from one rect to another
}

void eg_draw(eg_app *app)
{
    // Clear the contents of the previous frame.
    // TODO: uncomment this once collision detection is done.
    // SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    // SDL_RenderClear(app->renderer);

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
    if (handler == NULL)
    {
        return;
    }

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
