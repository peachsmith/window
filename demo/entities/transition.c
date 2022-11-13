#include "demo/entities/sign.h"
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
    // printf("transition ticks: %d\n", transition->ticks);
    if (transition->ticks < 60)
    {
        a = transition->ticks * 4;
        if (transition->ticks >= 59)
        {
            a = 0xFF;
        }
    }
    else
    {
        a = 0xFF - ((transition->ticks - 60) * 4);
        if (transition->ticks >= 119)
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

    if (transition->ticks < 120)
    {
        transition->ticks++;
        return;
    }

    transition->ticks = 0;
    transition->data = 0;

    printf("transition complete\n");
}

void transition_demo_register(eg_entity_type *t)
{
    t->width = 50;
    t->height = 50;
    t->render = render_transition;
    t->update = update_transition;
}

eg_entity *transition_demo_create()
{
    eg_entity *transition = NULL;

    transition = eg_create_entity();
    if (transition == NULL)
    {
        return NULL;
    }

    transition->type = ENTITY_TYPE_TRANSITION;

    return transition;
}