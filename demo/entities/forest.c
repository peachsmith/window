#include "demo/entities/forest.h"
#include "demo/entities/entity_types.h"

#include <stdlib.h>

static void render_forest(eg_app *app, eg_entity *hud)
{
    // canopy
    eg_rect canopy = {
        .x = hud->x_pos,
        .y = hud->y_pos,
        .w = app->entity_types[hud->type].width,
        .h = 20};

    eg_set_color(app, EG_COLOR_FOREST_GREEN);
    eg_draw_rect(app, &canopy, 1);

    // trunks and branches
    eg_rect trees_and_branches = {
        .x = hud->x_pos,
        .y = hud->y_pos + 20,
        .w = app->entity_types[hud->type].width,
        .h = app->entity_types[hud->type].height - 20};

    eg_set_color(app, EG_COLOR_GREEN_GRAY);
    eg_draw_rect(app, &trees_and_branches, 1);
}

void forest_demo_register(eg_entity_type *t)
{
    t->id = ENTITY_TYPE_FOREST;
    t->width = 240;
    t->height = 120;
    t->render = render_forest;
    t->collide = NULL;
}

eg_entity *forest_demo_create(eg_app *app, int x, int y)
{
    eg_entity *forest = NULL;

    forest = eg_create_entity(app);
    if (forest == NULL)
    {
        return NULL;
    }

    forest->type = ENTITY_TYPE_FOREST;
    forest->x_pos = x;
    forest->y_pos = y;

    return forest;
}
