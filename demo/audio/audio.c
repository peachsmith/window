#include "demo/audio/audio.h"

#include <stdio.h>

// maximum number of sounds
#define MAX_SOUNDS 10

// main sound list for the application
static eg_sound *sounds[MAX_SOUNDS];

int demo_init_audio(eg_app *app)
{
    app->sounds = &(sounds[0]);
    app->sound_count = 0;

    if (eg_load_sound(app, "assets/audio/confirmation_002.ogg") == NULL)
    {
        fprintf(stderr, "failed to load confirmation_002.ogg\n");
        return 0;
    }

    if (eg_load_sound(app, "assets/audio/drop_003.ogg") == NULL)
    {
        fprintf(stderr, "failed to load drop_003.ogg\n");
        return 0;
    }

    if (eg_load_sound(app, "assets/audio/toggle_001.ogg") == NULL)
    {
        fprintf(stderr, "failed to load toggle_001.ogg\n");
        return 0;
    }

    return 1;
}
