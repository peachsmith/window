#include "demo/demo.h"
#include "demo/input/input.h"
#include "demo/entities/entity_types.h"
#include "demo/scenes/scenes.h"

#include "common/util.h"

static inline void clear_input(cr_app *app)
{
    cr_pop_input_handler(app); // scenes menu input handler
    cr_pop_input_handler(app); // debug menu input handler
    cr_pop_input_handler(app); // current scene's input
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
static void begin_transition(cr_app *app, cr_func loader, cr_func input)
{
    app->transition_loader = loader;
    app->transition_input = input;

    // Start the screen transition by marking the data field as 1 for any
    // entity that has an entity type of ENTITY_TYPE_TRANSITION.
    for (int i = 0; i < app->entity_cap; i++)
    {
        if (app->entities[i].type == ENTITY_TYPE_TRANSITION && app->entities[i].present)
        {
            app->entities[i].data = 1 | TX_RESUME;
            app->overlays[app->overlay_count++] = &(app->entities[i]);
        }
    }
}

void demo_scene_menu_input(cr_app *app)
{
    if (cr_consume_input(app, CR_KEYCODE_X) ||
        cr_consume_input(app, CR_KEYCODE_Q) ||
        cr_consume_input(app, CR_KEYCODE_ESCAPE))
    {
        app->menu_count--;
        cr_pop_input_handler(app);
        return;
    }

    // Locate the scene menu.
    cr_entity *menu_entity = app->menus[app->menu_count - 1];
    if (menu_entity == NULL)
    {
        return;
    }

    if (cr_consume_input(app, CR_KEYCODE_UP))
    {
        if (menu_entity->cursor_y > 0)
        {
            menu_entity->cursor_y--;
        }
        else if (menu_entity->scroll_y > 0)
        {
            menu_entity->scroll_y--;
        }
    }

    if (cr_consume_input(app, CR_KEYCODE_DOWN))
    {
        if (menu_entity->cursor_y < 3)
        {
            menu_entity->cursor_y++;
        }
        else if (menu_entity->scroll_y < 1)
        {
            menu_entity->scroll_y++;
        }
    }

    // menu item selection
    if (cr_consume_input(app, CR_KEYCODE_Z))
    {
        switch (menu_entity->cursor_y + menu_entity->scroll_y)
        {
        case 0:
            clear_input(app);
            begin_transition(app, demo_load_scene_0, demo_root_input);
            break;

        case 1:
            clear_input(app);
            begin_transition(app, demo_load_scene_1, demo_root_input);
            break;

        case 2:
            clear_input(app);
            begin_transition(app, demo_load_scene_2, demo_root_input);
            break;

        case 3:
            clear_input(app);
            begin_transition(app, demo_load_scene_3, demo_root_input);
            break;

        case 4:
            clear_input(app);
            begin_transition(app, demo_load_scene_4, demo_root_input);
            break;

        default:
            break;
        }
    }
}