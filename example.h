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

// TEMP: debug IDs for identifying certain entities.
#define DEBUG_PLAYER 1
#define DEBUG_PLATFORM 2

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
 * Used for viewing and modifying the internal state of the application.
 * This is intended primarily for development and debugging.
 */
typedef struct eg_debug eg_debug;

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
 * Graphical data that can potentially be rendered to the screen.
 */
typedef struct eg_texture eg_texture;

/**
 * Font data for rendering text.
 */
typedef struct eg_font eg_font;

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
 * The result of collision detection.
 * This structure is defined by the consuming application.
 */
typedef struct eg_collision eg_collision;

typedef struct eg_camera
{
    int x;
    int y;
} eg_camera;

typedef struct eg_menu_item eg_menu_item;
typedef struct eg_menu eg_menu;
typedef struct eg_dialog eg_dialog;

/**
 * A function that can get all its required input from an app struct.
 *
 */
typedef void (*eg_func)(eg_app *);

/**
 * A callback function.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 */
typedef void (*eg_callback)(eg_app *);

/**
 * Performs a task using a specific entity.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_entity* - an entity that may be affected by the task.
 */
typedef void (*eg_entity_callback)(eg_app *, eg_entity *);

/**
 * A callback function for a menu or menu item.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_entity* - a menu
 */
typedef void (*eg_menu_callback)(eg_app *, eg_menu *);

/**
 * A callback function for a dialog.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_entity* - a dialog
 */
typedef void (*eg_dialog_callback)(eg_app *, eg_dialog *);

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
    eg_collision *,
    int);

struct eg_debug
{
    int overlay;
    int hitboxes;
    int collisions;
    int frame_len;
    int frame_by_frame;
};

// definition of the eg_app struct
struct eg_app
{
    eg_impl *impl;
    eg_debug debug;

    // This is an array of flags to indicate that a key press has already been
    // detected.
    unsigned char key_captures[EG_MAX_KEYCODE];

    // Counters to indicate how long an input has been actuated.
    unsigned char actuation_counters[EG_MAX_KEYCODE];

    // For different sized windows.
    int scale;

    // This flag is used as a sentinel value by the main loop.
    // As long as this value is 0, the main loop should continue to execute.
    // Once this flag is set to a non 0 value, any resources allocated by the
    // application should be freed and the application should terminate
    // gracefully.
    int done;

    // The pause flag prevents the normal update cycle, allowing for using
    // menus and dialogs.
    int pause;

    // screen dimensions
    int screen_width;
    int screen_height;

    eg_camera cam;

    // TEMP: camera boundaries for debugging
    // TODO: organize camera struct
    int cl;
    int cr;
    int ct;
    int cb;

    // The update function initiates the update cycle, wherein each object
    // that can be updated is advanced to the next stage
    eg_func update;
    eg_func draw;

    // textures
    eg_texture **textures;
    int texture_count;

    // fonts
    eg_font **fonts;
    int font_count;

    // menus
    eg_menu **menus;
    int menu_count;

    // dialogs
    eg_dialog **dialogs;
    int dialog_count;

    // input handlers
    eg_callback *input;
    int input_count;

    // counters
    int *counters;
    int counter_count;

    // A linked list of entities.
    // Entities are updated and rendered in the opposite order from which they
    // were added. The last entity added will be the first entity updated and
    // rendered.
    eg_entity *entities;

    // The entity registry is a collection of entity information common to all
    // entities of a given type.
    eg_entity_type *registry;

    // TEMP: a handle to the player entity.
    // TODO: implement searchable entities.
    eg_entity *player;
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
    int type;      // identifies the entity type in the registry
    int x_pos;     // horizontal position
    int y_pos;     // vertical position
    int x_vel;     // horizontal velocity
    int y_vel;     // vertical velocity
    uint8_t flags; // flags

    // Used for actions that take multiple iterations of the main loop.
    int ticks;

    // The carrier is an entity that is modifying another entity's position
    // and velocity.
    eg_entity *carrier;

    eg_entity *next;
    eg_entity *previous;

    // Experimental Features
    int x_acc; // horizontal acceleration
    int x_t;   // x velocity correction factor
    int y_acc; // vertical acceleration
    int y_t;   // y velocity correction factor
};

struct eg_entity_type
{
    int id;
    int width;
    int height;
    eg_entity_callback render;
    eg_entity_callback update;
    eg_collider collide;

    int (*get_x_vel)(eg_entity *); // calculates current x velocity.
    int (*get_y_vel)(eg_entity *); // calculates current y velocity.

    // entity interaction
    int interactable;
    int (*interact)(eg_app *, eg_entity *, eg_entity *);
};

struct eg_menu_item
{
    eg_point position;
    const char *text;
    eg_menu_callback callback;
};

struct eg_menu
{
    eg_point position;
    int w;
    int h;
    eg_point cursor;
    eg_menu_item **items;
    int item_count;
    eg_menu_callback render;
    eg_menu_callback update;

    // for passing values between menus and dialogs.
    int result;
};

struct eg_dialog
{
    eg_point position;
    int w;
    int h;
    int speed_scale;
    int panel;

    int ticks;
    int tick_limit;

    const char *text;
    int text_len;

    eg_dialog_callback render;
    eg_dialog_callback update;
    eg_dialog_callback advance;

    // for passing values between menus and dialogs.
    int result;
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

int *eg_reserve_counter(eg_app *);

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
// void eg_push_input_handler(eg_app *, eg_input_handler *);
void eg_push_input_handler(eg_app *, eg_callback);

/**
 * Pops an input handler off the top of the input handler stack.
 * This function returns a pointer to the input handler that was removed from
 * the stack. It does not free any memory. It is the repsonsibility of the
 * caller to dispose of any input handlers popped from the stack.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 */
void eg_pop_input_handler(eg_app *);

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

//----------------------------------------------------------------------------
// font functions

/**
 * Loads a font from a TrueType file.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   const char* - file path of the TrueType file
 *   int - point size
 *
 * Returns:
 *   int - 1 on success or 0 on failure
 */
// int eg_load_font(eg_app *, const char *, int);
eg_font *eg_load_font(eg_app *, const char *, int);

/**
 * Renders a string of text to the screen.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   const char* - the text to render
 *   int - the x position of the text on the screen
 *   int - the y position of the text on the screen
 */
void eg_draw_text(eg_app *, eg_font *, const char *, int, int);

/**
 * Renders a string of text to the screen within a specified area.
 * The fourth argument is an eg_rect structure describing the area that will
 * contain the text.
 * The x and y fields of the rect represent the starting position.
 * The w field of the rect represents the line width. This is the point at
 * which a line break will occur.
 * The h field of the rect represents the newline distance. This is the amount
 * of pixels to move down when writing a newline. If this value is 0, the
 * default heigh of the character 'A' in the font will be used.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   const char* - the text to render
 *   int - the x position of the text on the screen
 *   int - the y position of the text on the screen
 */
void eg_draw_text_bounded(eg_app *, eg_font *, const char *, eg_rect *);

//----------------------------------------------------------------------------
// texture functions

/**
 * Loads a texture from a PNG file.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   const char* - file path of the PNG file
 *
 * Returns:
 *   int - 1 on success or 0 on failure
 */
eg_texture *eg_load_texture(eg_app *, const char *);

/**
 * Renders a texure to the screen.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_texture* - the texure to render
 *   eg_rect* - a rectangular region of the texture that will be rendered
 *   eg_rect* - a rectangular region of the screen to which the texture
 *              will be rendered
 *   int - 1 for horizontal flip, 0 for no flip
 */
void eg_draw_texture(eg_app *, eg_texture *, eg_rect *, eg_rect *, int);

#endif