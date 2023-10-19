#include "demo/demo.h"
#include "demo/input/input.h"

// This file contains the input handling code for the input actuation debugger
// menu. Most files in the input directory end with the suffix "_input". Since
// the internal name of the input actuation debugger in code is "input_menu",
// the name of this file is "input_menu_input", which sounds confusing and
// warrants this explanation.

void demo_input_menu_input(cr_app *app)
{
    if (cr_consume_input(app, CR_KEYCODE_X) ||
        cr_consume_input(app, CR_KEYCODE_Q) ||
        cr_consume_input(app, CR_KEYCODE_ESCAPE))
    {
        app->menu_count--;
        cr_pop_input_handler(app);
        return;
    }

    int max_count = 255;

    if (cr_peek_input(app, CR_KEYCODE_W))
    {
        if (app->actuation_counters[CR_KEYCODE_W] < max_count)
        {
            app->actuation_counters[CR_KEYCODE_W]++;
        }
    }
    else
    {
        app->actuation_counters[CR_KEYCODE_W] = 0;
    }

    if (cr_peek_input(app, CR_KEYCODE_A))
    {
        if (app->actuation_counters[CR_KEYCODE_A] < max_count)
        {
            app->actuation_counters[CR_KEYCODE_A]++;
        }
    }
    else
    {
        app->actuation_counters[CR_KEYCODE_A] = 0;
    }

    if (cr_peek_input(app, CR_KEYCODE_S))
    {
        if (app->actuation_counters[CR_KEYCODE_S] < max_count)
        {
            app->actuation_counters[CR_KEYCODE_S]++;
        }
    }
    else
    {
        app->actuation_counters[CR_KEYCODE_S] = 0;
    }

    if (cr_peek_input(app, CR_KEYCODE_D))
    {
        if (app->actuation_counters[CR_KEYCODE_D] < max_count)
        {
            app->actuation_counters[CR_KEYCODE_D]++;
        }
    }
    else
    {
        app->actuation_counters[CR_KEYCODE_D] = 0;
    }

    if (cr_peek_input(app, CR_KEYCODE_SPACE))
    {
        if (app->actuation_counters[CR_KEYCODE_SPACE] < max_count)
        {
            app->actuation_counters[CR_KEYCODE_SPACE]++;
        }
    }
    else
    {
        app->actuation_counters[CR_KEYCODE_SPACE] = 0;
    }
}