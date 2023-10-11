#include "tootnsploot/tootnsploot.h"
#include "tootnsploot/input/input.h"
#include "tootnsploot/entities/entity_types.h"

static eg_callback input_handlers[20];

void tns_init_input(eg_app *app)
{
    app->input = &(input_handlers[0]);
}

void default_input_handler(eg_app *app)
{
}
