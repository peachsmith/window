#include "tootnsploot/tootnsploot.h"
#include "tootnsploot/input/input.h"
#include "tootnsploot/entities/entity_types.h"

static cr_func input_handlers[20];

void tns_init_input(cr_app *app)
{
    app->input = &(input_handlers[0]);
}

void default_input_handler(cr_app *app)
{
}
