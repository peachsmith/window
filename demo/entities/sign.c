#include "demo/input/input.h"
#include "demo/entities/entity_types.h"
#include "demo/entities/sign.h"
#include "demo/entities/sign_dialog.h"
#include "demo/util/sprite.h"

#include "common/dialog.h"
#include "common/util.h"

#include <stdio.h>
#include <stdlib.h>

static void render_sign(cr_app *app, cr_entity *sign)
{
    if (!util_is_on_screen(app, sign))
    {
        return;
    }
    
    // Render the sign sprite.
    sprite_draw_sign(
        app,
        sign->x_pos + app->cam.x,
        sign->y_pos + app->cam.y);

    // hit box
    if (app->debug.hitboxes)
    {
        cr_rect hit_box;
        hit_box.x = sign->x_pos + app->cam.x;
        hit_box.y = sign->y_pos + app->cam.y;
        hit_box.w = app->entity_types[sign->type].width;
        hit_box.h = app->entity_types[sign->type].height;

        // Render the sign hit box.
        cr_set_color(app, CR_COLOR_VINIK_ORANGE);
        cr_draw_rect(app, &hit_box, 0);
    }
}

static void update_sign(cr_app *app, cr_entity *sign)
{
}

static int interact_with_sign(cr_app *app, cr_entity *sign, cr_entity *actor)
{
    app->pause = 1;

    // Locate the sign's dialog.
    cr_entity *sign_dialog = NULL;
    for (int i = 0; i < app->entity_cap; i++)
    {
        if (app->entities[i].type == ENTITY_TYPE_SIGN_DIALOG && app->entities[i].present)
        {
            sign_dialog = &(app->entities[i]);
        }
    }

    demo_open_sign_dialog(app, sign_dialog);

    cr_push_input_handler(app, demo_common_dialog_input);
    return 0;
}

void demo_register_sign(cr_entity_type *t)
{
    t->width = 18;
    t->height = 18;
    t->render = render_sign;
    t->update = update_sign;
    t->interactable = 1;
    t->interact = interact_with_sign;
}

cr_entity *demo_create_sign(cr_app *app, int x, int y)
{
    cr_entity *sign = NULL;

    sign = cr_create_entity(app);
    if (sign == NULL)
    {
        return NULL;
    }

    sign->type = ENTITY_TYPE_SIGN;
    sign->x_pos = x;
    sign->y_pos = y;

    return sign;
}