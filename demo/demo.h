#ifndef EG_DEMO_H
#define EG_DEMO_H

#include "example.h"

#define DEMO_COUNTER_NOTES 0
#define DEMO_COUNTER_CRITTERS 1
#define DEMO_COUNTER_SCORE 2

#define DEMO_SCENE_0 0
#define DEMO_SCENE_1 1
#define DEMO_SCENE_2 2
#define DEMO_SCENE_3 3
#define DEMO_SCENE_FOREST 4
#define DEMO_SCENE_MOVEMENT 5

/**
 * Populates am app struct with the data needed for runtime.
 * 
 * Params:
 *   eg_app* - a pointer to an app struct
 * 
 * Returns:
 *   int - 1 on success or 0 on failure
 */
int demo_prepare(eg_app*);

#endif