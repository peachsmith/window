#ifndef DEMO_H
#define DEMO_H

#include "crumbs.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

// image assets
#define DEMO_TEXTURE_UI 0
#define DEMO_TEXTURE_CHARACTERS 1
#define DEMO_TEXTURE_SCENERY 2

// font assets
#define DEMO_FONT_KENNY_PIXEL 0
#define DEMO_FONT_POKEMON_FIRE_RED 1
#define DEMO_FONT_PRESS_START 2

// audio assets
#define DEMO_SOUND_EFFECT_CONFIRMATION 0
#define DEMO_SOUND_EFFECT_DROP 1
#define DEMO_SOUND_EFFECT_TOGGLE 2
#define DEMO_SONG_FIELD 3
#define DEMO_SONG_TRACK_4 4

// entity handles
#define DEMO_HANDLE_PLAYER 0
#define DEMO_HANDLE_TRANSITION 1
#define DEMO_MAX_ENTITY_HANDLES 10

// limits
#define DEMO_MAX_ENTITIES 256
#define DEMO_MAX_INPUT_HANDLERS 20
#define DEMO_MAX_MENUS 10
#define DEMO_MAX_DIALOGS 10
#define DEMO_MAX_OVERLAYS 10
#define DEMO_MAX_TEXTURES 10
#define DEMO_MAX_FONTS 10
#define DEMO_MAX_SOUNDS 10

// extension structure definition
struct cr_extension
{
    cr_entity **entity_handles;
};

/**
 * Populates am app struct with the data needed for runtime.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *
 * Returns:
 *   int - 1 on success or 0 on failure
 */
int demo_init_app(cr_app *);

#endif