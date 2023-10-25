#include "impl.h"

cr_texture *cr_impl_load_texture(cr_app *app, const char *path)
{
    cr_texture *texture;
    SDL_Renderer *r = app->impl->renderer;

    SDL_Surface *surface = IMG_Load(path);
    if (surface == NULL)
    {
        return NULL;
    }

    SDL_Texture *t = SDL_CreateTextureFromSurface(r, surface);
    SDL_FreeSurface(surface);
    if (t == NULL)
    {
        return NULL;
    }

    texture = (cr_texture *)malloc(sizeof(cr_texture));
    if (texture == NULL)
    {
        SDL_DestroyTexture(t);
        return NULL;
    }

    texture->img = t;

    // Add the font to the application's font list.
    app->textures[(app->texture_count)++] = texture;

    return texture;
}

void cr_impl_destroy_texture(cr_texture *texture)
{
    if (texture->img != NULL)
    {
        SDL_DestroyTexture(texture->img);
        texture->img = NULL;
    }
}

void cr_impl_draw_texture(cr_app *app, cr_texture *texture, cr_rect *src, cr_rect *dest, int mirror)
{
    SDL_Renderer *r = app->impl->renderer;
    SDL_Texture *sheet = texture->img;
    SDL_Rect s = {.x = src->x, .y = src->y, .w = src->w, .h = src->h};
    SDL_Rect d = {
        .x = app->origin_x + dest->x,
        .y = app->origin_y + dest->y,
        .w = dest->w,
        .h = dest->h};

    if (mirror)
    {
        SDL_RenderCopyEx(r, sheet, &s, &d, 0.0f, NULL, SDL_FLIP_HORIZONTAL);
        return;
    }

    SDL_RenderCopy(r, sheet, &s, &d);
}
