#include "demo/font/font.h"

#include <stdio.h>

// maximum number of fonts
#define MAX_FONTS 10

// main font list for the application
static eg_font *fonts[MAX_FONTS];

int demo_init_fonts(eg_app *app)
{
    app->fonts = &(fonts[0]);
    app->font_count = 0;

    // if (!eg_load_font(app, "assets/fonts/Alegreya-VariableFont_wght.ttf", 16))
    // {
    //     fprintf(stderr, "failed to load font\n");
    // }

    // if (!eg_load_font(app, "assets/fonts/JetBrainsMonoNL-Regular.ttf", 16))
    // {
    //     fprintf(stderr, "failed to load font\n");
    // }

    if (eg_load_font(app, "assets/fonts/Kenney Pixel.ttf", 16) == NULL)
    {
        fprintf(stderr, "failed to load font\n");
        return 0;
    }

    if (eg_load_font(app, "assets/fonts/pokemon_fire_red.ttf", 16) == NULL)
    {
        fprintf(stderr, "failed to load font\n");
        return 0;
    }

    return 1;
}
