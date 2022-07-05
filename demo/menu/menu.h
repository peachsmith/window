#ifndef DEMO_MENU_H
#define DEMO_MENU_H

#include "example.h"

void demo_init_menus(eg_app *);

void demo_init_pause_menu(eg_app *);
void demo_open_pause_menu(eg_app *);

void demo_init_fish_menu(eg_app *);
void demo_open_fish_menu(eg_app *);

void demo_init_info_menu(eg_app *);
void demo_open_info_menu(eg_app *);

void demo_init_debug_menu(eg_app *);
void demo_open_debug_menu(eg_app *);

void demo_init_scenes_menu(eg_app *);
void demo_open_scenes_menu(eg_app *);

#endif