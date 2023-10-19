#include "demo/demo.h"
#include "demo/assets.h"
#include "demo/input/input.h"
#include "demo/entities/entity_types.h"
#include "demo/entities/player.h"
#include "demo/entities/jimbo.h"
#include "demo/entities/billy.h"
#include "demo/entities/henry.h"
#include "demo/entities/fireball.h"

#include "common/util.h"
#include "common/collision.h"

void demo_common_dialog_input(cr_app *app)
{
    // Locate the active dialog.
    cr_entity *dialog = app->dialogs[app->dialog_count - 1];
    if (dialog == NULL)
    {
        return;
    }

    if (cr_consume_input(app, CR_KEYCODE_Z))
    {
        if (dialog->ticks >= dialog->tick_limit)
        {
            app->entity_types[dialog->type].advance(app, dialog);
            return;
        }
    }

    if (cr_consume_input(app, CR_KEYCODE_X))
    {
        if (dialog->ticks < dialog->tick_limit)
        {
            dialog->ticks = dialog->tick_limit;
        }
        else
        {
            app->entity_types[dialog->type].advance(app, dialog);
        }
    }
}