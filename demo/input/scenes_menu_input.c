#include "demo/input/input.h"
#include "demo/menu/menu.h"
#include "demo/entities/entity_types.h"
#include "demo/scenes/scenes.h"

#include <stdio.h>

static void do_transition(eg_app *app, eg_callback load_next_scene)
{
    eg_entity transition;
    eg_entity *entity = NULL;

    // Save the transition entity state.
    for (int i = 0; i < app->entity_count; i++)
    {
        if (app->entity_array[i].type == ENTITY_TYPE_TRANSITION)
        {
            entity = &(app->entity_array[i]);
        }
    }

    if (entity != NULL)
    {
        transition.ticks = entity->ticks;
        transition.data = entity->data;
        transition.flags = entity->flags;
    }

    printf("[DEBUG] loading next scene\n");

    // Clear the current scene and load the next scene.
    clear_scene(app);
    load_next_scene(app);

    // Restore the transition entity state.
    entity = NULL;
    for (int i = 0; i < app->entity_count; i++)
    {
        if (app->entity_array[i].type == ENTITY_TYPE_TRANSITION)
        {
            entity = &(app->entity_array[i]);
        }
    }

    if (entity != NULL)
    {
        // Increment the tick count of the new transition entity to
        // prevent the update function from being called again.
        entity->ticks = transition.ticks + 1;
        entity->data = transition.data;
        entity->flags = transition.flags;

        // TODO: make this work with entity array.
        // Give the update loop a reference to the new entity list.
        app->transition_complete = 1;
    }
}

static void scene0_transition(eg_app *app)
{
    do_transition(app, load_scene_0);
}

static void scene1_transition(eg_app *app)
{
    do_transition(app, load_scene_1);
}

static void scene2_transition(eg_app *app)
{
    do_transition(app, load_scene_2);
}

static void scene3_transition(eg_app *app)
{
    do_transition(app, load_scene_3);
}

/**
 * This function starts the scene transition process.
 * First, it prepares the scene loader and input handler for the next scene.
 * Then it locates any transition entities in the current scene and sets their
 * data field to 1. This tells the transition entity to begin doing whatever
 * it needs to do. The transition entity will call the scene loader to load
 * the next scene.
 *
 */
static void begin_transition(eg_app *app, eg_callback transition_loader)
{
    // Close the scenes menu.
    app->menu_count--;
    eg_pop_input_handler(app);

    // Close the debug menu.
    app->menu_count--;
    eg_pop_input_handler(app);

    // Set the transition callback to load scene 0.
    // Set the next input handler to be the root input handler.
    app->transition_loader = transition_loader;
    app->transition_input_handler = root_input_handler;

    // Remove the input handler of the current scene.
    eg_pop_input_handler(app);

    // Start the screen transition by marking the data field as 1 for any
    // entity that has an entity type of ENTITY_TYPE_TRANSITION.
    for (int i = 0; i < app->entity_count; i++)
    {
        if (app->entity_array[i].type == ENTITY_TYPE_TRANSITION)
        {
            app->entity_array[i].data = 1;
        }
    }
}

void scene_menu_input_handler(eg_app *app)
{
    if (eg_consume_input(app, EG_KEYCODE_X) ||
        eg_consume_input(app, EG_KEYCODE_Q) ||
        eg_consume_input(app, EG_KEYCODE_ESCAPE))
    {
        app->menu_count--;
        eg_pop_input_handler(app);
        return;
    }

    // Locate the scene menu.
    eg_entity *menu_entity = app->menus[app->menu_count - 1];
    if (menu_entity == NULL)
    {
        return;
    }

    if (eg_consume_input(app, EG_KEYCODE_UP))
    {
        if (menu_entity->cursor_y > 0)
        {
            menu_entity->cursor_y--;
        }
    }

    if (eg_consume_input(app, EG_KEYCODE_DOWN))
    {
        if (menu_entity->cursor_y < 3)
        {
            menu_entity->cursor_y++;
        }
    }

    // menu item selection
    if (eg_consume_input(app, EG_KEYCODE_Z))
    {
        switch (menu_entity->cursor_y)
        {
        case 0:
            begin_transition(app, scene0_transition);
            break;

        case 1:
            begin_transition(app, scene1_transition);
            break;

        case 2:
            begin_transition(app, scene2_transition);
            break;

        case 3:
            begin_transition(app, scene3_transition);
            break;

        default:
            break;
        }
    }
}