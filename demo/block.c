#include "demo/block.h"
#include "demo/entity_types.h"

static void render_block(eg_app *app, eg_entity *block)
{
    SDL_Rect r;
    r.x = block->x_pos;
    r.y = block->y_pos;
    r.w = app->registry[block->id].width;
    r.h = app->registry[block->id].height;

    SDL_SetRenderDrawColor(app->renderer, 0x02, 0x75, 0xD8, 0xFF);
    SDL_RenderFillRect(app->renderer, &r);
}

void block_demo_register(eg_entity_type *t)
{
    t->id = ENTITY_TYPE_BLOCK;
    t->width = 15;
    t->height = 15;
    t->render = render_block;
    // There is currently no update function for the block entity.
}

void block_demo_register_big(eg_entity_type *t)
{
    t->id = ENTITY_TYPE_BIG_BLOCK;
    t->width = 50;
    t->height = 50;
    t->render = render_block;
    // There is currently no update function for the block entity.
}

eg_entity *block_demo_create(int x, int y)
{
    eg_entity *block = NULL;

    block = eg_create_entity();
    if (block == NULL)
    {
        return NULL;
    }

    block->id = ENTITY_TYPE_BLOCK;
    block->x_pos = x;
    block->y_pos = y;

    return block;
}

eg_entity *block_demo_create_big(int x, int y)
{
    eg_entity *block = NULL;

    block = eg_create_entity();
    if (block == NULL)
    {
        return NULL;
    }

    block->id = ENTITY_TYPE_BIG_BLOCK;
    block->x_pos = x;
    block->y_pos = y;

    return block;
}
