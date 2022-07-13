#include "demo/util/overlay.h"
#include "demo/font/font.h"
#include "demo/entities/entity_types.h"

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

    // res = snprintf(
    //     buffer,
    //     n,
    //     "ground: %c\njump: %c\ndown: %c\nmove: %c\nupdate: %c\nslope: %c\nmirror: %c\n",
    //     eg_check_flag(app->player, ENTITY_FLAG_GROUND) ? 'Y' : 'N',
    //     eg_check_flag(app->player, ENTITY_FLAG_JUMP) ? 'Y' : 'N',
    //     eg_check_flag(app->player, ENTITY_FLAG_DOWN) ? 'Y' : 'N',
    //     eg_check_flag(app->player, ENTITY_FLAG_MOVE) ? 'Y' : 'N',
    //     eg_check_flag(app->player, ENTITY_FLAG_UPDATE) ? 'Y' : 'N',
    //     eg_check_flag(app->player, ENTITY_FLAG_SLOPE) ? 'Y' : 'N',
    //     eg_check_flag(app->player, ENTITY_FLAG_MIRROR) ? 'Y' : 'N');

    // // The return value from snprintf must be greater than 0 and less than
    // // the limit n.
    // if (res < 0 || res >= n)
    // {
    //     return;
    // }

    // eg_draw_text(app,
    //              app->fonts[DEMO_FONT_KENNY_PIXEL],
    //              buffer,
    //              5,
    //              20);
}