#ifndef EG_DEMO_SCENES_H
#define EG_DEMO_SCENES_H

#include "example.h"

#define DEMO_SCENE_0 0
#define DEMO_SCENE_1 1
#define DEMO_SCENE_2 2
#define DEMO_SCENE_3 3

void clear_scene(eg_app *);

/**
 * Scnene 0 is a demonstration of inclined platforms and dialog between
 * various entities.
 * 
 */
void load_scene_0(eg_app *);

/**
 * Scene 1 is a demonstration of collision with multiple blocks.
 * 
 */
void load_scene_1(eg_app *);

/**
 * Scene 2 is a demonstration of collision with multiple blocks, much like
 * Scene 1, but with an additional block/
 * 
 */
void load_scene_2(eg_app *);

/**
 * Scene 3 is a demonstration of hostile entity behavior.
 * 
 */
void load_scene_3(eg_app *);

void load_movement_scene(eg_app *);


//============================================================================
// Toot n Sploot scenes
void load_forest_scene(eg_app *);

#endif