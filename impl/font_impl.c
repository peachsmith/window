#include "example.h"
#include "impl.h"

// 95 printable ASCII characters [32:126]
//  !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~

static int init_font_atlas(SDL_Renderer *r, TTF_Font *ttf, eg_font *font)
{
    SDL_RendererInfo ri;
    SDL_Color white = {.a = 255, .r = 255, .g = 255, .b = 255};
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

eg_font *eg_impl_load_font(eg_app *app, const char *path, int p)
{
    eg_font *font = NULL;
    eg_impl *impl = app->impl;

    // Load the font data from the TrueType file.
    TTF_Font *ttf = TTF_OpenFont(path, p);
    if (ttf == NULL)
    {
        fprintf(stderr,
                "failed to load font. error: %s\n",
                TTF_GetError());
        return 0;
    }

    // Create the font object.
    font = (eg_font *)malloc(sizeof(eg_font));
    if (font == NULL)
    {
        TTF_CloseFont(ttf);
        return NULL;
    }

    // Zero out the font data.
    font->atlas = NULL;
    for (int i = 0; i < FONT_ATLAS_MAX; i++)
    {
        font->glyphs[i] = NULL;
    }

    if (!init_font_atlas(impl->renderer, ttf, font))
    {
        fprintf(stderr,
                "failed to create font atlas\n");
        free(font);
        TTF_CloseFont(ttf);
        return NULL;
    }

    TTF_CloseFont(ttf);

    // Add the font to the application's font list.
    app->fonts[(app->font_count)++] = font;

    return font;
}

/**
 * Renders a string of text to the screen using a font atlas that is composed
 * of multiple textures.
 */
static void impl_draw_text_multi(
    eg_app *app,
    eg_font *font,
    const char *msg,
    int x,
    int y,
    int line_width,
    int line_height)
{
    eg_impl *impl = app->impl;

    int w;
    int h;
    for (int i = 0; msg[i] != '\0'; i++)
    {
        SDL_Texture *tex = font->glyphs[(int)msg[i]];
        int q = SDL_QueryTexture(tex, NULL, NULL, &w, &h);
        if (!q)
        {
            SDL_Rect r = {.x = x, .y = y, .w = w, .h = h};
            x += w;
            SDL_RenderCopy(impl->renderer, tex, NULL, &r);
        }
    }
}

/**
 * Renders a string of text to the screen using a font atlas that is composed
 * of one single texture.
 */
static void impl_draw_text_single(
    eg_app *app,
    eg_font *font,
    const char *msg,
    int x,
    int y,
    int line_width,
    int line_height)
{
    eg_impl *impl = app->impl;

    int orig_x = x;

    for (int i = 0; msg[i] != '\0'; i++)
    {
        SDL_Rect src = {
            .x = font->sizes[(int)msg[i]].x,
            .y = 0,
            .w = font->sizes[(int)msg[i]].w,
            .h = font->sizes[(int)msg[i]].h};

        SDL_Rect dest = {
            .x = x,
            .y = y,
            .w = font->sizes[(int)msg[i]].w,
            .h = font->sizes[(int)msg[i]].h};

        x += font->sizes[(int)msg[i]].w;

        if (line_width > 0 && x >= line_width)
        {
            x = orig_x;
            // If the line height is 0, default to the line height of the
            // 'A' character.
            int dy = line_height > 0 ? line_height
                                     : font->sizes[(int)'A'].h;
            y += dy;
        }

        if (msg[i] == '\n')
        {
            x = orig_x;
            int dy = line_height > 0 ? line_height
                                     : font->sizes[(int)'A'].h;
            y += dy;
        }
        else
        {
            SDL_RenderCopy(impl->renderer, font->atlas, &src, &dest);
        }
    }
}

void eg_impl_draw_text(
    eg_app *app,
    eg_font *font,
    const char *msg,
    int x,
    int y)
{
    if (font->mode == FONT_MODE_MULTI)
    {
        impl_draw_text_multi(app, font, msg, x, y, 0, 0);
    }

    if (font->mode == FONT_MODE_SINGLE)
    {
        impl_draw_text_single(app, font, msg, x, y, 0, 0);
    }
}

void eg_impl_draw_text_bounded(
    eg_app *app,
    eg_font *font,
    const char *msg,
    eg_rect *bounds)
{
    if (font->mode == FONT_MODE_MULTI)
    {
        impl_draw_text_multi(
            app,
            font,
            msg,
            bounds->x,
            bounds->y,
            bounds->w,
            bounds->h);
    }

    if (font->mode == FONT_MODE_SINGLE)
    {
        impl_draw_text_single(
            app,
            font,
            msg,
            bounds->x,
            bounds->y,
            bounds->w,
            bounds->h);
    }
}
