#ifndef EG_TNS_H
#define EG_TNS_H

#include "example.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

// Implementation Detail:
// Get access to SDL's entry point.
#include <SDL2/SDL.h>

// counters
// counters [3:10] represent available critter slots
#define TNS_COUNTER_BREATH 0
#define TNS_COUNTER_CRITTERS 1
#define TNS_COUNTER_SCORE 2
#define TNS_COUNTER_CRITTER_SLOT_OFFSET 3
#define TNS_COUNTER_MAX 20

/**
 * Populates am app struct with the data needed for runtime.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *
 * Returns:
 *   int - 1 on success or 0 on failure
 */
int tns_prepare(eg_app *);

#endif