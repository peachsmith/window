#ifndef EG_DEMO_H
#define EG_DEMO_H

#include "example.h"

#define DEMO_COUNTER_BREATH 0
#define DEMO_COUNTER_CRITTERS 1
#define DEMO_COUNTER_SCORE 2
#define DEMO_COUNTER_CRITTER_SLOT_OFFSET 3
#define DEMO_COUNTER_MAX 20

//============================================================================
// Demo scenes
#define DEMO_SCENE_0 0
#define DEMO_SCENE_1 1
#define DEMO_SCENE_2 2
#define DEMO_SCENE_3 3
#define DEMO_SCENE_MOVEMENT 4

//============================================================================
// Toot n Sploot scenes
#define TNS_SCENE_TITLE_SCREEN 5
#define TNS_SCENE_FOREST 6

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