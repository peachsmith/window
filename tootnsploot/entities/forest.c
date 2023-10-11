#include "tootnsploot/tootnsploot.h"
#include "tootnsploot/entities/entity_types.h"
#include "tootnsploot/entities/forest.h"
#include "tootnsploot/util/sprite.h"

static void render_forest(eg_app *app, eg_entity *forest)
{
    sprite_draw_forest(app, forest->x_pos, forest->y_pos);
}

void tns_register_forest(eg_entity_type *t)
{
    t->id = ENTITY_TYPE_FOREST;
    t->width = 240;
    t->height = 120;
    t->render = render_forest;
    t->collide = NULL;
}

eg_entity *tns_create_forest(eg_app *app, int x, int y)
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
