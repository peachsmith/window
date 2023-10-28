#include "demo/demo.h"
#include "demo/assets.h"
#include "demo/util/overlay.h"
#include "demo/entities/entity_types.h"

#include "common/util.h"
#include "common/collision.h"

#define DEBUG_BUFSIZE 256

void demo_draw_overlay(cr_app *app)
{
    char buffer[DEBUG_BUFSIZE];
    int n = DEBUG_BUFSIZE - 1;

    // To render numerical values on the screen, we use snprintf.
    // This is the modern replacement for sprintf.
    // Documentation for snprintf:
    // https://cplusplus.com/reference/cstdio/snprintf

    cr_entity *player = app->extension->entity_handles[DEMO_HANDLE_PLAYER];

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

    cr_draw_text(app,
                 app->fonts[DEMO_FONT_KENNY_PIXEL],
                 buffer,
                 5,
                 5);

    // int avy = app->entity_types[player->type].get_y_vel(player);

    if (player == NULL)
    {
        return;
    }

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

    cr_draw_text(app,
                 app->fonts[DEMO_FONT_KENNY_PIXEL],
                 buffer,
                 5,
                 20);

    // Render entity array
    int ex = 160;
    int ey = 3;
    cr_rect er = {.w = 5, .h = 5};
    for (int i = 0; i < app->entity_cap; i++)
    {
        er.x = ex + (i / 20) * (er.w + 1);
        er.y = ey + (i % 20) * (er.h + 1);

        if (app->entities[i].present)
        {
            switch (app->entities[i].type)
            {
            case ENTITY_TYPE_BILLY:
                cr_set_color(app, 0xfffa8ee3);
                break;

            case ENTITY_TYPE_JIMBO:
                cr_set_color(app, 0xffffed47);
                break;

            case ENTITY_TYPE_PLAYER:
                cr_set_color(app, 0xff78eb5b);
                break;

            case ENTITY_TYPE_BLOCK:
                cr_set_color(app, 0xffc49f0c);
                break;

            case ENTITY_TYPE_BLOCK_SLOPE:
                cr_set_color(app, 0xffbd80d9);
                break;

            case ENTITY_TYPE_BLOCK_MOVING:
                cr_set_color(app, 0xff739c3b);
                break;

            case ENTITY_TYPE_HENRY:
                cr_set_color(app, 0xfff24202);
                break;

            case ENTITY_TYPE_FIREBALL:
                cr_set_color(app, 0xfff28918);
                break;

            default:
                cr_set_color(app, 0xffa8a8a8);
                break;
            }

            if (cr_check_flag(&(app->entities[i]), ENTITY_FLAG_MENU))
            {
                cr_set_color(app, 0xff84f0ec);
            }

            cr_draw_rect(app, &er, 1);
        }
        else
        {
            cr_set_color(app, 0xffedece8);
            cr_draw_rect(app, &er, 0);
        }
    }

    // Render jump height control
    int act = app->actuation_counters[CR_KEYCODE_SPACE];
    res = snprintf(
        buffer,
        n,
        "actuation: %d\n",
        act);

    // The return value from snprintf must be greater than 0 and less than
    // the limit n.
    if (res < 0 || res >= n)
    {
        return;
    }

    cr_draw_text(app,
                 app->fonts[DEMO_FONT_KENNY_PIXEL],
                 buffer,
                 5,
                 70);

    er.x = 55;
    er.y = 83;
    er.w = 34;
    er.h = 7;
    cr_set_color(app, 0xffedece8);
    cr_draw_rect(app, &er, 0);

    er.x = 56;
    er.y = 84;
    er.w = act * 2;
    er.h = 5;
    cr_set_color(app, 0xffe85900);
    cr_draw_rect(app, &er, 1);

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

    // cr_draw_text(app,
    //              app->fonts[DEMO_FONT_KENNY_PIXEL],
    //              buffer,
    //              5,
    //              85);
}