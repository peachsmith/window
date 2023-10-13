#ifndef CRUMBS_IMPLEMENTATION_H
#define CRUMBS_IMPLEMENTATION_H

#include "crumbs.h"

// Simple DirectMedia Layer (SDL) is a library for thigns like creating
// windows and capturing user input.
// https://www.libsdl.org/
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include <stdio.h>
#include <stdlib.h>

// maximum number of characters in a font atlas
#define FONT_ATLAS_MAX 128

// The font modes indicate whether a font atlas uses one texture or
// multiple textures.
#define FONT_MODE_SINGLE 0
#define FONT_MODE_MULTI 1

typedef struct cr_timing cr_timing;

struct cr_texture
{
    SDL_Texture *img;
};

struct cr_font
{
    int loaded; // initialization flag
    SDL_Texture *atlas;
    SDL_Texture *glyphs[FONT_ATLAS_MAX];
    cr_rect sizes[FONT_ATLAS_MAX];
    int mode; // 0 for single texture, 1 for multiple
};

struct cr_sound
{
    int loaded;       // initialization flag
    int type;         // 1 for chunk, 2 for music
    Mix_Chunk *chunk; // sound effect
    Mix_Music *music; // music
};

struct cr_timing
{
    Uint64 frequency;
    Uint64 count;
    float delta;
};

// complete definition of the cr_impl type
struct cr_impl
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    const Uint8 *keystates;
    Uint64 ticks;
    Uint64 frame_len;
    cr_timing timing;
};

//----------------------------------------------------------------------------
// keyboard

/**
 * Populates an internal keycode lookup table.
 */
void cr_impl_init_keyboard();

/**
 * Converts an cr_keycode into an SDL_Scancode.
 *
 * Params:
 *   cr_keycode - a keycode from the public API
 *
 * Returns:
 *   SDL_Scancode - an internal representation of the keycode
 */
SDL_Scancode cr_impl_get_sdl_scancode(cr_keycode k);

/**
 * Converts an SDL_Scancode into an cr_keycode.
 *
 * Params:
 *   SDL_Scancode - an internal representation of the keycode
 *
 * Returns:
 *   cr_keycode - a keycode from the public API
 */
cr_keycode cr_impl_get_cr_keycode(SDL_Scancode sc);

//----------------------------------------------------------------------------
// textures

/**
 * Loads a texture from a PNG file.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   const char* - file path of the PNG file
 *
 * Returns:
 *   int - 1 on success or 0 on failure
 */
cr_texture *cr_impl_load_texture(cr_app *, const char *);

/**
 * Frees the memory allocated for a texture.
 *
 * Params:
 *   cr_texture* - a pointer to a texture
 */
void cr_impl_destroy_texture(cr_texture *);

/**
 * Renders a portion of a texure to the screen.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   cr_texture* - the texure to render
 *   cr_rect* - a rectangular region of the texture that will be rendered
 *   cr_rect* - a rectangular region of the screen to which the texture
 *              will be rendered
 */
void cr_impl_draw_texture(cr_app *, cr_texture *, cr_rect *, cr_rect *, int);

//----------------------------------------------------------------------------
// font

/**
 * Reads from a TrueType font file and creates the textures containing glyphs
 * for rendering text. This function attempts to combine all of the necessary
 * glyphs from the font into one texture. If it is unable to do so, it will
 * use an individual texture for each character.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   const char* - the path to the TrueType file
 *   int - point size
 *
 * Returns:
 *   int - 1 on success, or 0 on failure
 */
cr_font *cr_impl_load_font(cr_app *, const char *, int);

/**
 * Frees the memory allocated for a font.
 *
 * Params:
 *   cr_font* - a pointer to a font
 */
void cr_impl_destroy_font(cr_font *);

/**
 * Reads a sprite sheet from a PNG file.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   const char* - the path to the PNG file
 *
 * Returns:
 *   int - 1 on success, or 0 on failure
 */
int cr_impl_load_sprite_sheet(cr_app *, const char *);

/**
 * Renders a string of text to the screen.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   cr_font* - a pointer to a font
 *   const char* - the text to render
 *   int - the x position of the text on the screen
 *   int - the y position of the text on the screen
 */
void cr_impl_draw_text(cr_app *, cr_font *, const char *, int, int);

/**
 * Renders a string of text within a rectangular boundary.
 * Newlines will be rendered to attempt to fit the text within the rectangle.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   cr_font* - a pointer to a font
 *   const char* - the text to render
 *   cr_rect* - a pointer to a rectangular area of the screen to hold the text
 */
void cr_impl_draw_text_bounded(cr_app *, cr_font *, const char *, cr_rect *, int *);

//----------------------------------------------------------------------------
// audio

/**
 * Reads audio data from a file.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   const char* - the path to the audio file
 *   int - the type of sound (1 for sound effect, 2 for music)
 *
 * Returns:
 *   int - 1 on success, or 0 on failure
 */
cr_sound *cr_impl_load_sound(cr_app *, const char *, int);

/**
 * Plays audio.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   cr_sound* - a pointer to a sound
 */
void cr_impl_play_sound(cr_app *, cr_sound *);

/**
 * Frees the memory allocated for a sound.
 *
 * Params:
 *   cr_sound* - a pointer to a sound
 */
void cr_impl_destroy_sound(cr_sound *);

#endif