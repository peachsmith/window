#include "tootnsploot/input/input.h"
#include "tootnsploot/entities/entity_types.h"
#include "tootnsploot/scenes/scenes.h"
#include "common/menu.h"

#include <stdlib.h>

// TODO: I definitely need to move the transition functions into some
// other file.

static void do_transition(cr_app *app, cr_func load_next_scene)
{
    cr_entity transition;
    cr_entity *entity = NULL;

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
        // We don't set the update flag here, because we're transitioning
        // into the main menu, which starts in a paused state.

        transition.ticks = entity->ticks;
        transition.data = entity->data;
        transition.flags = entity->flags;
    }

    // Close the pause menu.
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

static void main_menu_transition(cr_app *app)
{
    do_transition(app, load_title_screen);
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
static void begin_transition(cr_app *app, cr_func transition_loader, cr_func handler)
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

void pause_menu_input_handler(cr_app *app)
{
    // Locate the pause menu.
    cr_entity *menu_entity = app->menus[app->menu_count - 1];
    if (menu_entity == NULL)
    {
        return;
    }

    // Close the pause menu if the escape key or x key is pressed.
    if (cr_consume_input(app, CR_KEYCODE_X) || cr_consume_input(app, CR_KEYCODE_ESCAPE))
    {
        app->menu_count--;
        app->pause = 0;
        cr_pop_input_handler(app);
        return;
    }

    if (cr_consume_input(app, CR_KEYCODE_UP))
    {
        if (menu_entity->cursor_y > 0)
        {
            menu_entity->cursor_y--;
        }
    }

    if (cr_consume_input(app, CR_KEYCODE_DOWN))
    {
        if (menu_entity->cursor_y < 2)
        {
            menu_entity->cursor_y++;
        }
    }

    // Resume
    // Main Menu
    // Quit
    if (cr_consume_input(app, CR_KEYCODE_Z))
    {
        switch (menu_entity->cursor_y)
        {
        case 0:
            app->menu_count--;
            app->pause = 0;
            cr_pop_input_handler(app);
            break;
        case 1:
            // Remove the input handlers of the pause menu and the forest
            // scene, then transition to the main menu.
            cr_pop_input_handler(app);
            cr_pop_input_handler(app);
            begin_transition(app, main_menu_transition, main_menu_input_handler);
            break;
        case 2:
            app->done = 1;
            break;
        default:
            break;
        }
    }
}
