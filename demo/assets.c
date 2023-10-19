#include "crumbs.h"
#include "demo/assets.h"

#include <stdio.h>
#include <stdlib.h>

int demo_load_all_assets(cr_app *app)
{
    // images
    if (cr_load_texture(app, "assets/images/ui.png") == NULL)
    {
        fprintf(stderr, "failed to load ui image\n");
        return 0;
    }
    if (cr_load_texture(app, "assets/images/characters.png") == NULL)
    {
        fprintf(stderr, "failed to load characters image\n");
        return 0;
    }
    if (cr_load_texture(app, "assets/images/scenery_3.png") == NULL)
    {
        fprintf(stderr, "failed to load scenery image\n");
        return 0;
    }

    // fonts
    if (cr_load_font(app, "assets/fonts/Kenney Pixel.ttf", 16) == NULL)
    {
        fprintf(stderr, "failed to load Kenney Pixel font\n");
        return 0;
    }
    if (cr_load_font(app, "assets/fonts/pokemon_fire_red.ttf", 16) == NULL)
    {
        fprintf(stderr, "failed to load pokemon_fire_red font\n");
        return 0;
    }
    if (cr_load_font(app, "assets/fonts/press-start.regular.ttf", 8) == NULL)
    {
        fprintf(stderr, "failed to load press-start.regular font\n");
        return 0;
    }

    // sounds
    if (cr_load_sound(app, "assets/audio/confirmation_002.ogg", AUDIO_TYPE_SOUND_EFFECT) == NULL)
    {
        fprintf(stderr, "failed to load confirmation_002.ogg\n");
        return 0;
    }
    if (cr_load_sound(app, "assets/audio/drop_003.ogg", AUDIO_TYPE_SOUND_EFFECT) == NULL)
    {
        fprintf(stderr, "failed to load drop_003.ogg\n");
        return 0;
    }
    if (cr_load_sound(app, "assets/audio/toggle_001.ogg", AUDIO_TYPE_SOUND_EFFECT) == NULL)
    {
        fprintf(stderr, "failed to load toggle_001.ogg\n");
        return 0;
    }
    if (cr_load_sound(app, "assets/audio/field_theme_1.wav", AUDIO_TYPE_MUSIC) == NULL)
    {
        fprintf(stderr, "failed to load field_theme_1.wav\n");
        return 0;
    }
    if (cr_load_sound(app, "assets/audio/track_4.wav", AUDIO_TYPE_MUSIC) == NULL)
    {
        fprintf(stderr, "failed to load track_4.wav\n");
        return 0;
    }

    return 1;
}
