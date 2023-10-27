#include "impl.h"

// 95 printable ASCII characters [32:126]
//  !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~

static int build_atlas_texture(SDL_Renderer *r, cr_font *font)
{
    SDL_Rect src;
    SDL_Rect dest;
    int dest_x = 0;
    SDL_BlendMode blend_mode;

    // Only rebuild the atlas for single texture fonts.
    // They are the ones that use a texture that is a render target.
    if (font->mode != FONT_MODE_SINGLE)
    {
        return 1;
    }

    // Get the current renderer blend mode.
    SDL_GetRenderDrawBlendMode(r, &blend_mode);

    // Set the blend mode for the atlas texture to allow for alpha values.
    SDL_SetTextureBlendMode(font->atlas, SDL_BLENDMODE_BLEND);

    // Set the font atlas texture as the current render target.
    if (SDL_SetRenderTarget(r, font->atlas))
    {
        fprintf(stderr,
                "failed to set render target for font atlas: %s\n",
                SDL_GetError());
        return 0;
    }

    // Copy each glyph texture into the target texture.
    for (int i = 32; i < 127; i++)
    {
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

    // Reset the renderer target and blend mode.
    SDL_SetRenderTarget(r, NULL);
    SDL_SetRenderDrawBlendMode(r, blend_mode);

    return 1;
}

static int init_font_atlas(SDL_Renderer *r, TTF_Font *ttf, cr_font *font)
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

    // Get the current renderer blend mode.
    SDL_GetRenderDrawBlendMode(r, &blend_mode);

    // Default to multi texture mode.
    font->mode = FONT_MODE_MULTI;

    for (int i = 0; i < FONT_ATLAS_MAX; i++)
    {
        font->sizes[i].x = 0xFF;
        font->sizes[i].y = 0xFF;
        font->sizes[i].w = 0xFF;
        font->sizes[i].h = 0xFF;
    }

    // Create a texture for each character.
    // 32 = ' '
    // 126 = '~'
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

        // Get the format of the first glyph texture.
        if (SDL_QueryTexture(font->glyphs[32], &format, NULL, NULL, NULL))
        {
            fprintf(stderr, "failed to query first glyph texture\n");
            return 0;
        }

        // Create the font atlas texture.
        // Since we use the SDL_TEXTUREACCESS_TARGET format, the font atlas
        // will need to be rebuilt when the SDL_RENDER_TARGETS_RESET event
        // occurs.
        font->atlas = SDL_CreateTexture(
            r,
            format,
            SDL_TEXTUREACCESS_TARGET,
            2048,
            24);
        if (font->atlas == NULL)
        {
            fprintf(stderr, "failed to create font atlas texture\n");
            return 0;
        }

        font->mode = FONT_MODE_SINGLE;

        if (!build_atlas_texture(r, font))
        {
            return 0;
        }
    }

    font->loaded = 1;

    return 1;
}

cr_font *cr_impl_load_font(cr_app *app, const char *path, int p)
{
    cr_font *font = NULL;
    cr_impl *impl = app->impl;

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
    font = (cr_font *)malloc(sizeof(cr_font));
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

void cr_impl_destroy_font(cr_font *font)
{
    for (int i = 0; i < FONT_ATLAS_MAX; i++)
    {
        if (font->glyphs[i] != NULL)
        {
            SDL_DestroyTexture(font->glyphs[i]);
        }
    }

    if (font->atlas != NULL)
    {
        SDL_DestroyTexture(font->atlas);
    }
}

/**
 * Renders a string of text to the screen using a font atlas that is composed
 * of multiple textures.
 */
static void impl_draw_text_multi(
    cr_app *app,
    cr_font *font,
    const char *msg,
    int x,
    int y,
    int line_w,
    int line_h,
    int *result)
{
    cr_impl *impl;
    int x0, y0; // line origin
    int dy;
    int glyph_w, glyph_h; // glyph dimensions
    int line_break;
    int q; // texture query result

    impl = app->impl;
    x0 = x;
    y0 = y;

    // If the line height is 0, default to the line height of the
    // 'A' character.
    dy = line_h > 0 ? line_h : font->sizes[(int)'A'].h;

    for (int i = 0; msg[i] != '\0'; i++)
    {
        line_break = 0;
        SDL_Texture *tex = font->glyphs[(int)msg[i]];
        q = SDL_QueryTexture(tex, NULL, NULL, &glyph_w, &glyph_h);

        // Detect line breaks.
        // Line breaks can occur due to the text exceeding the specified
        // line width, or the presence of a newline character.
        if (!q && ((line_w > 0 && x - x0 >= line_w) || msg[i] == '\n'))
        {
            x = x0;
            y += dy;
            line_break = 1;
        }

        // Only render printable characters to the screen.
        // Only increase the x position if we have not performed a line break.
        if (!q && msg[i] >= ' ' && msg[i] <= '~')
        {
            SDL_Rect r = {.x = x, .y = y, .w = glyph_w, .h = glyph_h};
            SDL_RenderCopy(impl->renderer, tex, NULL, &r);
            x += glyph_w * (line_break ^ 1);
        }
    }

    if (result != NULL)
    {
        *result = (y - y0) + dy;
    }
}

/**
 * Renders a string of text to the screen using a font atlas that is composed
 * of one single texture.
 */
static void impl_draw_text_single(
    cr_app *app,
    cr_font *font,
    const char *msg,
    int x,
    int y,
    int line_w,
    int line_h,
    int *result)
{
    cr_impl *impl;
    int x0, y0; // line origin
    int dy;
    int glyph_w;
    int line_break;
    SDL_Rect src, dest;

    impl = app->impl;
    x0 = x;
    y0 = y;

    // If the line height is 0, default to the line height of the
    // 'A' character.
    dy = line_h > 0 ? line_h : font->sizes[(int)'A'].h;

    for (int i = 0; msg[i] != '\0'; i++)
    {
        src.x = font->sizes[(int)msg[i]].x;
        src.y = 0;
        src.w = font->sizes[(int)msg[i]].w;
        src.h = font->sizes[(int)msg[i]].h;

        dest.x = x;
        dest.y = y;
        dest.w = font->sizes[(int)msg[i]].w;
        dest.h = font->sizes[(int)msg[i]].h;

        glyph_w = font->sizes[(int)msg[i]].w;

        line_break = 0;

        // Detect line breaks.
        // Line breaks can occur due to the text exceeding the specified
        // line width, or the presence of a newline character.
        if ((line_w > 0 && x - x0 >= line_w) || msg[i] == '\n')
        {
            x = x0;
            y += dy;
            line_break = 1;
        }

        // Only render printable characters to the screen.
        // Only increase the x position if we have not performed a line break.
        if (msg[i] >= ' ' && msg[i] <= '~')
        {
            SDL_RenderCopy(impl->renderer, font->atlas, &src, &dest);
            x += glyph_w * (line_break ^ 1);
        }
    }

    if (result != NULL)
    {
        *result = (y - y0) + dy;
    }
}

void cr_impl_draw_text(
    cr_app *app,
    cr_font *font,
    const char *msg,
    int x,
    int y)
{
    if (font->mode == FONT_MODE_MULTI)
    {
        impl_draw_text_multi(app, font, msg, x, y, 0, 0, NULL);
    }

    if (font->mode == FONT_MODE_SINGLE)
    {
        impl_draw_text_single(app, font, msg, x, y, 0, 0, NULL);
    }
}

void cr_impl_draw_text_bounded(
    cr_app *app,
    cr_font *font,
    const char *msg,
    cr_rect *bounds,
    int *result)
{
    if (font->mode == FONT_MODE_MULTI)
    {
        impl_draw_text_multi(
            app,
            font,
            msg,
            app->origin_x + bounds->x,
            app->origin_y + bounds->y,
            bounds->w,
            bounds->h,
            result);
    }

    if (font->mode == FONT_MODE_SINGLE)
    {
        impl_draw_text_single(
            app,
            font,
            msg,
            app->origin_x + bounds->x,
            app->origin_y + bounds->y,
            bounds->w,
            bounds->h,
            result);
    }
}

int cr_impl_rebuild_font_atlases(cr_app *app)
{
    int res = 1;
    for (int i = 0; i < app->font_count && res; i++)
    {
        res = build_atlas_texture(app->impl->renderer, app->fonts[i]);
    }

    return res;
}
