#include "demo/util/sprite.h"
#include "demo/texture/texture.h"

void sprite_draw_character(eg_app *app, int x, int y)
{
    // tile dimensions
    int tile_w = 24;
    int tile_h = 24;

    // tile coordinates
    int tile_x = 0;
    int tile_y = 0;

    eg_rect src = {
        .x = tile_x * tile_w,
        .y = tile_y * tile_h,
        .w = tile_w,
        .h = tile_h};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    eg_draw_texture(
        app,
        app->textures[DEMO_TEXTURE_CHARACTERS],
        &src,
        &dest);
}

void sprite_draw_brick(eg_app *app, int x, int y)
{
    // tile dimensions
    int tile_w = 18;
    int tile_h = 18;

    // tile coordinates
    int tile_x = 7;
    int tile_y = 2;

    eg_rect src = {
        .x = tile_x * tile_w,
        .y = tile_y * tile_h,
        .w = tile_w,
        .h = tile_h};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    eg_draw_texture(
        app,
        app->textures[DEMO_TEXTURE_SCENERY],
        &src,
        &dest);
}
