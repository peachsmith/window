#include "common/texture.h"

#include <stdio.h>

// maximum number of textures
#define MAX_TEXTURES 10

// main texture list for the application
static eg_texture *textures[MAX_TEXTURES];

int common_init_textures(eg_app *app)
{
    app->textures = &(textures[0]);
    app->texture_count = 0;

    return 1;
}
