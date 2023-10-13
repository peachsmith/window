#include "impl.h"

cr_sound *cr_impl_load_sound(cr_app *app, const char *path, int type)
{
    cr_sound *sound = NULL;

    if (type == AUDIO_TYPE_SOUND_EFFECT)
    {

        // Load the sound effect.
        Mix_Chunk *chunk = Mix_LoadWAV(path);
        if (chunk == NULL)
        {
            fprintf(stderr, "failed to open sound effect file\n");
            return NULL;
        }

        sound = (cr_sound *)malloc(sizeof(cr_sound));
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

        sound = (cr_sound *)malloc(sizeof(cr_sound));
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

void cr_impl_destroy_sound(cr_sound *sound)
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

void cr_impl_play_sound(cr_app *app, cr_sound *sound)
{
    if (sound->type == AUDIO_TYPE_SOUND_EFFECT)
    {
        Mix_PlayChannel(-1, sound->chunk, 0);
        return;
    }

    Mix_PlayMusic(sound->music, 0);
}
