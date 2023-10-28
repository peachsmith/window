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

void cr_impl_pad_window(cr_app *app)
{
    int win_w, win_h;
    int h0;
    SDL_Rect pad;

    SDL_GetWindowSize(app->impl->window, &win_w, &win_h);
    pad.x = 0;
    pad.y = 0;
    pad.w = app->origin_x;
    h0 = pad.h = win_h;
    pad.h /= app->scale;
    pad.h += h0 - pad.h * app->scale;

    cr_set_color(app, 0xFF000000);

    if (win_w > app->screen_width * app->scale)
    {
        // left
        SDL_RenderFillRect(app->impl->renderer, &pad);

        // right
        pad.x = app->origin_x + app->screen_width;
        pad.w += app->scale;
        SDL_RenderFillRect(app->impl->renderer, &pad);
    }

    if (win_h > app->screen_height * app->scale)
    {
        // top
        pad.x = app->origin_x;
        pad.w = app->screen_width;
        pad.h = app->origin_y;
        SDL_RenderFillRect(app->impl->renderer, &pad);

        // bottom
        pad.y = app->origin_y + app->screen_height;
        pad.h += app->scale;
        SDL_RenderFillRect(app->impl->renderer, &pad);
    }
}
