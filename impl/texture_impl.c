#include "example.h"
#include "impl.h"

eg_texture *eg_impl_load_texture(eg_app *app, const char *path)
{
    eg_texture *texture;
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

    texture = (eg_texture *)malloc(sizeof(eg_texture));
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

void eg_impl_destroy_texture(eg_texture *texture)
{
    if (texture->img != NULL)
    {
        SDL_DestroyTexture(texture->img);
        texture->img = NULL;
    }
}

void eg_impl_draw_texture(eg_app *app, eg_texture *texture, eg_rect *src, eg_rect *dest, int mirror)
{
    SDL_Renderer *r = app->impl->renderer;
    SDL_Texture *sheet = texture->img;
    SDL_Rect s = {.x = src->x, .y = src->y, .w = src->w, .h = src->h};
    SDL_Rect d = {.x = dest->x, .y = dest->y, .w = dest->w, .h = dest->h};

    if (mirror)
    {
        SDL_RenderCopyEx(r, sheet, &s, &d, 0.0f, NULL, SDL_FLIP_HORIZONTAL);
        return;
    }

    SDL_RenderCopy(r, sheet, &s, &d);
}
