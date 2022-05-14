#include "demo/texture/texture.h"

#include <stdio.h>

// maximum number of textures
#define MAX_TEXTURES 10

// main texture list for the application
static eg_texture *textures[MAX_TEXTURES];

int demo_init_textures(eg_app *app)
{
    app->textures = &(textures[0]);
    app->texture_count = 0;

    if (eg_load_texture(app, "assets/images/ui.png") == NULL)
    {
        fprintf(stderr, "failed to load image\n");
        return 0;
    }

    return 1;
}
