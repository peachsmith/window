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

void eg_update(eg_app *app)
{
    //--------------------------------------------------------
    // TODO: remove this once collision detection is done.
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);

    eg_entity *ent = app->entities;
    eg_entity *source = app->entities;

    // // Update state.
    // while (ent != NULL)
    // {
    //     if (app->registry[ent->id].update != NULL)
    //     {
    //         app->registry[ent->id].update(app, ent);
    //     }
    //     ent = ent->next;
    // }

    // Create a dynamic array of collision results.
    // This array can be reused on each iteration of the main collision
    // detection loop.
    int col_cap = 10;     // collision result list capacity
    eg_col_res *col_ress; // collision result list
    col_ress = (eg_col_res *)malloc(sizeof(eg_col_res) * col_cap);
    if (col_ress == NULL)
    {
        fprintf(stderr, "failed to create collision result list\n");
        app->done = 1;
        return;
    }

    // Collision Detection Loop.
    // The main collision detection loop checks each element of the entity
    // list with all toher elements of the entity list.
    // On each iteration of the main collision detection loop, the current
    // element from the entity list is called the source.
    // On each iteration of the inner loop, the current entity being checked
    // against the source is called the target. When dealing with functions
    // that refer top their entity arguments as A and B, A is the source and
    // B is the target.
    //
    // This has three main stages:
    //  1. detect collisions
    //  2. sort collisions
    //  3. resolve collisions
    while (source != NULL)
    {
        int ovl_count = 0;
        int col_count = 0;

        // Detect the collisions.
        eg_entity *target = app->entities; // original value: a->next;
        while (target != NULL)
        {
            // If a and b overlap, then calculate the contact point and
            // contact normal to assist with resolution.
            eg_overlap ovl;
            eg_rect ar = {
                .x = source->x_pos + source->x_vel,
                .y = source->y_pos + source->y_vel,
                .w = app->registry[source->id].width,
                .h = app->registry[source->id].height,
            };
            eg_rect br = {
                .x = target->x_pos,
                .y = target->y_pos,
                .w = app->registry[target->id].width,
                .h = app->registry[target->id].height,
            };
            eg_ray_res res = {.cn = {.x = 2, .y = 2}};

            // TODO: figure out how to skip checking collision of an entity
            // with itself.
            if (target->id != 0) // && is_overlapped(&ar, &br, &ovl))
            {
                ovl_count++;
                if (eg_check_past_col(app, source, target, &res))
                {
                    if (!is_overlapped(&ar, &br, &ovl))
                    {
                        if (source->id == 0)
                        {
                            printf("purported collision, but NO SIRREE!\n");
                        }
                    }

                    // if (source->id == 0)
                    // {
                    //     printf("collision detected\n");
                    // }

                    // Add the collision result to the array.
                    if (col_count >= col_cap)
                    {
                        int new_cap = col_count + col_count / 2;
                        eg_col_res *new_ress = (eg_col_res *)realloc(col_ress, new_cap);
                        if (new_ress == NULL)
                        {
                            fprintf(stderr, "failed to reallocate collision result list\n");
                            free(col_ress);
                            app->done = 1;
                            return;
                        }
                    }
                    col_ress[col_count].col = res;
                    col_ress[col_count++].target = target;
                }
            }

            target = target->next;
        }

        // Sort the collisions.
        int sorted = 0;
        while (!sorted)
        {
            sorted = 1;
            for (int i = 0; i < col_count; i++)
            {
                eg_col_res tmp;
                if (i < col_count - 1)
                {
                    float t0 = col_ress[i].col.t;
                    float t1 = col_ress[i + 1].col.t;
                    eg_point cn0 = col_ress[i].col.cn;
                    eg_point cn1 = col_ress[i + 1].col.cn;

                    // Check the contact normals.
                    // If both the x and y components of a contact normal
                    // are non zero, then it is considered "greater" than
                    // a true contact normal.
                    int corner = 0;
                    if (cn0.x && cn0.y && (!cn1.x || !cn1.y))
                    {
                        corner = 1;
                    }

                    // Check if we have two possible perfect corner collisions
                    // for one source. Hopefully, this never happens.
                    // It should only be possible if two targets overlap in
                    // just the right way.
                    if (cn0.x && cn0.y && cn1.x && cn1.y)
                    {
                        printf("[WARN] two corner collisions for one source\n");
                    }

                    if (t0 > t1 || (t0 == t1 && corner))
                    {
                        sorted = 0;
                        tmp = col_ress[i];
                        col_ress[i] = col_ress[i + 1];
                        col_ress[i + 1] = tmp;
                    }
                }
            }
        }

        // TEMP: print the possible collisions.
        // if (col_count > 0)
        // {
        //     printf("possible collisions: { ");
        //     for (int i = 0; i < col_count; i++)
        //     {
        //         printf("{%.2f, (%d, %d)}", col_ress[i].col.t, col_ress[i].col.cn.x, col_ress[i].col.cn.y);
        //         if (i < col_count - 1)
        //         {
        //             printf(", ");
        //         }
        //         else
        //         {
        //             printf(" }\n");
        //         }
        //     }
        // }

        // Resolve the collisions.
        int resolve_count = 0;
        for (int i = 0; i < col_count; i++)
        {
            eg_entity *a = source;
            eg_entity *b = col_ress[i].target;
            eg_overlap ovl = {.dx0 = 0, .dx1 = 0, .dy0 = 0, .dy1 = 0};
            eg_rect ar = {
                .x = a->x_pos + a->x_vel,
                .y = a->y_pos + a->y_vel,
                .w = app->registry[a->id].width,
                .h = app->registry[a->id].height,
            };
            eg_rect br = {
                .x = b->x_pos,
                .y = b->y_pos,
                .w = app->registry[b->id].width,
                .h = app->registry[b->id].height,
            };
            eg_ray_res col = {.cn = {.x = 2, .y = 2}};

            if (is_overlapped(&ar, &br, &ovl))
            {
                if (eg_check_past_col(app, a, b, &col))
                {
                    eg_collider cola = app->registry[a->id].collide;
                    eg_collider colb = app->registry[b->id].collide;
                    if (cola != NULL)
                    {
                        cola(app, a, b, &ovl, &col, 0);
                    }
                    if (colb != NULL)
                    {
                        colb(app, b, a, &ovl, &col, 1);
                    }
                    resolve_count++;
                }
            }
        }

        source = source->next;
    }

    // Destroy the collision detection result array.
    free(col_ress);

    // Update state.
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
