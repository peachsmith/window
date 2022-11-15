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

// Deprecated
// TODO: remove this once demo_common_dialog_renderer is fully implemented
void demo_render_dialog(eg_app *, eg_dialog *);

// new common dialog renderer
void demo_common_dialog_renderer(eg_app *, eg_entity *);

void demo_init_sign_dialog(eg_app *);
void demo_open_sign_dialog(eg_app *);

#endif