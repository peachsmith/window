#include "common/font.h"

#include <stdio.h>

// maximum number of fonts
#define MAX_FONTS 10

// main font list for the application
static cr_font *fonts[MAX_FONTS];

int common_init_fonts(cr_app *app)
{
    app->fonts = &(fonts[0]);
    app->font_count = 0;

    if (cr_load_font(app, "assets/fonts/Kenney Pixel.ttf", 16) == NULL)
    {
        fprintf(stderr, "failed to load font\n");
        return 0;
    }

    if (cr_load_font(app, "assets/fonts/pokemon_fire_red.ttf", 16) == NULL)
    {
        fprintf(stderr, "failed to load font\n");
        return 0;
    }

    // source: https://www.1001fonts.com/press-start-font.html
    if (cr_load_font(app, "assets/fonts/press-start.regular.ttf", 8) == NULL)
    {
        fprintf(stderr, "failed to load font\n");
        return 0;
    }

    return 1;
}
