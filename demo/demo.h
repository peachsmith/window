#ifndef EG_DEMO_H
#define EG_DEMO_H

#include "example.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

// Implementation Detail:
// Get access to SDL's entry point.
#include <SDL2/SDL.h>

/**
 * Populates am app struct with the data needed for runtime.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *
 * Returns:
 *   int - 1 on success or 0 on failure
 */
int demo_prepare(eg_app *);

#endif