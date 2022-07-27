#ifndef DEMO_DIALOG_H
#define DEMO_DIALOG_H

#include "example.h"

#define DEMO_MAX_DIALOGS 10
#define DEMO_DIALOG_BUFSIZE 256

void demo_init_dialogs(eg_app *);

void demo_render_dialog(eg_app *, eg_dialog *);

void demo_init_demo_dialog(eg_app *);
void demo_open_demo_dialog(eg_app *);

void demo_init_info_dialog(eg_app *);
void demo_open_info_dialog(eg_app *);

void demo_init_sign_dialog(eg_app *);
void demo_open_sign_dialog(eg_app *);

#endif