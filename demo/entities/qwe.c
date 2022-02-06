#include "demo/entities/qwe.h"
#include "demo/entities/entity_types.h"

#include <stdio.h>

/**
 * Gets the character representation of a demo entity id.
 * The returned character should be either 'Q', 'W', or 'E'. If an unexpected
 * id is received, 'X' is returned.
 * 
 * Params:
 *   int - the id of the demo entity
 * 
 * Returns:
 *   char - a single character representation of the id
 */
static char id_to_char(int id)
{
    switch (id)
    {
    case ENTITY_TYPE_Q:
        return 'Q';

    case ENTITY_TYPE_W:
        return 'W';

    case ENTITY_TYPE_E:
        return 'E';

    default:
        return 'X';
    }
}

// populates an SDL_Rect with values from an entity.
static void get_rect(SDL_Rect *r, eg_app *app, eg_entity *e)
{
    r->x = e->x_pos;
    r->y = e->y_pos;
    r->w = app->registry[e->id].width;
    r->h = app->registry[e->id].height;
}

//----------------------------------------------------------------------------
// entity rendering functions

// draws a red rectangle at the location of entity q
static void render_q(eg_app *app, eg_entity *ent)
{
    SDL_Rect r;
    get_rect(&r, app, ent);

    SDL_SetRenderDrawColor(app->renderer, 0x02, 0x75, 0xD8, 0xFF);
    SDL_RenderFillRect(app->renderer, &r);
}

// draws a red rectangle at the location of entity w
static void render_w(eg_app *app, eg_entity *ent)
{
    SDL_Rect r;
    get_rect(&r, app, ent);

    SDL_SetRenderDrawColor(app->renderer, 0x5C, 0xB8, 0x5C, 0xFF);
    SDL_RenderFillRect(app->renderer, &r);
}

// draws a red rectangle at the location of entity e
static void render_e(eg_app *app, eg_entity *ent)
{
    SDL_Rect r;
    get_rect(&r, app, ent);

    SDL_SetRenderDrawColor(app->renderer, 0xD9, 0x53, 0x4F, 0xFF);
    SDL_RenderFillRect(app->renderer, &r);
}

//----------------------------------------------------------------------------
// entity update functions

// updates the state of the Q entity
static void update_q(eg_app *app, eg_entity *ent)
{
    // printf("this is the update q function\n");
}

// updates the state of the W entity
static void update_w(eg_app *app, eg_entity *ent)
{
    // printf("this is the update w function\n");
}

// updates the state of the E entity
static void update_e(eg_app *app, eg_entity *ent)
{
    // printf("this is the update e function\n");
}

void entity_demo_register_q(eg_entity_type *t)
{
    t->width = 20;
    t->height = 20;
    t->render = render_q;
    t->update = update_q;
}

void entity_demo_register_w(eg_entity_type *t)
{
    t->width = 20;
    t->height = 20;
    t->render = render_w;
    t->update = update_w;
}

void entity_demo_register_e(eg_entity_type *t)
{
    t->width = 20;
    t->height = 20;
    t->render = render_e;
    t->update = update_e;
}

int entity_demo_add(eg_app *app, int id)
{
    eg_entity *ent = app->entities;

    // Check for an entity with a matching id.
    while (ent != NULL && ent->id != id)
    {
        ent = ent->next;
    }

    // If the entity is already present, simply return without doing anything.
    if (ent != NULL)
    {
        return 1;
    }

    // If the entity was not found, then create it and add it to the entity
    // list.
    ent = eg_create_entity();
    if (ent == NULL)
    {
        return 0;
    }

    // Set the render function.
    switch (id)
    {
    case ENTITY_TYPE_Q:
        ent->id = ENTITY_TYPE_Q;
        ent->x_pos = 10;
        ent->y_pos = 10;
        break;

    case ENTITY_TYPE_W:
        ent->id = ENTITY_TYPE_W;
        ent->x_pos = 40;
        ent->y_pos = 10;
        break;

    case ENTITY_TYPE_E:
        ent->id = ENTITY_TYPE_E;
        ent->x_pos = 70;
        ent->y_pos = 10;
        break;

    default:
        break;
    }

    eg_add_entity(app, ent);

    printf("added the %c entity\n", id_to_char(id));

    return 1;
}

void entity_demo_remove(eg_app *app, int id)
{
    eg_entity *ent = app->entities;

    // Check for an entity with a matching id.
    while (ent != NULL && ent->id != id)
    {
        ent = ent->next;
    }

    // If the entity is already present, remove it and destroy it.
    if (ent != NULL)
    {
        eg_entity *rem = eg_remove_entity(app, ent);
        eg_destroy_entity(rem);
        printf("removed the %c entity\n", id_to_char(id));
    }
}

void entity_demo_print(eg_app *app)
{
    printf("---- BEGIN entity list ----\n");
    eg_entity *ent = app->entities;
    while (ent != NULL)
    {
        printf("entity: %c\n", id_to_char(ent->id));
        ent = ent->next;
    }
    printf("---- END entity list ----\n");
}
