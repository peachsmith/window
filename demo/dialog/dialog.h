#ifndef DEMO_DIALOG_H
#define DEMO_DIALOG_H

#include "example.h"

#define DEMO_MAX_DIALOGS 10
#define DEMO_DIALOG_BUFSIZE 256

// common dialog text rendering speed
#define DEMO_DIALOG_SPEED_SCALE 4

// common dimensions of a dialog panel
#define DEMO_DIALOG_WIDTH 224
#define DEMO_DIALOG_HEIGHT 48

void demo_init_dialogs(eg_app *);

// new common dialog renderer
void demo_common_dialog_renderer(eg_app *, eg_entity *);

#endif