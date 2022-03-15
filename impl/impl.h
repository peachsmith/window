#ifndef EG_IMPLEMENTATION_H
#define EG_IMPLEMENTATION_H

// Simple DirectMedia Layer (SDL) is a library for thigns like creating
// windows and capturing user input.
// https://www.libsdl.org/
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>

// maximum number of characters in a font atlas
#define FONT_ATLAS_MAX 128

// The font modes indicate whether a font atlas uses one texture or
// multiple textures.
#define FONT_MODE_SINGLE 0
#define FONT_MODE_MULTI 1

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
    eg_font font;
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
 */
int eg_impl_load_font(eg_app *, const char *, int);

/**
 * Renders a string of text to the screen.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   const char* - the text to render
 *   int - the x position of the text on the screen
 *   int - the y position of the text on the screen
 */
void eg_impl_draw_text(eg_app *, const char *, int, int);

#endif