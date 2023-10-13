#ifndef DEMO_H
#define DEMO_H

#include "crumbs.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

/**
 * Populates am app struct with the data needed for runtime.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *
 * Returns:
 *   int - 1 on success or 0 on failure
 */
int demo_prepare(cr_app *);

#endif