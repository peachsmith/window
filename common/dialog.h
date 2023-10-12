#ifndef EG_COMMON_DIALOG_H
#define EG_COMMON_DIALOG_H

#include "example.h"

#define COMMON_MAX_DIALOGS 10
#define COMMON_DIALOG_BUFSIZE 256

// common dialog text rendering speed
#define COMMON_DIALOG_SPEED_SCALE 4

// common dimensions of a dialog panel
#define COMMON_DIALOG_WIDTH 224
#define COMMON_DIALOG_HEIGHT 48

void common_init_dialogs(eg_app *);

void common_dialog_renderer(
    eg_app *,
    eg_entity *,
    eg_font *,
    eg_entity_func,
    eg_entity_func);

#endif