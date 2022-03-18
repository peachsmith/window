#include "example.h"
#include "impl.h"

int eg_impl_load_sprite_sheet(eg_app *app, const char *path)
{
    SDL_Renderer *r = app->impl->renderer;

    SDL_Surface *surface = IMG_Load(path);
    if (surface == NULL)
    {
        return 0;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(r, surface);
    SDL_FreeSurface(surface);
    if (texture == NULL)
    {
        return 0;
    }

    app->impl->sprite_sheet.img = texture;

    return 1;
}

void eg_impl_draw_image(eg_app *app, eg_rect *src, eg_rect *dest)
{
    SDL_Renderer *r = app->impl->renderer;
    SDL_Texture *sheet = app->impl->sprite_sheet.img;
    SDL_Rect s = {.x = src->x, .y = src->y, .w = src->w, .h = src->h};
    SDL_Rect d = {.x = dest->x, .y = dest->y, .w = dest->w, .h = dest->h};

    SDL_RenderCopy(r, sheet, &s, &d);
}
