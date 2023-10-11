#include "tootnsploot/assets.h"
#include "tootnsploot/util/sprite.h"
#include "common/texture.h"

#include "colors.h"

void sprite_draw_corgi(eg_app *app, int x, int y, int mirror, int tile)
{
    // tile dimensions
    int tile_w = 24;
    int tile_h = 24;

    // tile coordinates
    int tile_x = tile;
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
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        mirror);
}

void sprite_draw_note(eg_app *app, int x, int y)
{
    // tile dimensions
    int tile_w = 24;
    int tile_h = 24;

    // tile coordinates
    int tile_x = 4;
    int tile_y = 0;

    // The note sprite is 11 x 11 pixels, so we subtract 13
    // from the tile width and height.
    eg_rect src = {
        .x = tile_x * tile_w,
        .y = tile_y * tile_h,
        .w = tile_w - 13,
        .h = tile_h - 13};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w - 13,
        .h = tile_h - 13};

    eg_draw_texture(
        app,
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        0);
}

void sprite_draw_critter(eg_app *app, int x, int y, int mirror, int tile)
{
    // tile dimensions
    int tile_w = 20;
    int tile_h = 12;

    eg_rect src = {
        .x = 96 + tile * tile_w,
        .y = 11,
        .w = tile_w,
        .h = tile_h};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    eg_draw_texture(
        app,
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        mirror);
}

void sprite_draw_sparkle(eg_app *app, int x, int y, int tile)
{
    // tile dimensions
    int tile_w = 19;
    int tile_h = 9;

    eg_rect src = {
        .x = 116 + tile * tile_w,
        .y = 0,
        .w = tile_w,
        .h = tile_h};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    eg_draw_texture(
        app,
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        0);
}

void sprite_draw_puff(eg_app *app, int x, int y, int tile)
{
    // tile dimensions
    int tile_w = 16;
    int tile_h = 8;

    eg_rect src = {
        .x = 158 + tile * tile_w,
        .y = 10,
        .w = tile_w,
        .h = tile_h};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    eg_draw_texture(
        app,
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        0);
}

void sprite_draw_leaves(eg_app *app, int x, int y, int tile)
{
    // tile dimensions
    int tile_w = 20;
    int tile_h = 12;

    eg_rect src = {
        .x = 96 + tile * tile_w,
        .y = 24,
        .w = tile_w,
        .h = tile_h};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = tile_w,
        .h = tile_h};

    eg_draw_texture(
        app,
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        0);
}

void sprite_draw_trumpet(eg_app *app, int x, int y, int mirror)
{
    eg_rect src = {
        .x = 137,
        .y = 25,
        .w = 14,
        .h = 8};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = 14,
        .h = 8};

    eg_draw_texture(
        app,
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        mirror);
}

void sprite_draw_ground(eg_app *app, int x, int y)
{
    eg_rect src = {
        .x = 0,
        .y = 24,
        .w = 24,
        .h = 18};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = 24,
        .h = 18};

    eg_draw_texture(
        app,
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        0);
}

void sprite_draw_forest(eg_app * app, int x, int y)
{
    eg_rect src = {
        .x = 0,
        .y = 44,
        .w = 240,
        .h = 120};

    eg_rect dest = {
        .x = x,
        .y = y,
        .w = 240,
        .h = 120};

    eg_draw_texture(
        app,
        app->textures[TNS_TEXTURE_SPRITES],
        &src,
        &dest,
        0);
}
