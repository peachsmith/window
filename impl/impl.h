#ifndef EG_IMPLEMENTATION_H
#define EG_IMPLEMENTATION_H

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

struct eg_sound
{
    int loaded;       // initialization flag
    int type;         // 1 for chunk, 2 for music
    Mix_Chunk *chunk; // sound effect
    Mix_Music *music; // music
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
// core

int eg_impl_init();

void eg_impl_term();

eg_impl *eg_impl_create(int, int, int);

void eg_impl_destroy(eg_impl *);

void eg_impl_process_events(eg_app *);

void eg_impl_delay(eg_app *);

//----------------------------------------------------------------------------
// graphics

void eg_impl_clear_screen(eg_app *);

void eg_impl_render_screen(eg_app *);

void eg_impl_set_color(eg_app *app, uint32_t color);

void eg_impl_draw_line(eg_app *app, eg_point *a, eg_point *b);

void eg_impl_draw_rect(eg_app *app, eg_rect *r, int filled);

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

/**
 * Checks to see if an input is actuated.
 * As long as the input is actuated, this function will return 1. This is used
 * for handling input events where some task must be performed for as long as
 * the input is actuated.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   int - the scancode of the key to check
 */
int eg_impl_peek_key(eg_app *, int);

/**
 * Checks to see if an input is actuated.
 * The first time the input is actuated, this function will return 1.
 * All subsequent calls to this function will return 0 until the state of
 * the input in question is reset. This is used for handling input events
 * where a task must be performed a single time for a single input actuation.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   int - the scancode of the key to check
 */
int eg_impl_consume_key(eg_app *, int);

//----------------------------------------------------------------------------
// textures

/**
 * Loads a texture from a PNG file.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   const char* - file path of the PNG file
 *
 * Returns:
 *   int - 1 on success or 0 on failure
 */
eg_texture *eg_impl_load_texture(eg_app *, const char *);

/**
 * Frees the memory allocated for a texture.
 *
 * Params:
 *   eg_texture* - a pointer to a texture
 */
void eg_impl_destroy_texture(eg_texture *);

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
void eg_impl_draw_texture(eg_app *, eg_texture *, eg_rect *, eg_rect *, int);

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
eg_font *eg_impl_load_font(eg_app *, const char *, int);

/**
 * Frees the memory allocated for a font.
 *
 * Params:
 *   eg_font* - a pointer to a font
 */
void eg_impl_destroy_font(eg_font *);

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
 *   eg_font* - a pointer to a font
 *   const char* - the text to render
 *   int - the x position of the text on the screen
 *   int - the y position of the text on the screen
 */
void eg_impl_draw_text(eg_app *, eg_font *, const char *, int, int);

/**
 * Renders a string of text within a rectangular boundary.
 * Newlines will be rendered to attempt to fit the text within the rectangle.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_font* - a pointer to a font
 *   const char* - the text to render
 *   eg_rect* - a pointer to a rectangular area of the screen to hold the text
 */
void eg_impl_draw_text_bounded(eg_app *, eg_font *, const char *, eg_rect *);

//----------------------------------------------------------------------------
// audio

/**
 * Reads audio data from a file.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   const char* - the path to the audio file
 *   int - the type of sound (1 for sound effect, 2 for music)
 *
 * Returns:
 *   int - 1 on success, or 0 on failure
 */
eg_sound *eg_impl_load_sound(eg_app *, const char *, int);

/**
 * Plays audio.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_sound* - a pointer to a sound
 */
void eg_impl_play_sound(eg_app *, eg_sound *);

/**
 * Frees the memory allocated for a sound.
 *
 * Params:
 *   eg_sound* - a pointer to a sound
 */
void eg_impl_destroy_sound(eg_sound *);

#endif