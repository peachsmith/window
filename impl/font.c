#include "example.h"
#include "impl.h"

// 95 printable ASCII characters [32:126]
//  !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~

static int init_font_atlas(SDL_Renderer *r, TTF_Font *ttf, eg_font *font)
{
    SDL_RendererInfo ri;
    SDL_Color white = {.a = 255, .r = 255, .g = 255, .b = 255};
    SDL_Color clear = {.a = 0, .r = 255, .g = 255, .b = 255};
    SDL_BlendMode blend_mode;

    // Get renderer info.
    if (SDL_GetRendererInfo(r, &ri))
    {
        fprintf(stderr, "failed to get renderer info %s\n", SDL_GetError());
        return 0;
    }

    printf("[DEBUG] Renderer Info\n-------------\n");
    printf("[DEBUG] Name: %s\n", ri.name);
    printf("[DEBUG] Number of texture formats: %d\n", ri.num_texture_formats);
    printf("[DEBUG] Max Width: %d\n", ri.max_texture_width);
    printf("[DEBUG] Max Height: %d\n", ri.max_texture_height);
    printf("[DEBUG] SDL_RENDERER_SOFTWARE: %s\n", (ri.flags & SDL_RENDERER_SOFTWARE) ? "yes" : "no");
    printf("[DEBUG] SDL_RENDERER_ACCELERATED: %s\n", (ri.flags & SDL_RENDERER_ACCELERATED) ? "yes" : "no");
    printf("[DEBUG] SDL_RENDERER_PRESENTVSYNC: %s\n", (ri.flags & SDL_RENDERER_PRESENTVSYNC) ? "yes" : "no");
    printf("[DEBUG] SDL_RENDERER_TARGETTEXTURE: %s\n", (ri.flags & SDL_RENDERER_TARGETTEXTURE) ? "yes" : "no");

    // Get the current renderer blend mode.
    SDL_GetRenderDrawBlendMode(r, &blend_mode);

    // Default to multi texture mode.
    font->mode = FONT_MODE_MULTI;

    // Create a texture for each character.
    for (int i = 32; i < 127 && i < FONT_ATLAS_MAX; i++)
    {
        char c[2]; // string representation of a character
        SDL_Surface *surface;
        SDL_Texture *texture;

        // Build the string containing the character.
        c[0] = (char)i;
        c[1] = '\0';

        // Create an SDL_Surface from the font file.
        surface = TTF_RenderUTF8_Blended(ttf, c, white);
        if (surface == NULL)
        {
            return 0;
        }

        // Convert the SDL_Surface into an SDL_Texture.
        texture = SDL_CreateTextureFromSurface(r, surface);
        SDL_FreeSurface(surface);
        if (texture == NULL)
        {
            return 0;
        }

        // Get the width and height of the texture.
        SDL_QueryTexture(
            texture,
            NULL,
            NULL,
            &(font->sizes[i].w),
            &(font->sizes[i].h));

        font->sizes[i].x = 0;
        font->sizes[i].y = 0;

        // The x position of the current texture is the sum of the x
        // position and width of the previous texture.
        // This allows us to build a single tetxure that contains a row
        // of all the glyphs that can be rendered.
        if (i > 32)
        {
            font->sizes[i].x = font->sizes[i - 1].x + font->sizes[i - 1].w;
        }

        font->glyphs[i] = texture;
    }

    // If the renderer supports using textures as render targets,
    // then put all the glyph textures into one texture.
    if (ri.flags & SDL_RENDERER_TARGETTEXTURE)
    {
        Uint32 format;
        SDL_Texture *target;

        int dest_x = 0;

        // Get the format of the first glyph texture.
        if (SDL_QueryTexture(font->glyphs[32], &format, NULL, NULL, NULL))
        {
            fprintf(stderr, "failed to query first glyph texture\n");
            return 0;
        }

        // Create the font atlas texture.
        target = SDL_CreateTexture(
            r,
            format,
            SDL_TEXTUREACCESS_TARGET,
            2048,
            24);
        if (target == NULL)
        {
            fprintf(stderr, "failed to create font atlas texture\n");
            return 0;
        }

        // Set the blend mode for the atlas texture to allow for alpha values.
        SDL_SetTextureBlendMode(target, SDL_BLENDMODE_BLEND);

        // Set the font atlas texture as the current render target.
        if (SDL_SetRenderTarget(r, target))
        {
            fprintf(stderr,
                    "failed to set render target for font atlas: %s\n",
                    SDL_GetError());
            return 0;
        }

        // Copy each glyph texture into the target texture.
        for (int i = 32; i < 127; i++)
        {
            SDL_Rect src;
            SDL_Rect dest;

            src.x = 0;
            src.y = 0;
            src.w = font->sizes[i].w;
            src.h = font->sizes[i].h;

            dest.x = dest_x;
            dest.y = 0;
            dest.w = font->sizes[i].w;
            dest.h = font->sizes[i].h;

            // Set the glyph texture blending mode to none to prevent
            // loss of quality when copying it into the target texture.
            // Someone else had the same problem with quality loss when
            // copying glyph textures into an atlas texture.
            // Forum post: https://discourse.libsdl.org/t/solved-sdl-ttf-low-quality-glyphs/27089/5
            SDL_SetTextureBlendMode(font->glyphs[i], SDL_BLENDMODE_NONE);

            if (SDL_RenderCopy(r, font->glyphs[i], &src, &dest))
            {
                fprintf(
                    stderr,
                    "error copying glyph texture to atlas %s\n",
                    SDL_GetError());
                return 0;
            }

            dest_x += font->sizes[i].w;
        }

        font->atlas = target;
        font->mode = FONT_MODE_SINGLE;

        // Destroy the individual glyph textures since they are
        // no longer needed.
        for (int i = 32; i < 127; i++)
        {
            SDL_DestroyTexture(font->glyphs[i]);
            font->glyphs[i] = NULL;
        }

        // Reset the renderer target and blend mode.
        SDL_SetRenderTarget(r, NULL);
        SDL_SetRenderDrawBlendMode(r, blend_mode);
    }

    font->loaded = 1;

    return 1;
}

int eg_impl_load_font(eg_app *app, const char *path, int p)
{
    eg_impl *impl = app->impl;

    TTF_Font *ttf = TTF_OpenFont(path, p);
    if (ttf == NULL)
    {
        fprintf(stderr,
                "failed to load font. error: %s\n",
                TTF_GetError());
        return 0;
    }

    if (!init_font_atlas(impl->renderer, ttf, &(impl->font)))
    {
        fprintf(stderr,
                "failed to create font atlas\n");
        TTF_CloseFont(ttf);
        return 0;
    }

    TTF_CloseFont(ttf);
    return 1;
}

void eg_impl_draw_text(eg_app *app, const char *msg, int x, int y)
{
    eg_impl *impl = app->impl;

    if (!impl->font.loaded)
    {
        return;
    }

    // Render text using an individual texture for each glyph.
    if (impl->font.mode == FONT_MODE_MULTI)
    {
        int w;
        int h;
        for (int i = 0; msg[i] != '\0'; i++)
        {
            SDL_Texture *tex = impl->font.glyphs[(int)msg[i]];
            int q = SDL_QueryTexture(tex, NULL, NULL, &w, &h);
            if (!q)
            {
                SDL_Rect r = {.x = x, .y = y, .w = w, .h = h};
                x += w;
                SDL_RenderCopy(impl->renderer, tex, NULL, &r);
            }
        }
    }

    // Render text using a single texture atlas.
    if (impl->font.mode == FONT_MODE_SINGLE)
    {
        for (int i = 0; msg[i] != '\0'; i++)
        {
            SDL_Rect src = {
                .x = impl->font.sizes[(int)msg[i]].x,
                .y = 0,
                .w = impl->font.sizes[(int)msg[i]].w,
                .h = impl->font.sizes[(int)msg[i]].h};

            SDL_Rect dest = {
                .x = x,
                .y = y,
                .w = impl->font.sizes[(int)msg[i]].w,
                .h = impl->font.sizes[(int)msg[i]].h};

            x += impl->font.sizes[(int)msg[i]].w;

            SDL_RenderCopy(impl->renderer, impl->font.atlas, &src, &dest);
        }
    }
}
