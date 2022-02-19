#ifndef EG_PUBLIC_API_H
#define EG_PUBLIC_API_H

// This is the Example interface.
// Since this is an example, all functions an data types will be prefixed
// with eg (for exempli gratia).
// "Example" is a working title. A slightly more original name should be
// used at some point in the future.

#include "keyboard.h"
#include "colors.h"

#include <stdint.h>

// default screen dimensions based on the Game Boy Advance.
#define EG_DEFAULT_SCREEN_WIDTH 240
#define EG_DEFAULT_SCREEN_HEIGHT 160

/**
 * This structure represents the state of an application.
 * Only one of these should be created in a given program.
 *
 */
typedef struct eg_app eg_app;

/**
 * The underlying implementation of the framework.
 * This handles things like windows and input handling.
 *
 */
typedef struct eg_impl eg_impl;

/**
 * A node in a list of input handlers.
 */
typedef struct eg_input_handler eg_input_handler;

/**
 * An entity is a thing that can interact with other things.
 */
typedef struct eg_entity eg_entity;

/**
 * Common information for all entities of a given type.
 */
typedef struct eg_entity_type eg_entity_type;

/**
 * Result of rectangle overlap calculation.
 */
typedef struct eg_overlap
{
    // The fields dx0, dx1, dy0, and dy1 are referred to as the deltas.
    // They represent the differences between the positions of the sides
    // of two rectangles.
    // Given two rectangles A and B:
    // dx0 is difference between the right side of A and the left side of B.
    // dx1 is difference between the right side of B and the left side of A.
    // dy0 is difference between the bottom side of A and the top side of B.
    // dy1 is difference between the bottom side of B and the top side of A.
    int dx0;
    int dx1;
    int dy0;
    int dy1;
} eg_overlap;

/**
 * A point represents the x and y coordinates in 2D space.
 */
typedef struct eg_point
{
    int x;
    int y;
} eg_point;

/**
 * A rectangle in this context, is a quadrilateral whose sides are parallel
 * to the x and y axes.
 */
typedef struct eg_rect
{
    int x;
    int y;
    int w;
    int h;
} eg_rect;

/**
 * The result of testing for the intersection of a ray with a rectangle.
 */
typedef struct eg_ray_res
{
    eg_point cp; // contact point
    eg_point cn; // contact normal
    float t;     // t such that P(t) = CP
} eg_ray_res;

/**
 * The result of collision detection.
 */
typedef struct eg_col_res
{
    eg_ray_res col;
    eg_entity *target;
} eg_col_res;

typedef struct eg_camera
{
    int x;
    int y;
} eg_camera;

/**
 * A function that can get all its required input from an app struct.
 *
 */
typedef void (*eg_func)(eg_app *);

/**
 * Performs some task.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_entity* - an entity that may be affected by the task.
 */
typedef void (*eg_callback)(eg_app *, eg_entity *);

/**
 * The behavior of one entity when it collides with another.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_entity* - the entity whose state may be updated
 *   eg_entity* - the entity that affects the state of the first entity
 */
typedef void (*eg_collider)(
    eg_app *,
    eg_entity *,
    eg_entity *,
    eg_ray_res *,
    int);

// definition of the eg_app struct
struct eg_app
{
    eg_impl *impl;

    // This is an array of flags to indicate that a key press has already been
    // detected.
    unsigned char key_captures[EG_MAX_KEYCODE];

    // This flag is used as a sentinel value by the main loop.
    // As long as this value is 0, the main loop should continue to execute.
    // Once this flag is set to a non 0 value, any resources allocated by the
    // application should be freed and the application should terminate
    // gracefully.
    int done;

    // The input handler stack is a dynamic linked list of input handlers.
    // The input handler on the top of the stack is the only input handler
    // that can perform any action at any given time.
    eg_input_handler *input;

    // A linked list of entities.
    // Entities are updated and rendered in the opposite order from which they
    // were added. The last entity added will be the first entity updated and
    // rendered.
    eg_entity *entities;

    // The entity registry is a collection of entity information common to all
    // entities of a given type.
    eg_entity_type *registry;

    eg_camera cam;

    // TEMP: camera boundaries for debugging
    // TODO: organize camera struct
    int cl;
    int cr;
    int ct;
    int cb;
    int screen_width;
    int screen_height;

    eg_func update;
    eg_func draw;
};

// definition of the eg_input_handler struct
struct eg_input_handler
{
    // The callback performs some action in response to input events.
    eg_callback callback;

    // The previous handler will become the top when the node that references
    // it is popped from the stack.
    eg_input_handler *previous;

    // The entities that can be directly affected by input.
    eg_entity *target;
};

// definition of the eg_entity struct
struct eg_entity
{
    int id; // identifies the entity type in the registry

    int x_pos;     // horizontal position
    int y_pos;     // vertical position
    int x_vel;     // horizontal velocity
    int y_vel;     // vertical velocity
    uint8_t flags; // flags

    eg_entity *next;
    eg_entity *previous;
};

struct eg_entity_type
{
    int id;
    int width;
    int height;
    eg_callback render;
    eg_callback update;
    eg_collider collide;
};

//----------------------------------------------------------------------------
// core functions

/**
 * Initializes the framework.
 * This should be called once before allocating any resources or using any
 * other functions in this interface.
 *
 * Returns:
 *   int - 1 on success, or 0 on failure
 */
int eg_initialize();

/**
 * Terminates the framework.
 * This should be called after freeing all other resources that were allocated
 * during execution.
 */
void eg_terminate();

/**
 * Creates a new instance of the app struct.
 *
 * Returns:
 *   app* - a pointer to an app struct
 */
eg_app *eg_create_app();

/**
 * Frees the memory allocated for an app struct.
 *
 * Params:
 *   app* - a pointer to an app struct
 */
void eg_destroy_app(eg_app *);

/**
 * Initiates the current frame.
 * This should be called at the beginning of each iteration of the main loop.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 */
void eg_begin_frame(eg_app *);

/**
 * Concludes the current frame.
 * This should be called at the end of each iteration of the main loop.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 */
void eg_end_frame(eg_app *);

//----------------------------------------------------------------------------
// drawing functions

/**
 * Sets the current draw color.
 * The color is a 32-bit unsigned integer in the format 0xAARRGGBB. For
 * example, the color teal is represented as 0xFF387080.
 * After calling this function, all subsequent drawing functions will use the
 * color specified until this function is called again with a different color.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_color - a color
 */
void eg_set_color(eg_app *, eg_color);

/**
 * Draws a line connecting two points.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_point* - a pointer to the starting point of the line
 *   eg_point* - a pointer to the ending point of the line
 */
void eg_draw_line(eg_app *, eg_point *, eg_point *);

/**
 * Draws a rectangle.
 * The third argument is an integer that specifies whether or not to fill the
 * rectangle. If 1 is passed, then the rectangle is filled, if 0 is passed,
 * the rectangle is not filled.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_rect* - a pointer to a rectangle
 *   int - fill flag. 1 if the rectangle should be filled, otherwise 0
 */
void eg_draw_rect(eg_app *, eg_rect *, int);

//----------------------------------------------------------------------------
// input handling functions

/**
 * Checks to see if an input is actuated.
 * As long as the input is actuated, this function will return 1. This is used
 * for handling input events where some task must be performed for as long as
 * the input is actuated.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   int - the scancode of the key to check
 */
int eg_peek_input(eg_app *app, int);

/**
 * Checks to see if an input is actuated.
 * The first time the input is actuated, this function will return 1.
 * All subsequent calls to this function will return 0 until the state of
 * the input in question is reset. This is used for handling input events
 * where a task must be performed a single time for a single input actuation.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   int - the scancode of the key to check
 */
int eg_consume_input(eg_app *, int);

/**
 * Creates a new input handler.
 *
 * Params:
 *   eg_callback - a function that handles input
 *   eg_entity* - a reference to a target entity
 *
 * Returns:
 *   eg_input_handler* - a pointer to a new input handler
 */
eg_input_handler *eg_create_input_handler(eg_callback, eg_entity *);

/**
 * Frees the memory allocated for an input handler.
 *
 * Params:
 *   eg_input_handler* - a pointer to an input handler
 */
void eg_destroy_input_handler(eg_input_handler *);

/**
 * Pushes an input handler onto the top of the input handler stack.
 * Once pushed, the new input handler will be the current handler as long as
 * remains at the top.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_input_handler* - a pointer to an input handler.
 */
void eg_push_input_handler(eg_app *, eg_input_handler *);

/**
 * Pops an input handler off the top of the input handler stack.
 * This function returns a pointer to the input handler that was removed from
 * the stack. It does not free any memory. It is the repsonsibility of the
 * caller to dispose of any input handlers popped from the stack.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *
 * Returns:
 *   eg_input_handler* - the handler that was removed from the stack
 */
eg_input_handler *eg_pop_input_handler(eg_app *);

//----------------------------------------------------------------------------
// entity functions

/**
 * Allocates memory for an entity registry.
 *
 * Params:
 *   int - the number of entity types
 *
 * Returns:
 *   eg_entity_type* - a new entity registry
 */
eg_entity_type *eg_create_registry(int);

/**
 * Frees the memory allocated for an entity registry.
 *
 * Params:
 *   eg_entity_type* - the entity registry to be destroyed
 */
void eg_destroy_registry(eg_entity_type *);

/**
 * Creates a new entity.
 *
 * Returns:
 *   eg_entity* - a pointer to a new entity
 */
eg_entity *eg_create_entity();

/**
 * Frees the memory allocated for an entity.
 *
 * Params:
 *   eg_entity* - the entity to be destroyed
 */
void eg_destroy_entity(eg_entity *);

/**
 * Adds an entity to the entity list.
 * The last entity added is the first entity in the list.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_entity* - the entity to add
 */
void eg_add_entity(eg_app *, eg_entity *);

/**
 * Removes an entity from the entity list.
 * This function returns a pointer to the entity that was removed. It does not
 * free any memory allocated for the entity.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_entity* - the entity to remove
 *
 * Returns:
 *   eg_entity* - the entity that was removed
 */
eg_entity *eg_remove_entity(eg_app *, eg_entity *);

/**
 * Checks the value of an entity's flag.
 *
 * Params:
 *   eg_entity* - a reference to an entity whose flags will be checked
 *   int - the index of the flag to check, ranging from 0 to 7.
 *
 * Returns:
 *   int - 1 if the flag is set or 0 if the flag is not set.
 */
int eg_check_flag(eg_entity *e, int f);

/**
 * Sets a flag on an entity.
 *
 * Params:
 *   eg_entity* - a reference to an entity whose flag will be set
 *   int - the index of the flag to set, ranging from 0 to 7.
 */
void eg_set_flag(eg_entity *e, int f);

/**
 * Clears a flag on an entity.
 *
 * Params:
 *   eg_entity* - a reference to an entity whose flag will be cleared
 *   int - the index of the flag to clear, ranging from 0 to 7.
 */
void eg_clear_flag(eg_entity *e, int f);

/**
 * Toggles a flag on an entity.
 *
 * Params:
 *   eg_entity* - a reference to an entity whose flag will be toggled
 *   int - the index of the flag to toggle, ranging from 0 to 7.
 */
void eg_toggle_flag(eg_entity *e, int f);

#endif