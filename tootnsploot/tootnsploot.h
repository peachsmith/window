#ifndef TNS_H
#define TNS_H

#include "crumbs.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

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
 *   cr_app* - a pointer to an app struct
 *
 * Returns:
 *   int - 1 on success or 0 on failure
 */
int init_app(cr_app *);

#endif