#ifndef CRUMBS_PUBLIC_API_H
#define CRUMBS_PUBLIC_API_H

//----------------------------------------------------------------------------
// Crumbs
// An exercise in API design.

#if  defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#ifdef CRUMBS_EXPORTS
#define CRUMBS_API __declspec(dllexport)
#else
#define CRUMBS_API __declspec(dllimport)
#endif
#define CRUMBS_CALL __cdecl
#else
#define CRUMBS_API
#define CRUMBS_CALL
#endif // _WIN32

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

//----------------------------------------------------------------------------

// default screen dimensions based on the Game Boy Advance.
#define CR_DEFAULT_SCREEN_WIDTH 240
#define CR_DEFAULT_SCREEN_HEIGHT 160

// types of audio resources
#define AUDIO_TYPE_SOUND_EFFECT 1
#define AUDIO_TYPE_MUSIC 2

#define TIMING_WAIT 1
#define TIMING_DELTA 2

//----------------------------------------------------------------------------
// Type Declarations

/**
 * The app struct represents the state of an application.
 * Only one of these should be created in a given program.
 */
typedef struct cr_app cr_app;

/**
 * The impl struct underlying implementation of the framework.
 * This handles things like windows and input handling.
 */
typedef struct cr_impl cr_impl;

/**
 * The debug struct is used for viewing and modifying the internal state of
 * the application. This is intended primarily for development and debugging.
 */
typedef struct cr_debug cr_debug;

/**
 * The extension structure is defined by the consuming application, and allows
 * it to add arbitrary contents to the app struct.
 */
typedef struct cr_extension cr_extension;

/**
 * An entity is a thing that can interact with other things.
 */
typedef struct cr_entity cr_entity;

/**
 * An entity_type represents common information for all entities of a given
 * type.
 */
typedef struct cr_entity_type cr_entity_type;

/**
 * A texture represents some graphical data that may be rendered to the
 * screen.
 */
typedef struct cr_texture cr_texture;

/**
 * A font represents a specific style of typeface for text that may be
 * rendered to the screen.
 */
typedef struct cr_font cr_font;

/**
 * A sound represents audio data.
 */
typedef struct cr_sound cr_sound;

/**
 * A point represents x and y coordinates in 2D space.
 */
typedef struct cr_point cr_point;

/**
 * A rectangle represents a 2D area bound by four right angles.
 */
typedef struct cr_rect cr_rect;

/**
 * The camera represents a modification to the field of view.
 */
typedef struct cr_camera cr_camera;

/**
 * The collision struct represents the result of collision detection.
 * This structure is defined by the consuming application.
 */
typedef struct cr_collision cr_collision;

/**
 * A color represents a 32-bit value of the format 0xAARRGGBB.
 * For example, the color cornflower blue can be represented as 0xFF6495ED.
 */
typedef uint32_t cr_color;

/**
 * A keycode identifies a key on the keyboard.
 */
typedef enum cr_keycode cr_keycode;

/**
 * A func is a function that can get all its required input from an app
 * struct.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 */
typedef void (*cr_func)(cr_app *);

/**
 * An entity_func is a function that performs a task using a specific
 * entity.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   cr_entity* - an entity that may be affected by the task.
 */
typedef void (*cr_entity_func)(cr_app *, cr_entity *);

/**
 * A collider is a function that provides the behavior of one entity when it
 * collides with another.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   cr_entity* - the entity whose state may be updated
 *   cr_entity* - the entity that affects the state of the first entity
 *   cr_collision* - a collision detection result
 */
typedef void (*cr_collider)(
    cr_app *,
    cr_entity *,
    cr_entity *,
    cr_collision *);

/**
 * An interaction is a function that provides the behavior of two entities
 * when they interact with each other.
 * 
 * Params:
 *   cr_app* - a pointer to an app struct
 *   cr_entity* - the entity responding to the interaction
 *   cr_entity* - the entity initiating the interaction
 */
typedef int (*cr_interaction)(
    cr_app *,
    cr_entity *,
    cr_entity *);

//----------------------------------------------------------------------------
// Structure Definitions

enum cr_keycode
{
    // dummy keycode
    CR_KEYCODE_UNKNOWN = 0,

    // alphabetic keys
    CR_KEYCODE_A,
    CR_KEYCODE_B,
    CR_KEYCODE_C,
    CR_KEYCODE_D,
    CR_KEYCODE_E,
    CR_KEYCODE_F,
    CR_KEYCODE_G,
    CR_KEYCODE_H,
    CR_KEYCODE_I,
    CR_KEYCODE_J,
    CR_KEYCODE_K,
    CR_KEYCODE_L,
    CR_KEYCODE_M,
    CR_KEYCODE_N,
    CR_KEYCODE_O,
    CR_KEYCODE_P,
    CR_KEYCODE_Q,
    CR_KEYCODE_R,
    CR_KEYCODE_S,
    CR_KEYCODE_T,
    CR_KEYCODE_U,
    CR_KEYCODE_V,
    CR_KEYCODE_W,
    CR_KEYCODE_X,
    CR_KEYCODE_Y,
    CR_KEYCODE_Z,

    // numbers
    CR_KEYCODE_1,
    CR_KEYCODE_2,
    CR_KEYCODE_3,
    CR_KEYCODE_4,
    CR_KEYCODE_5,
    CR_KEYCODE_6,
    CR_KEYCODE_7,
    CR_KEYCODE_8,
    CR_KEYCODE_9,
    CR_KEYCODE_0,

    CR_KEYCODE_RETURN,
    CR_KEYCODE_ESCAPE,
    CR_KEYCODE_BACKSPACE,
    CR_KEYCODE_TAB,
    CR_KEYCODE_SPACE,

    // punctuation
    CR_KEYCODE_MINUS,
    CR_KEYCODE_EQUALS,
    CR_KEYCODE_LEFTBRACKET,
    CR_KEYCODE_RIGHTBRACKET,
    CR_KEYCODE_BACKSLASH,
    CR_KEYCODE_NONUSHASH, // unused, don't rely on it
    CR_KEYCODE_SEMICOLON,
    CR_KEYCODE_APOSTROPHE,
    CR_KEYCODE_GRAVE, // back tick or tilde key in top left
    CR_KEYCODE_COMMA,
    CR_KEYCODE_PERIOD,
    CR_KEYCODE_SLASH,
    CR_KEYCODE_CAPSLOCK,

    // function keys
    CR_KEYCODE_F1,
    CR_KEYCODE_F2,
    CR_KEYCODE_F3,
    CR_KEYCODE_F4,
    CR_KEYCODE_F5,
    CR_KEYCODE_F6,
    CR_KEYCODE_F7,
    CR_KEYCODE_F8,
    CR_KEYCODE_F9,
    CR_KEYCODE_F10,
    CR_KEYCODE_F11,
    CR_KEYCODE_F12,

    CR_KEYCODE_PRINTSCREEN,
    CR_KEYCODE_SCROLLLOCK,
    CR_KEYCODE_PAUSE,
    CR_KEYCODE_INSERT,
    CR_KEYCODE_HOME,
    CR_KEYCODE_PAGEUP,
    CR_KEYCODE_DELETE,
    CR_KEYCODE_END,
    CR_KEYCODE_PAGEDOWN,
    CR_KEYCODE_RIGHT,
    CR_KEYCODE_LEFT,
    CR_KEYCODE_DOWN,
    CR_KEYCODE_UP,

    CR_KEYCODE_NUMLOCKCLEAR,

    // key pad (a.k.a. 10 key) keys
    CR_KEYCODE_KP_DIVIDE,
    CR_KEYCODE_KP_MULTIPLY,
    CR_KEYCODE_KP_MINUS,
    CR_KEYCODE_KP_PLUS,
    CR_KEYCODE_KP_ENTER,
    CR_KEYCODE_KP_1,
    CR_KEYCODE_KP_2,
    CR_KEYCODE_KP_3,
    CR_KEYCODE_KP_4,
    CR_KEYCODE_KP_5,
    CR_KEYCODE_KP_6,
    CR_KEYCODE_KP_7,
    CR_KEYCODE_KP_8,
    CR_KEYCODE_KP_9,
    CR_KEYCODE_KP_0,
    CR_KEYCODE_KP_PERIOD,

    // Control, Shift, Alt, and super
    CR_KEYCODE_LCTRL,
    CR_KEYCODE_LSHIFT,
    CR_KEYCODE_LALT, // left alt or option
    CR_KEYCODE_LGUI, // super key (windows key, command, etc.)
    CR_KEYCODE_RCTRL,
    CR_KEYCODE_RSHIFT,
    CR_KEYCODE_RALT, // right alt or option
    CR_KEYCODE_RGUI, // right super key (windows key, command, etc.)

    // unknown keycodes
    CR_KEYCODE_NONUSBACKSLASH,
    CR_KEYCODE_APPLICATION,

    // Any values greater than or equal to this are considered to be invalid
    // keycodes.
    CR_MAX_KEYCODE
};

struct cr_point
{
    int x;
    int y;
};

struct cr_rect
{
    int x;
    int y;
    int w;
    int h;
};

struct cr_camera
{
    // position
    int x;
    int y;

    // boundaries
    int cl;
    int cr;
    int ct;
    int cb;

    // boundary configuration
    int config;
};

struct cr_debug
{
    int overlay;
    int hitboxes;
    int collisions;
    int camera;
    int frame_len;
    int frame_by_frame;
    float fps;
};

// definition of the cr_app struct
struct cr_app
{
    cr_impl *impl;
    cr_extension* extension;

    cr_debug debug;

    unsigned char key_captures[CR_MAX_KEYCODE];
    unsigned char actuation_counters[CR_MAX_KEYCODE];

    int origin_x; // screen origin coordinates
    int origin_y;
    int scale; // screen size scaling
    int time;  // 0 for delay, 1 for delta time
    int done;  // 0 while the application is running
    int pause;
    int ticks;
    int scene;
    float frame_check; // regulates framerate
    int entity_cap;    // maximum number of entities
    int screen_width;
    int screen_height;
    cr_camera cam;
    cr_func update;
    cr_func draw;
    cr_func transition_loader;
    cr_func transition_input;
    cr_entity_type *entity_types;
    
    cr_entity *entities;
    cr_func *input;
    cr_entity **menus;
    cr_entity **dialogs;
    cr_entity **overlays;
    cr_texture **textures;
    cr_font **fonts;
    cr_sound **sounds;

    int entity_count;
    int input_count;
    int menu_count;
    int dialog_count;
    int overlay_count;
    int texture_count;
    int font_count;
    int sound_count;
};

// definition of the cr_entity struct
struct cr_entity
{
    int present;    // whether entity is considered alive
    int type;       // entity's location in the registry
    uint16_t flags; // entity state
    uint8_t data;   // generic data

    int x_pos;      // position
    int y_pos;

    int x_vel;      // velocity
    int y_vel;

    int x_acc;      // acceleration
    int y_acc;
    int x_t;        // velocity correction factor
    int y_t;

    int animation_ticks;
    int ticks;
    int tick_limit;

    int iframes;        // invincibility frame counter
    cr_entity *carrier; // a moving platform

    const char *text;
    int text_len;
    int result;   // return value of a callback
    int cursor_x; // cursor position in a menu
    int cursor_y;
    int scroll_x; // scroll position
    int scroll_y;
};

struct cr_entity_type
{
    int id;
    int width;
    int height;
    cr_entity_func render;
    cr_entity_func update;
    cr_entity_func advance;
    int (*get_x_vel)(cr_entity *); // calculates current x velocity.
    int (*get_y_vel)(cr_entity *); // calculates current y velocity.

    cr_collider collide;     // collision behavior
    cr_interaction interact; // interaction behavior

    int interactable; // entity can interact
    int control;      // entity represents the user
    int spur;         // entity override the spurious collision check
    int move;         // entity can carry another entity
    int slope;        // entity is a sloped platform
};

//----------------------------------------------------------------------------
// Core Functions

/**
 * Initializes the framework.
 * This should be called once before allocating any resources or using any
 * other functions in this interface.
 *
 * Returns:
 *   int - 1 on success, or 0 on failure
 */
CRUMBS_API int CRUMBS_CALL cr_initialize();

/**
 * Terminates the framework.
 * This should be called after freeing all other resources that were allocated
 * during execution.
 */
CRUMBS_API void CRUMBS_CALL cr_terminate();

/**
 * Creates a new instance of the app struct.
 *
 * Returns:
 *   app* - a pointer to an app struct
 */
CRUMBS_API cr_app *CRUMBS_CALL cr_create_app();

/**
 * Frees the memory allocated for an app struct.
 *
 * Params:
 *   app* - a pointer to an app struct
 */
CRUMBS_API void CRUMBS_CALL cr_destroy_app(cr_app *);

/**
 * Sets the title of the application. This string will be used by the
 * underlying platform to label the window.
 */
CRUMBS_API void CRUMBS_CALL cr_set_title(cr_app *, const char *);

/**
 * Initiates the current frame.
 * This should be called at the beginning of each iteration of the main loop.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 */
CRUMBS_API void CRUMBS_CALL cr_begin_frame(cr_app *);

/**
 * Concludes the current frame.
 * This should be called at the end of each iteration of the main loop.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 */
CRUMBS_API void CRUMBS_CALL cr_end_frame(cr_app *);

//----------------------------------------------------------------------------
// Drawing Functions

/**
 * Sets the current draw color.
 * The color is a 32-bit unsigned integer in the format 0xAARRGGBB. For
 * example, the color teal is represented as 0xFF387080.
 * After calling this function, all subsequent drawing functions will use the
 * color specified until this function is called again with a different color.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   cr_color - a color
 */
CRUMBS_API void CRUMBS_CALL cr_set_color(cr_app *, cr_color);

/**
 * Draws a line connecting two points.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   cr_point* - a pointer to the starting point of the line
 *   cr_point* - a pointer to the ending point of the line
 */
CRUMBS_API void CRUMBS_CALL cr_draw_line(cr_app *, cr_point *, cr_point *);

/**
 * Draws a rectangle.
 * The third argument is an integer that specifies whether or not to fill the
 * rectangle. If 1 is passed, then the rectangle is filled, if 0 is passed,
 * the rectangle is not filled.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   cr_rect* - a pointer to a rectangle
 *   int - fill flag. 1 if the rectangle should be filled, otherwise 0
 */
CRUMBS_API void CRUMBS_CALL cr_draw_rect(cr_app *, cr_rect *, int);

//----------------------------------------------------------------------------
// Input Handling Functions

/**
 * Checks to see if an input is actuated.
 * As long as the input is actuated, this function will return 1. This is used
 * for handling input events where some task must be performed for as long as
 * the input is actuated.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   int - the scancode of the key to check
 */
CRUMBS_API int CRUMBS_CALL cr_peek_input(cr_app *, int);

/**
 * Checks to see if an input is actuated.
 * The first time the input is actuated, this function will return 1.
 * All subsequent calls to this function will return 0 until the state of
 * the input in question is reset. This is used for handling input events
 * where a task must be performed a single time for a single input actuation.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   int - the scancode of the key to check
 */
CRUMBS_API int CRUMBS_CALL cr_consume_input(cr_app *, int);

/**
 * Pushes an input handler onto the top of the input handler stack.
 * Once pushed, the new input handler will be the current handler as long as
 * remains at the top.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   cr_func - an input handliner function
 */
CRUMBS_API void CRUMBS_CALL cr_push_input_handler(cr_app *, cr_func);

/**
 * Pops an input handler off the top of the input handler stack.
 * This function returns a pointer to the input handler that was removed from
 * the stack. It does not free any memory. It is the repsonsibility of the
 * caller to dispose of any input handlers popped from the stack.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 */
CRUMBS_API void CRUMBS_CALL cr_pop_input_handler(cr_app *);

//----------------------------------------------------------------------------
// Entity Functions

/**
 * Creates a new entity.
 *
 * Returns:
 *   cr_entity* - a pointer to a new entity
 */
CRUMBS_API cr_entity *CRUMBS_CALL cr_create_entity(cr_app *);

/**
 * Removes an entity from the entity list.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   cr_entity* - the entity to remove
 */
CRUMBS_API void CRUMBS_CALL cr_remove_entity(cr_app *, cr_entity *);

/**
 * Checks the value of an entity's flag.
 *
 * Params:
 *   cr_entity* - a reference to an entity whose flags will be checked
 *   int - the index of the flag to check, ranging from 0 to 7.
 *
 * Returns:
 *   int - 1 if the flag is set or 0 if the flag is not set.
 */
CRUMBS_API int CRUMBS_CALL cr_check_flag(cr_entity *, int);

/**
 * Sets a flag on an entity.
 *
 * Params:
 *   cr_entity* - a reference to an entity whose flag will be set
 *   int - the index of the flag to set, ranging from 0 to 7.
 */
CRUMBS_API void CRUMBS_CALL cr_set_flag(cr_entity *, int);

/**
 * Clears a flag on an entity.
 *
 * Params:
 *   cr_entity* - a reference to an entity whose flag will be cleared
 *   int - the index of the flag to clear, ranging from 0 to 7.
 */
CRUMBS_API void CRUMBS_CALL cr_clear_flag(cr_entity *, int);

/**
 * Toggles a flag on an entity.
 *
 * Params:
 *   cr_entity* - a reference to an entity whose flag will be toggled
 *   int - the index of the flag to toggle, ranging from 0 to 7.
 */
CRUMBS_API void CRUMBS_CALL cr_toggle_flag(cr_entity *, int);

//----------------------------------------------------------------------------
// Font Functions

/**
 * Loads a font from a TrueType file.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   const char* - file path of the TrueType file
 *   int - point size
 *
 * Returns:
 *   int - 1 on success or 0 on failure
 */
CRUMBS_API cr_font *CRUMBS_CALL cr_load_font(cr_app *, const char *, int);

/**
 * Renders a string of text to the screen.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   const char* - the text to render
 *   int - the x position of the text on the screen
 *   int - the y position of the text on the screen
 */
CRUMBS_API void CRUMBS_CALL cr_draw_text(cr_app *, cr_font *, const char *, int, int);

/**
 * Renders a string of text to the screen within a specified area.
 * The fourth argument is an cr_rect structure describing the area that will
 * contain the text.
 * The x and y fields of the rect represent the starting position.
 * The w field of the rect represents the line width. This is the point at
 * which a line break will occur.
 * The h field of the rect represents the newline distance. This is the amount
 * of pixels to move down when writing a newline. If this value is 0, the
 * default heigh of the character 'A' in the font will be used.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   const char* - the text to render
 *   int - the x position of the text on the screen
 *   int - the y position of the text on the screen
 */
CRUMBS_API void CRUMBS_CALL cr_draw_text_bounded(cr_app *, cr_font *, const char *, cr_rect *, int *);

//----------------------------------------------------------------------------
// Texture Functions

/**
 * Loads a texture from a PNG file.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   const char* - file path of the PNG file
 *
 * Returns:
 *   int - 1 on success or 0 on failure
 */
CRUMBS_API cr_texture *CRUMBS_CALL cr_load_texture(cr_app *, const char *);

/**
 * Renders a texure to the screen.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   cr_texture* - the texure to render
 *   cr_rect* - a rectangular region of the texture that will be rendered
 *   cr_rect* - a rectangular region of the screen to which the texture
 *              will be rendered
 *   int - 1 for horizontal flip, 0 for no flip
 */
CRUMBS_API void CRUMBS_CALL cr_draw_texture(cr_app *, cr_texture *, cr_rect *, cr_rect *, int);

//----------------------------------------------------------------------------
// Audio functions

/**
 * Loads a sound from a file.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   const char* - file path of the sound file
 */
CRUMBS_API cr_sound *CRUMBS_CALL cr_load_sound(cr_app *app, const char *, int);

/**
 * Plays the contents of a sound file.
 *
 * Params:
 *   cr_app* - a pointer to an app struct
 *   cr_sound* - a pointer to the sound to play
 */
CRUMBS_API void CRUMBS_CALL cr_play_sound(cr_app *, cr_sound *);

//----------------------------------------------------------------------------
// Reference Colors

// Palette Name: Vinik24
// Source: https://lospec.com/palette-list/vinik24
#define CR_COLOR_VINIK_BLACK 0xFF000000
#define CR_COLOR_VINIK_GRAY 0xFF6F6776
#define CR_COLOR_VINIK_LIGHT_GRAY 0xFF9A9A97
#define CR_COLOR_VINIK_GREEN_GRAY 0xFFC5CCB8
#define CR_COLOR_VINIK_MAUVE 0xFF8B5580
#define CR_COLOR_VINIK_PINK 0xFFC38890
#define CR_COLOR_VINIK_LAVENDAR 0xFFA593A5
#define CR_COLOR_VINIK_INDIGO 0xFF666092
#define CR_COLOR_VINIK_RED 0xFF9A4F50
#define CR_COLOR_VINIK_ORANGE 0xFFC28D75
#define CR_COLOR_VINIK_LIGHT_BLUE 0xFF7CA1C0
#define CR_COLOR_VINIK_BLUE 0xFF416AA3
#define CR_COLOR_VINIK_MAROON 0xFF8D6268
#define CR_COLOR_VINIK_YELLOW 0xFFBE955C
#define CR_COLOR_VINIK_CYAN 0xFF68ACA9
#define CR_COLOR_VINIK_TEAL 0xFF387080
#define CR_COLOR_VINIK_BROWN 0xFF6E6962
#define CR_COLOR_VINIK_CHARTEUSE 0xFF93A167
#define CR_COLOR_VINIK_SEA_GREEN 0xFF6EAA78
#define CR_COLOR_VINIK_FOREST_GREEN 0xFF557064
#define CR_COLOR_VINIK_MILITARY_GREEN 0xFF9D9F7F
#define CR_COLOR_VINIK_GREEMP 0xFF7E9E99
#define CR_COLOR_VINIK_GROOMP 0xFF5D6872
#define CR_COLOR_VINIK_PURPLE 0xFF433455

#ifdef __cplusplus
}
#endif

#endif