#ifndef EG_IMPLEMENTATION_H
#define EG_IMPLEMENTATION_H

// Simple DirectMedia Layer (SDL) is a library for thigns like creating
// windows and capturing user input.
// https://www.libsdl.org/
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>

// maximum number of characters in a font atlas
#define FONT_ATLAS_MAX 128

// The font modes indicate whether a font atlas uses one texture or
// multiple textures.
#define FONT_MODE_SINGLE 0
#define FONT_MODE_MULTI 1

struct eg_texture
{
    SDL_Texture *img;
};

struct eg_font
{
    int loaded; // initialization flag
    SDL_Texture *atlas;
    SDL_Texture *glyphs[FONT_ATLAS_MAX];
    eg_rect sizes[FONT_ATLAS_MAX];
    int mode; // 0 for single texture, 1 for multiple
};

// complete definition of the eg_impl type
struct eg_impl
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    const Uint8 *keystates;
    Uint64 ticks;
    Uint64 frame_len;
};

//----------------------------------------------------------------------------
// keyboard

/**
 * Populates an internal keycode lookup table.
 */
void eg_impl_init_keyboard();

/**
 * Converts an eg_keycode into an SDL_Scancode.
 *
 * Params:
 *   eg_keycode - a keycode from the public API
 *
 * Returns:
 *   SDL_Scancode - an internal representation of the keycode
 */
SDL_Scancode eg_impl_get_sdl_scancode(eg_keycode k);

/**
 * Converts an SDL_Scancode into an eg_keycode.
 *
 * Params:
 *   SDL_Scancode - an internal representation of the keycode
 *
 * Returns:
 *   eg_keycode - a keycode from the public API
 */
eg_keycode eg_impl_get_eg_keycode(SDL_Scancode sc);

//----------------------------------------------------------------------------
// font

/**
 * Reads from a TrueType font file and creates the textures containing glyphs
 * for rendering text. This function attempts to combine all of the necessary
 * glyphs from the font into one texture. If it is unable to do so, it will
 * use an individual texture for each character.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   const char* - the path to the TrueType file
 *   int - point size
 *
 * Returns:
 *   int - 1 on success, or 0 on failure
 */
// int eg_impl_load_font(eg_app *, const char *, int);
eg_font *eg_impl_load_font(eg_app *, const char *, int);

/**
 * Reads a sprite sheet from a PNG file.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   const char* - the path to the PNG file
 *
 * Returns:
 *   int - 1 on success, or 0 on failure
 */
int eg_impl_load_sprite_sheet(eg_app *, const char *);

/**
 * Renders a string of text to the screen.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   const char* - the text to render
 *   int - the x position of the text on the screen
 *   int - the y position of the text on the screen
 */
// void eg_impl_draw_text(eg_app *, const char *, int, int);
void eg_impl_draw_text(eg_app *, eg_font *, const char *, int, int);

/**
 * Renders a portion of a texure to the screen.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_texture* - the texure to render
 *   eg_rect* - a rectangular region of the texture that will be rendered
 *   eg_rect* - a rectangular region of the screen to which the texture
 *              will be rendered
 */
void eg_impl_draw_texture(eg_app *, eg_texture *, eg_rect *, eg_rect *);

#endif