#ifndef DEMO_SCENES_H
#define DEMO_SCENES_H

#include "crumbs.h"

#define DEMO_SCENE_0 0
#define DEMO_SCENE_1 1
#define DEMO_SCENE_2 2
#define DEMO_SCENE_3 3
#define DEMO_SCENE_4 4

void demo_clear_scene(cr_app *);

/**
 * Scnene 0 is a demonstration of inclined platforms and dialog between
 * various entities.
 */
void demo_load_scene_0(cr_app *);

/**
 * Scene 1 is a demonstration of collision with multiple blocks.
 */
void demo_load_scene_1(cr_app *);

/**
 * Scene 2 is a demonstration of collision with multiple blocks, much like
 * Scene 1, but with an additional block.
 */
void demo_load_scene_2(cr_app *);

/**
 * Scene 3 is a demonstration of hostile entity behavior.
 */
void demo_load_scene_3(cr_app *);

/**
 * Scene 4 is designed to test moving platforms.
 */
void demo_load_scene_4(cr_app *);

#endif