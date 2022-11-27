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

    // int avy = app->entity_types[player->type].get_y_vel(player);

    res = snprintf(
        buffer,
        n,
        "x_acc: %d\n"
        "y_acc: %d\n"
        "x_pos: %d\n"
        "y_pos: %d\n",
        player->x_acc,
        player->y_acc,
        player->x_pos,
        player->y_pos);

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

    // Render entity array
    int ex = 160;
    int ey = 3;
    eg_rect er = {.w = 5, .h = 5};
    for (int i = 0; i < app->entity_cap; i++)
    {
        er.x = ex + (i / 20) * (er.w + 1);
        er.y = ey + (i % 20) * (er.h + 1);

        if (app->entities[i].present)
        {
            switch (app->entities[i].type)
            {
            case ENTITY_TYPE_BILLY:
                eg_set_color(app, 0xfffa8ee3);
                break;

            case ENTITY_TYPE_JIMBO:
                eg_set_color(app, 0xffffed47);
                break;

            case ENTITY_TYPE_PLAYER:
                eg_set_color(app, 0xff78eb5b);
                break;

            case ENTITY_TYPE_BLOCK:
                eg_set_color(app, 0xffc49f0c);
                break;

            case ENTITY_TYPE_BLOCK_SLOPE:
                eg_set_color(app, 0xffbd80d9);
                break;

            case ENTITY_TYPE_BLOCK_MOVING:
                eg_set_color(app, 0xff739c3b);
                break;

            case ENTITY_TYPE_HENRY:
                eg_set_color(app, 0xfff24202);
                break;

            case ENTITY_TYPE_FIREBALL:
                eg_set_color(app, 0xfff28918);
                break;

            default:
                eg_set_color(app, 0xffa8a8a8);
                break;
            }

            if (eg_check_flag(&(app->entities[i]), ENTITY_FLAG_MENU))
            {
                eg_set_color(app, 0xff84f0ec);
            }

            eg_draw_rect(app, &er, 1);
        }
        else
        {
            eg_set_color(app, 0xffedece8);
            eg_draw_rect(app, &er, 0);
        }
    }

    // Render jump height control
    int act = app->actuation_counters[EG_KEYCODE_SPACE];
    res = snprintf(
        buffer,
        n,
        "acutation: %d\n",
        act);

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
                 70);

    er.x = 55;
    er.y = 83;
    er.w = 32;
    er.h = 7;
    eg_set_color(app, 0xffedece8);
    eg_draw_rect(app, &er, 0);

    er.x = 56;
    er.y = 84;
    er.w = act * 2;
    er.h = 5;
    eg_set_color(app, 0xffe85900);
    eg_draw_rect(app, &er, 1);

    // // Render frame rate information
    // res = snprintf(
    //     buffer,
    //     n,
    //     "fps: %.2f\n",
    //     app->debug.fps);

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
    //              85);
}