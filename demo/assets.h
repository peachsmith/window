#include "crumbs.h"

#ifndef DEMO_ASSETS_H
#define DEMO_ASSETS_H

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

int load_all_assets(cr_app *app);

#endif