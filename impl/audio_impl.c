#include "crumbs.h"
#include "impl.h"

eg_sound *eg_impl_load_sound(eg_app *app, const char *path, int type)
{
    eg_sound *sound = NULL;

    if (type == AUDIO_TYPE_SOUND_EFFECT)
    {

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
    }
    else if (type == AUDIO_TYPE_MUSIC)
    {

        // Load the sound effect.
        Mix_Music *music = Mix_LoadMUS(path);
        if (music == NULL)
        {
            fprintf(stderr, "failed to open sound effect file\n");
            return NULL;
        }

        sound = (eg_sound *)malloc(sizeof(eg_sound));
        if (sound == NULL)
        {
            fprintf(stderr, "failed to create sound\n");
            Mix_FreeMusic(music);
            return NULL;
        }

        sound->music = music;
        sound->type = AUDIO_TYPE_MUSIC;
    }

    // Add the font to the application's font list.
    app->sounds[(app->sound_count)++] = sound;

    return sound;
}

void eg_impl_destroy_sound(eg_sound *sound)
{
    if (sound->type == AUDIO_TYPE_SOUND_EFFECT)
    {
        Mix_FreeChunk(sound->chunk);
    }

    if (sound->type == AUDIO_TYPE_MUSIC)
    {
        Mix_FreeMusic(sound->music);
    }
}

void eg_impl_play_sound(eg_app *app, eg_sound *sound)
{
    if (sound->type == AUDIO_TYPE_SOUND_EFFECT)
    {
        Mix_PlayChannel(-1, sound->chunk, 0);
        return;
    }

    Mix_PlayMusic(sound->music, 0);
}
