#include "demo/demo.h"
#include "demo/input/input.h"
#include "demo/scenes/scenes.h"
#include "demo/entities/block.h"
#include "demo/entities/player.h"
#include "demo/entities/henry.h"
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

#include "common/util.h"

void demo_load_scene_3(cr_app *app)
{
    cr_entity **handles = app->extension->entity_handles;

    app->scene = DEMO_SCENE_3;

    // Starting position for building a sequence of blocks.
    int x_start = 50;

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

    demo_create_block(app, x_start - 27 + 18, 94);
    demo_create_block(app, x_start - 27 + 36, 94);
    demo_create_block(app, x_start - 27 + 54, 94);
    demo_create_block(app, x_start - 27 + 72, 94);
    demo_create_block(app, x_start - 27 + 90, 94);

    // player
    handles[DEMO_HANDLE_PLAYER] = demo_create_player(app, 100, 55);

    // Add the rest of the horizontal row.
    demo_create_block(app, x_start - 27 + 108, 94);
    demo_create_block(app, x_start - 27 + 126, 94);
    demo_create_block(app, x_start - 27 + 144, 94);
    demo_create_block(app, x_start - 27 + 162, 94);
    demo_create_block(app, x_start - 27 + 180, 94);
    demo_create_block(app, x_start - 27 + 198, 94);
    demo_create_block(app, x_start - 27 + 216, 94);
    demo_create_block(app, x_start - 27 + 234, 94);
    demo_create_block(app, x_start - 27 + 252, 94);

    // Vertical column 1.
    demo_create_block(app, x_start - 27 + 18, 76);
    demo_create_block(app, x_start - 27 + 18, 58);
    demo_create_block(app, x_start - 27 + 18, 40);
    demo_create_block(app, x_start - 27 + 18, 22);

    // Vertical column 2
    demo_create_block(app, x_start - 27 + 198, 76);
    demo_create_block(app, x_start - 27 + 198, 58);
    demo_create_block(app, x_start - 27 + 198, 40);
    demo_create_block(app, x_start - 27 + 198, 22);

    // A hostile entity.
    demo_create_henry(app, x_start - 27 + 150, 22);

    handles[DEMO_HANDLE_TRANSITION] = demo_create_transition(app);
}
