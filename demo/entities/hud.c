#include "demo/entities/hud.h"
#include "demo/font/font.h"
#include "demo/entities/entity_types.h"
#include "demo/demo.h"

#include <stdio.h>

#define HUD_BUFSIZE 256

static void render_hud(eg_app *app, eg_entity *hud)
{
    char buffer[HUD_BUFSIZE];
    int n = HUD_BUFSIZE - 1;

    eg_entity *corgi = app->primary;
    if (corgi == NULL)
    {
        return;
    }

    // render the HUD panel
    eg_rect r = {
        .x = hud->x_pos,
        .y = hud->y_pos,
        .w = app->entity_types[hud->type].width,
        .h = app->entity_types[hud->type].height};
    eg_set_color(app, EG_COLOR_BLACK);
    eg_draw_rect(app, &r, 1);

    // render the score
    int res = snprintf(
        buffer,
        n,
        "SCORE: %d",
        corgi->data);
    if (res < 0 || res >= n)
    {
        return;
    }
    eg_draw_text(app, app->fonts[DEMO_FONT_KENNY_PIXEL], buffer, 2, 0);

    // render the available music notes
    eg_draw_text(app, app->fonts[DEMO_FONT_KENNY_PIXEL], "MUSIC:", 2, 10);
    eg_set_color(app, EG_COLOR_PURPLE);
    for (int i = 0; i < 3 - app->counters[DEMO_COUNTER_NOTES]; i++)
    {
        r.x = 35 + i * 13;
        r.y = 10;
        r.w = 11;
        r.h = 11;
        eg_draw_rect(app, &r, 1);
    }
}

void hud_demo_register(eg_entity_type *t)
{
    t->id = ENTITY_TYPE_HUD;
    t->width = 240;
    t->height = 22;
    t->render = render_hud;
    t->collide = NULL;
}

eg_entity *hud_demo_create(eg_app *app, int x, int y)
{
    eg_entity *hud = NULL;

    hud = eg_create_entity(app);
    if (hud == NULL)
    {
        return NULL;
    }

    hud->type = ENTITY_TYPE_HUD;
    hud->x_pos = x;
    hud->y_pos = y;

    return hud;
}
