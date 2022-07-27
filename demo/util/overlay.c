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

    res = snprintf(
        buffer,
        n,
        "x_acc: %d\n"
        "y_acc: %d\n",
        app->player->x_acc,
        app->player->y_acc);

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

    eg_draw_text(app,
                 app->fonts[DEMO_FONT_KENNY_PIXEL],
                 "collisions:",
                 5,
                 45);

    if (app->col_count > 0)
    {
        int x = 15;
        int y = 55;

        for (int i = 0; i < app->col_count; i++)
        {
            res = snprintf(
                buffer,
                n,
                "%s",
                (app->dbg_collision[i] == 2) ? "LINE" : "BLOCK");

            // The return value from snprintf must be greater than 0 and less than
            // the limit n.
            if (res < 0 || res >= n)
            {
                return;
            }

            eg_draw_text(app,
                         app->fonts[DEMO_FONT_KENNY_PIXEL],
                         buffer,
                         x,
                         y);

            x += 35;
        }
    }
}