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
#include "demo/util/util.h"
#include "demo/demo.h"

#include <stdio.h>

void load_movement_scene(eg_app *app)
{
    app->scene = DEMO_SCENE_MOVEMENT;
    
    demo_set_camera(app, EG_CAMERA_ALL);

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

    block_demo_row(app, 0, 120, 15);

    transition_demo_create(app);

    block_demo_create_moving(app, -60, 60, 0); // vertical before player
    block_demo_create_moving(app, 200, 75, 1); // horizontal before the player
    app->primary = player_demo_create(app, 80, 50);
    block_demo_create_moving(app, 100, 75, 1);  // horizontal after the player
    block_demo_create_moving(app, -140, 60, 0); // vertical after player
}
