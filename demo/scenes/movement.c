#include "demo/scenes/scenes.h"
#include "demo/input/input.h"
#include "demo/entities/block.h"
#include "demo/entities/player.h"
#include "demo/entities/sign.h"
#include "demo/entities/jimbo.h"
#include "demo/entities/transition.h"
#include "demo/entities/pause_menu.h"
#include "demo/entities/fish_menu.h"
#include "demo/entities/info_menu.h"
#include "demo/entities/debug_menu.h"
#include "demo/entities/scene_menu.h"
#include "demo/entities/input_menu.h"
#include "demo/entities/info_dialog.h"
#include "demo/entities/demo_dialog.h"
#include "demo/entities/jimbo_dialog.h"
#include "demo/entities/sign_dialog.h"

#include <stdio.h>

static void block_row(eg_app *app, int x, int y, int length)
{
    int block_w = 18;

    for (int i = 0; i < length; i++)
    {
        block_demo_create(app, x + i * block_w, y);
    }
}

static void block_col(eg_app *app, int x, int y, int length)
{
    int block_h = 18;

    for (int i = 0; i < length; i++)
    {
        block_demo_create(app, x, y + i * block_h);
    }
}

void load_movement_scene(eg_app *app)
{

    app->cam.x = 0;
    app->cam.y = 0;

    // menus
    pause_menu_demo_create(app);
    fish_menu_demo_create(app);
    info_menu_demo_create(app);
    debug_menu_demo_create(app);
    scene_menu_demo_create(app);
    input_menu_demo_create(app);

    // dialogs
    demo_dialog_demo_create(app);
    info_dialog_demo_create(app);
    jimbo_dialog_demo_create(app);
    sign_dialog_demo_create(app);

    block_row(app, 0, 120, 15);

    transition_demo_create(app);

    player_demo_create(app, 100, 70);
}
