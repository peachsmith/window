#include "demo/demo.h"
#include "demo/input/input.h"
#include "demo/scenes/scenes.h"
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

#include "common/util.h"

void demo_load_scene_4(cr_app *app)
{
    cr_entity **handles = app->extension->entity_handles;

    app->scene = DEMO_SCENE_4;

    util_set_camera(app, CR_CAMERA_ALL);

    app->cam.x = 0;
    app->cam.y = 0;

    // menus
    demo_create_pause_menu(app);
    demo_create_fish_menu(app);
    demo_create_info_menu(app);
    demo_create_debug_menu(app);
    demo_create_scene_menu(app);
    demo_create_input_menu(app);

    // dialogs
    demo_create_demo_dialog(app);
    demo_create_info_dialog(app);
    demo_create_jimbo_dialog(app);
    demo_create_sign_dialog(app);

    block_demo_row(app, 0, 120, 15);

    handles[DEMO_HANDLE_TRANSITION] = demo_create_transition(app);

    demo_create_moving_block(app, -60, 60, 0); // vertical before player
    demo_create_moving_block(app, 200, 75, 1); // horizontal before the player
    handles[DEMO_HANDLE_PLAYER] = demo_create_player(app, 80, 50);
    demo_create_moving_block(app, 100, 75, 1);  // horizontal after the player
    demo_create_moving_block(app, -140, 60, 0); // vertical after player
}
