#include "demo/entities/transition.h"
#include "demo/entities/entity_types.h"
#include "demo/texture/texture.h"
#include "demo/dialog/dialog.h"

#include <stdio.h>
#include <stdlib.h>

static void render_transition(eg_app *app, eg_entity *transition)
{
    if (!transition->data)
    {
        return;
    }

    eg_rect transition_rect;
    transition_rect.x = 0;
    transition_rect.y = 0;
    transition_rect.w = app->screen_width;
    transition_rect.h = app->screen_height;

    uint32_t c = 0x00000000;
    uint32_t a = 0xFF;

    if (transition->ticks < 30)
    {
        a = transition->ticks * 8;
        if (transition->ticks >= 29)
        {
            a = 0xFF;
        }
    }
    else if (transition->ticks < 60)
    {
        a = 0xFF;
    }
    else
    {
        a = 0xFF - ((transition->ticks - 60) * 8);
        if (transition->ticks >= 89)
        {
            a = 0x00;
        }
    }

    eg_set_color(app, c | (a << 24));
    eg_draw_rect(app, &transition_rect, 1);
}

static void update_transition(eg_app *app, eg_entity *transition)
{
    if (!transition->data)
    {
        return;
    }

    // The screen fades out for 30 frames, followed by 30 frames of blank
    // screen, followed by 30 frames of the next frame fading in.
    // During the second 30 frame period, we load the next scene.
    if (transition->ticks < 90)
    {
        // Load the next scene.
        if (transition->ticks == 30)
        {
            app->transition_loader(app);

            // Once the next scene has been loaded, the memory for the
            // previous scene has been freed. This means the reference
            // to the transition entity we have been using should be
            // considered no longer valid. So we return to the update
            // loop.
            return;
        }

        transition->ticks++;

        return;
    }

    transition->ticks = 0;
    transition->data = 0;

    // Once the next scene has been fully loaded and the transition is
    // complete, we push the input handler of the next scene to give control
    // back to the user.
    eg_push_input_handler(app, app->transition_input_handler);

    // If the update flag is set, unpause the application so the next scene
    // doesn't start in a paused state.
    if (eg_check_flag(transition, ENTITY_FLAG_UPDATE))
    {
        app->pause = 0;
    }
}

void transition_demo_register(eg_entity_type *t)
{
    t->width = 50;
    t->height = 50;
    t->render = render_transition;
    t->update = update_transition;
}

eg_entity *transition_demo_create(eg_app *app)
{
    eg_entity *transition = NULL;

    transition = eg_create_entity(app);
    if (transition == NULL)
    {
        return NULL;
    }

    transition->type = ENTITY_TYPE_TRANSITION;
    eg_set_flag(transition, ENTITY_FLAG_PAUSE);
    eg_set_flag(transition, ENTITY_FLAG_MENU);

    return transition;
}