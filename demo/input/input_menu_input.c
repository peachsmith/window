#include "demo/demo.h"
#include "demo/input/input.h"
#include "common/menu.h"

// This file contains the input handling code for the input actuation debugger
// menu. Most files in the input directory end with the suffix "_input". Since
// the internal name of the input actuation debugger in code is "input_menu",
// the name of this file is "input_menu_input", which sounds confusing and
// warrants this explanation.

void input_menu_input_handler(eg_app *app)
{
    if (eg_consume_input(app, EG_KEYCODE_X) ||
        eg_consume_input(app, EG_KEYCODE_Q) ||
        eg_consume_input(app, EG_KEYCODE_ESCAPE))
    {
        app->menu_count--;
        eg_pop_input_handler(app);
        return;
    }

    int max_count = 255;

    if (eg_peek_input(app, EG_KEYCODE_W))
    {
        if (app->actuation_counters[EG_KEYCODE_W] < max_count)
        {
            app->actuation_counters[EG_KEYCODE_W]++;
        }
    }
    else
    {
        app->actuation_counters[EG_KEYCODE_W] = 0;
    }

    if (eg_peek_input(app, EG_KEYCODE_A))
    {
        if (app->actuation_counters[EG_KEYCODE_A] < max_count)
        {
            app->actuation_counters[EG_KEYCODE_A]++;
        }
    }
    else
    {
        app->actuation_counters[EG_KEYCODE_A] = 0;
    }

    if (eg_peek_input(app, EG_KEYCODE_S))
    {
        if (app->actuation_counters[EG_KEYCODE_S] < max_count)
        {
            app->actuation_counters[EG_KEYCODE_S]++;
        }
    }
    else
    {
        app->actuation_counters[EG_KEYCODE_S] = 0;
    }

    if (eg_peek_input(app, EG_KEYCODE_D))
    {
        if (app->actuation_counters[EG_KEYCODE_D] < max_count)
        {
            app->actuation_counters[EG_KEYCODE_D]++;
        }
    }
    else
    {
        app->actuation_counters[EG_KEYCODE_D] = 0;
    }

    if (eg_peek_input(app, EG_KEYCODE_SPACE))
    {
        if (app->actuation_counters[EG_KEYCODE_SPACE] < max_count)
        {
            app->actuation_counters[EG_KEYCODE_SPACE]++;
        }
    }
    else
    {
        app->actuation_counters[EG_KEYCODE_SPACE] = 0;
    }
}