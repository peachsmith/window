#include "entity_demo.h"

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
    case ENTITY_ID_Q:
        return 'Q';

    case ENTITY_ID_W:
        return 'W';

    case ENTITY_ID_E:
        return 'E';

    default:
        return 'X';
    }
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

    ent->id = id;

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
