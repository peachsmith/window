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

void demo_load_scene_0(cr_app *app)
{
    cr_entity **handles = app->extension->entity_handles;

    app->scene = DEMO_SCENE_0;

    util_set_camera(app, CR_CAMERA_ALL);
    app->cam.x = -320;
    app->cam.y = 40;

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

    demo_create_block(app, 1, 94);
    demo_create_block(app, 19, 94);
    demo_create_block(app, 37, 94);
    demo_create_block(app, 55, 94);
    demo_create_block(app, 73, 94);

    // Vertically moving platform (added before player).
    demo_create_jimbo(app, 434, -60); // jimbo BEFORE vertically moving platform
    demo_create_moving_block(app, 433, -2, 0);
    demo_create_jimbo(app, 459, -60); // jimbo AFTER vertically moving platform

    // Horizontally moving platform (added before player).
    demo_create_jimbo(app, 635, -40); // jimbo BEFORE horizontally moving platform
    demo_create_moving_block(app, 640, -2, 1);
    demo_create_jimbo(app, 659, -40); // jimbo AFTER horizontally moving platform

    // A sign that contains information.
    demo_create_sign(app, 37, 76);

    // player
    handles[DEMO_HANDLE_PLAYER] = demo_create_player(app, 150, 0);

    // scene transition
    handles[DEMO_HANDLE_TRANSITION] = demo_create_transition(app);

    // Add the rest of the horizontal row.
    demo_create_block(app, 91, 94);
    demo_create_block(app, 109, 94);
    demo_create_block(app, 127, 94);
    demo_create_block(app, 145, 94);

    // Horizontally moving platform (added after player).
    demo_create_moving_block(app, 640, 62, 1);

    // Sloped platform inclined from left to right, followed by a flat
    // sloped platform (for dismounting) followed by a slope down, then up.
    //
    // It looks something like this
    //    _____
    //   /     \    /
    //  /       \  /
    // /         \/
    demo_create_sloped_block(app, 145, 76, 1); // ramp up from left to right
    demo_create_sloped_block(app, 217, 76, 2); // flat slope
    demo_create_sloped_block(app, 289, 76, 0); // ramp down from left to right
    demo_create_sloped_block(app, 361, 76, 1); // ramp up from left to right
    demo_create_block(app, 355, 93);           // solid ground to allow transition between sloped blocks

    // Long slope built from several sloped platforms.
    demo_create_sloped_block(app, -71, 94, 2);
    demo_create_sloped_block(app, -143, 76, 0);
    demo_create_sloped_block(app, -215, 58, 0);
    demo_create_block(app, -85, 94); // solid ground to allow transition between sloped blocks

    // Vertically moving platform (after player).
    demo_create_moving_block(app, 489, -2, 0);
}