#include "tootnsploot/tootnsploot.h"
#include "tootnsploot/input/input.h"
#include "tootnsploot/entities/entity_types.h"
#include "tootnsploot/scenes/scenes.h"

#include "common/util.h"
#include "common/menu.h"

static void do_transition(eg_app *app, eg_func load_next_scene)
{
    eg_entity transition;
    eg_entity *entity = NULL;

    // Save the transition entity state.
    for (int i = 0; i < app->entity_cap; i++)
    {
        if (app->entities[i].type == ENTITY_TYPE_TRANSITION && app->entities[i].present)
        {
            entity = &(app->entities[i]);
        }
    }

    if (entity != NULL)
    {
        // Set the update flag on the transition entity so that the next scene
        // doesn't start in a paused state.
        eg_set_flag(entity, ENTITY_FLAG_UPDATE);

        transition.ticks = entity->ticks;
        transition.data = entity->data;
        transition.flags = entity->flags;
    }

    // Close the main menu.
    app->menu_count--;

    // Clear the current scene and load the next scene.
    clear_scene(app);
    load_next_scene(app);

    // Restore the transition entity state.
    entity = NULL;
    for (int i = 0; i < app->entity_cap; i++)
    {
        if (app->entities[i].type == ENTITY_TYPE_TRANSITION && app->entities[i].present)
        {
            entity = &(app->entities[i]);
        }
    }

    if (entity != NULL)
    {
        // Increment the tick count of the new transition entity to
        // prevent the update function from being called again.
        entity->ticks = transition.ticks + 1;
        entity->data = transition.data;
        entity->flags = transition.flags;

        // Add the new transition entity to the overlay slice.
        app->overlays[app->overlay_count++] = entity;
    }
}

static void forest_transition(eg_app *app)
{
    do_transition(app, load_forest_scene);
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
static void begin_transition(eg_app *app, eg_func transition_loader, eg_func handler)
{
    // Set the transition callback to load scene 0.
    // Set the next input handler to be the root input handler.
    app->transition_loader = transition_loader;
    app->transition_input_handler = handler;

    // Start the screen transition by marking the data field as 1 for any
    // entity that has an entity type of ENTITY_TYPE_TRANSITION.
    for (int i = 0; i < app->entity_cap; i++)
    {
        if (app->entities[i].type == ENTITY_TYPE_TRANSITION && app->entities[i].present)
        {
            app->entities[i].data = 1;
            app->overlays[app->overlay_count++] = &(app->entities[i]);
        }
    }
}

void main_menu_input_handler(eg_app *app)
{
    // Locate the main menu.
    eg_entity *menu_entity = app->menus[app->menu_count - 1];
    if (menu_entity == NULL)
    {
        return;
    }

    // If the escape key is pressed, terminate the application.
    if (eg_consume_input(app, EG_KEYCODE_ESCAPE))
    {
        app->done = 1;
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
        if (menu_entity->cursor_y < 2)
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
            // Remove the main menu input handler and transition to the forest scene.
            eg_pop_input_handler(app);
            begin_transition(app, forest_transition, forest_input_handler);
            break;

        case 1:
        {
            // Locate the controls menu.
            eg_entity *controls_menu = NULL;
            for (int i = 0; i < app->entity_cap && controls_menu == NULL; i++)
            {
                if (app->entities[i].type == ENTITY_TYPE_CONTROLS_MENU && app->entities[i].present)
                {
                    controls_menu = &(app->entities[i]);
                }
            }

            // Set the pause menu as the active menu.
            app->menus[app->menu_count++] = controls_menu;

            eg_push_input_handler(app, controls_menu_input_handler);
        }
        break;

        case 2:
        {
            // Locate the characters menu.
            eg_entity *characters_menu = NULL;
            for (int i = 0; i < app->entity_cap && characters_menu == NULL; i++)
            {
                if (app->entities[i].type == ENTITY_TYPE_CHARACTERS_MENU && app->entities[i].present)
                {
                    characters_menu = &(app->entities[i]);
                }
            }

            // Set the pause menu as the active menu.
            app->menus[app->menu_count++] = characters_menu;

            eg_push_input_handler(app, characters_menu_input_handler);
        }
        break;

        default:
            break;
        }
    }
}
