#include "tootnsploot/tootnsploot.h"
#include "tootnsploot/assets.h"
#include "tootnsploot/entities/entity_types.h"
#include "tootnsploot/util/overlay.h"

#include "common/util.h"
#include "common/collision.h"

#define DEBUG_BUFSIZE 256

void tns_draw_overlay(cr_app *app)
{
    char buffer[DEBUG_BUFSIZE];
    int n = DEBUG_BUFSIZE - 1;

    cr_entity *player = app->primary;

    int res = snprintf(
        buffer,
        n,
        "camera: (%d, %d)",
        app->cam.x,
        app->cam.y);
    if (res < 0 || res >= n)
    {
        return;
    }

    cr_draw_text(app,
                 app->fonts[TNS_FONT_KENNY_PIXEL],
                 buffer,
                 5,
                 5);

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
    if (res < 0 || res >= n)
    {
        return;
    }

    cr_draw_text(app,
                 app->fonts[TNS_FONT_KENNY_PIXEL],
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
            case ENTITY_TYPE_CRITTER:
                cr_set_color(app, 0xffa16800);
                break;

            case ENTITY_TYPE_CORGI:
                cr_set_color(app, 0xffffd342);
                break;

            case ENTITY_TYPE_NOTE:
                cr_set_color(app, 0xff8d04cc);
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
    if (res < 0 || res >= n)
    {
        return;
    }

    cr_draw_text(app,
                 app->fonts[TNS_FONT_KENNY_PIXEL],
                 buffer,
                 5,
                 70);

    er.x = 55;
    er.y = 83;
    er.w = 32;
    er.h = 7;
    cr_set_color(app, 0xffedece8);
    cr_draw_rect(app, &er, 0);

    er.x = 56;
    er.y = 84;
    er.w = act * 2;
    er.h = 5;
    cr_set_color(app, 0xffe85900);
    cr_draw_rect(app, &er, 1);
}