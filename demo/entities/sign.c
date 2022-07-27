#include "demo/entities/sign.h"
#include "demo/entities/entity_types.h"
#include "demo/texture/texture.h"
#include "demo/util/sprite.h"
#include "demo/dialog/dialog.h"

#include <stdio.h>
#include <stdlib.h>

static void render_sign(eg_app *app, eg_entity *sign)
{
    // Render the player sprite.
    sprite_draw_sign(
        app,
        sign->x_pos + app->cam.x,
        sign->y_pos + app->cam.y);

    // hit box
    if (app->debug.hitboxes)
    {
        eg_rect hit_box;
        hit_box.x = sign->x_pos + app->cam.x;
        hit_box.y = sign->y_pos + app->cam.y;
        hit_box.w = app->registry[sign->type].width;
        hit_box.h = app->registry[sign->type].height;

        // Render the player hit box.
        eg_set_color(app, EG_COLOR_ORANGE);
        eg_draw_rect(app, &hit_box, 0);
    }
}

static void update_sign(eg_app *app, eg_entity *player)
{
}

static int interact_with_sign(eg_app *app, eg_entity *sign, eg_entity *actor)
{
    app->pause = 1;
    demo_open_sign_dialog(app);
    return 0;
}

void sign_demo_register(eg_entity_type *t)
{
    t->width = 18;
    t->height = 18;
    t->render = render_sign;
    t->update = update_sign;
    t->interactable = 1;
    t->interact = interact_with_sign;
}

eg_entity *sign_demo_create(int x, int y)
{
    eg_entity *sign = NULL;

    sign = eg_create_entity();
    if (sign == NULL)
    {
        return NULL;
    }

    sign->type = ENTITY_TYPE_SIGN;
    sign->x_pos = x;
    sign->y_pos = y;

    return sign;
}