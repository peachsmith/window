#include "common/audio.h"

#include <stdio.h>

// maximum number of sounds
#define MAX_SOUNDS 10

// main sound list for the application
static cr_sound *sounds[MAX_SOUNDS];

int common_init_audio(cr_app *app)
{
    app->sounds = &(sounds[0]);
    app->sound_count = 0;

    return 1;
}
