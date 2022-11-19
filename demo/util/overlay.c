#include "demo/util/overlay.h"
#include "demo/font/font.h"
#include "demo/entities/entity_types.h"
#include "demo/collision/collision.h"

#include <stdio.h>

#define DEBUG_BUFSIZE 256

void debug_draw_overlay(eg_app *app)
{
    char buffer[DEBUG_BUFSIZE];
    int n = DEBUG_BUFSIZE - 1;

    // To render numerical values on the screen, we use snprintf.
    // This is the modern replacement for sprintf.
    // Documentation for snprintf:
    // https://cplusplus.com/reference/cstdio/snprintf

    eg_entity *player = NULL;
    for (int i = 0; i < app->entity_cap && player == NULL; i++)
    {
        if (app->entities[i].type == ENTITY_TYPE_PLAYER && app->entities[i].present)
        {
            player = &(app->entities[i]);
        }
    }

    // The nul terminator is appended by the snprintf function.
    int res = snprintf(
        buffer,
        n,
        "camera: (%d, %d)",
        app->cam.x,
        app->cam.y);

    // The return value from snprintf must be greater than 0 and less than
    // the limit n.
    if (res < 0 || res >= n)
    {
        return;
    }

    eg_draw_text(app,
                 app->fonts[DEMO_FONT_KENNY_PIXEL],
                 buffer,
                 5,
                 5);

    int avy = app->entity_types[player->type].get_y_vel(player);

    res = snprintf(
        buffer,
        n,
        "x_acc: %d\n"
        "y_acc: %d\n"
        "x_pos: %d\n"
        "y_pos: %d\n"
        "y_vel: %d\n"
        "entity count: %d\n",
        player->x_acc,
        player->y_acc,
        player->x_pos,
        player->y_pos,
        avy,
        app->entity_count);

    // The return value from snprintf must be greater than 0 and less than
    // the limit n.
    if (res < 0 || res >= n)
    {
        return;
    }

    eg_draw_text(app,
                 app->fonts[DEMO_FONT_KENNY_PIXEL],
                 buffer,
                 5,
                 20);
}