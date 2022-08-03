#include "example.h"
#include "impl.h"

eg_sound *eg_impl_load_sound(eg_app *app, const char *path)
{
    eg_sound *sound = NULL;

    // Load the sound effect.
    Mix_Chunk *chunk = Mix_LoadWAV(path);
    if (chunk == NULL)
    {
        fprintf(stderr, "failed to open sound effect file\n");
        return NULL;
    }

    sound = (eg_sound *)malloc(sizeof(eg_sound));
    if (sound == NULL)
    {
        fprintf(stderr, "failed to create sound\n");
        Mix_FreeChunk(chunk);
        return NULL;
    }

    sound->chunk = chunk;
    sound->type = AUDIO_TYPE_SOUND_EFFECT;

    // Add the font to the application's font list.
    app->sounds[(app->sound_count)++] = sound;

    return sound;
}

void eg_impl_play_sound(eg_app *app, eg_sound *sound)
{
    Mix_PlayChannel(-1, sound->chunk, 0);
}
