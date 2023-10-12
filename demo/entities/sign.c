#include "demo/entities/sign.h"
#include "demo/entities/entity_types.h"
#include "demo/entities/sign_dialog.h"
#include "common/texture.h"
#include "demo/util/sprite.h"
#include "common/dialog.h"
#include "demo/input/input.h"

#include <stdio.h>
#include <stdlib.h>

static void render_sign(eg_app *app, eg_entity *sign)
{
    // Render the sign sprite.
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
        hit_box.w = app->entity_types[sign->type].width;
        hit_box.h = app->entity_types[sign->type].height;

        // Render the sign hit box.
        eg_set_color(app, EG_COLOR_VINIK_ORANGE);
        eg_draw_rect(app, &hit_box, 0);
    }
}

static void update_sign(eg_app *app, eg_entity *sign)
{
}

static int interact_with_sign(eg_app *app, eg_entity *sign, eg_entity *actor)
{
    app->pause = 1;

    // Locate the sign's dialog.
    eg_entity *sign_dialog = NULL;
    for (int i = 0; i < app->entity_cap; i++)
    {
        if (app->entities[i].type == ENTITY_TYPE_SIGN_DIALOG && app->entities[i].present)
        {
            sign_dialog = &(app->entities[i]);
        }
    }

    sign_dialog_demo_open(app, sign_dialog);

    eg_push_input_handler(app, common_dialog_input_handler);
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

eg_entity *sign_demo_create(eg_app *app, int x, int y)
{
    eg_entity *sign = NULL;

    sign = eg_create_entity(app);
    if (sign == NULL)
    {
        return NULL;
    }

    sign->type = ENTITY_TYPE_SIGN;
    sign->x_pos = x;
    sign->y_pos = y;

    return sign;
}