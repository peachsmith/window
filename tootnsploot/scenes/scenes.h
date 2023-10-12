#ifndef TNS_SCENES_H
#define TNS_SCENES_H

#include "crumbs.h"

// scenes
#define TNS_SCENE_TITLE_SCREEN 0
#define TNS_SCENE_FOREST 1

void clear_scene(cr_app *);

void load_title_screen(cr_app *);
void load_forest_scene(cr_app *);

#endif