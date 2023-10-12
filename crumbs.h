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
#define EG_DEFAULT_SCREEN_WIDTH 240
#define EG_DEFAULT_SCREEN_HEIGHT 160

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
typedef struct eg_app eg_app;

/**
 * The impl struct underlying implementation of the framework.
 * This handles things like windows and input handling.
 */
typedef struct eg_impl eg_impl;

/**
 * The debug struct is used for viewing and modifying the internal state of
 * the application. This is intended primarily for development and debugging.
 */
typedef struct eg_debug eg_debug;

/**
 * An entity is a thing that can interact with other things.
 */
typedef struct eg_entity eg_entity;

/**
 * An entity_type represents common information for all entities of a given
 * type.
 */
typedef struct eg_entity_type eg_entity_type;

/**
 * A texture represents some graphical data that may be rendered to the
 * screen.
 */
typedef struct eg_texture eg_texture;

/**
 * A font represents a specific style of typeface for text that may be
 * rendered to the screen.
 */
typedef struct eg_font eg_font;

/**
 * A sound represents audio data.
 */
typedef struct eg_sound eg_sound;

/**
 * A point represents x and y coordinates in 2D space.
 */
typedef struct eg_point eg_point;

/**
 * A rectangle represents a 2D area bound by four right angles.
 */
typedef struct eg_rect eg_rect;

/**
 * The camera represents a modification to the field of view.
 */
typedef struct eg_camera eg_camera;

/**
 * The collision struct represents the result of collision detection.
 * This structure is defined by the consuming application.
 */
typedef struct eg_collision eg_collision;

/**
 * A color represents a 32-bit value of the format 0xAARRGGBB.
 * For example, the color cornflower blue can be represented as 0xFF6495ED.
 */
typedef uint32_t eg_color;

/**
 * A keycode identifies a key on the keyboard.
 */
typedef enum eg_keycode eg_keycode;

/**
 * A func is a function that can get all its required input from an app
 * struct.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 */
typedef void (*eg_func)(eg_app *);

/**
 * An entity_func is a function that performs a task using a specific
 * entity.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_entity* - an entity that may be affected by the task.
 */
typedef void (*eg_entity_func)(eg_app *, eg_entity *);

/**
 * A collider is a function that provides the behavior of one entity when it
 * collides with another.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_entity* - the entity whose state may be updated
 *   eg_entity* - the entity that affects the state of the first entity
 *   eg_collision* - a
 */
typedef void (*eg_collider)(
    eg_app *,
    eg_entity *,
    eg_entity *,
    eg_collision *);

//----------------------------------------------------------------------------
// Keycodes

enum eg_keycode
{
    // dummy keycode
    EG_KEYCODE_UNKNOWN,

    // alphabetic keys
    EG_KEYCODE_A,
    EG_KEYCODE_B,
    EG_KEYCODE_C,
    EG_KEYCODE_D,
    EG_KEYCODE_E,
    EG_KEYCODE_F,
    EG_KEYCODE_G,
    EG_KEYCODE_H,
    EG_KEYCODE_I,
    EG_KEYCODE_J,
    EG_KEYCODE_K,
    EG_KEYCODE_L,
    EG_KEYCODE_M,
    EG_KEYCODE_N,
    EG_KEYCODE_O,
    EG_KEYCODE_P,
    EG_KEYCODE_Q,
    EG_KEYCODE_R,
    EG_KEYCODE_S,
    EG_KEYCODE_T,
    EG_KEYCODE_U,
    EG_KEYCODE_V,
    EG_KEYCODE_W,
    EG_KEYCODE_X,
    EG_KEYCODE_Y,
    EG_KEYCODE_Z,

    // numbers
    EG_KEYCODE_1,
    EG_KEYCODE_2,
    EG_KEYCODE_3,
    EG_KEYCODE_4,
    EG_KEYCODE_5,
    EG_KEYCODE_6,
    EG_KEYCODE_7,
    EG_KEYCODE_8,
    EG_KEYCODE_9,
    EG_KEYCODE_0,

    EG_KEYCODE_RETURN,
    EG_KEYCODE_ESCAPE,
    EG_KEYCODE_BACKSPACE,
    EG_KEYCODE_TAB,
    EG_KEYCODE_SPACE,

    // punctuation
    EG_KEYCODE_MINUS,
    EG_KEYCODE_EQUALS,
    EG_KEYCODE_LEFTBRACKET,
    EG_KEYCODE_RIGHTBRACKET,
    EG_KEYCODE_BACKSLASH,
    EG_KEYCODE_NONUSHASH, // unused, don't rely on it
    EG_KEYCODE_SEMICOLON,
    EG_KEYCODE_APOSTROPHE,
    EG_KEYCODE_GRAVE, // back tick or tilde key in top left
    EG_KEYCODE_COMMA,
    EG_KEYCODE_PERIOD,
    EG_KEYCODE_SLASH,
    EG_KEYCODE_CAPSLOCK,

    // function keys
    EG_KEYCODE_F1,
    EG_KEYCODE_F2,
    EG_KEYCODE_F3,
    EG_KEYCODE_F4,
    EG_KEYCODE_F5,
    EG_KEYCODE_F6,
    EG_KEYCODE_F7,
    EG_KEYCODE_F8,
    EG_KEYCODE_F9,
    EG_KEYCODE_F10,
    EG_KEYCODE_F11,
    EG_KEYCODE_F12,

    EG_KEYCODE_PRINTSCREEN,
    EG_KEYCODE_SCROLLLOCK,
    EG_KEYCODE_PAUSE,
    EG_KEYCODE_INSERT,
    EG_KEYCODE_HOME,
    EG_KEYCODE_PAGEUP,
    EG_KEYCODE_DELETE,
    EG_KEYCODE_END,
    EG_KEYCODE_PAGEDOWN,
    EG_KEYCODE_RIGHT,
    EG_KEYCODE_LEFT,
    EG_KEYCODE_DOWN,
    EG_KEYCODE_UP,

    EG_KEYCODE_NUMLOCKCLEAR,

    // key pad (a.k.a. 10 key) keys
    EG_KEYCODE_KP_DIVIDE,
    EG_KEYCODE_KP_MULTIPLY,
    EG_KEYCODE_KP_MINUS,
    EG_KEYCODE_KP_PLUS,
    EG_KEYCODE_KP_ENTER,
    EG_KEYCODE_KP_1,
    EG_KEYCODE_KP_2,
    EG_KEYCODE_KP_3,
    EG_KEYCODE_KP_4,
    EG_KEYCODE_KP_5,
    EG_KEYCODE_KP_6,
    EG_KEYCODE_KP_7,
    EG_KEYCODE_KP_8,
    EG_KEYCODE_KP_9,
    EG_KEYCODE_KP_0,
    EG_KEYCODE_KP_PERIOD,

    // Control, Shift, Alt, and super
    EG_KEYCODE_LCTRL,
    EG_KEYCODE_LSHIFT,
    EG_KEYCODE_LALT, // left alt or option
    EG_KEYCODE_LGUI, // super key (windows key, command, etc.)
    EG_KEYCODE_RCTRL,
    EG_KEYCODE_RSHIFT,
    EG_KEYCODE_RALT, // right alt or option
    EG_KEYCODE_RGUI, // right super key (windows key, command, etc.)

    // unknown keycodes
    EG_KEYCODE_NONUSBACKSLASH,
    EG_KEYCODE_APPLICATION,

    // Any values greater than or equal to this are considered to be invalid
    // keycodes.
    EG_MAX_KEYCODE
};

//----------------------------------------------------------------------------
// Structure Definitions

struct eg_point
{
    int x;
    int y;
};

struct eg_rect
{
    int x;
    int y;
    int w;
    int h;
};

struct eg_camera
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

struct eg_debug
{
    int overlay;
    int hitboxes;
    int collisions;
    int camera;
    int frame_len;
    int frame_by_frame;
    float fps;
};

// definition of the eg_app struct
struct eg_app
{
    eg_impl *impl;

    // This is an array of flags to indicate that a key press has already been
    // detected.
    unsigned char key_captures[EG_MAX_KEYCODE];

    // Counters to indicate how long an input has been actuated.
    unsigned char actuation_counters[EG_MAX_KEYCODE];

    // For different sized windows.
    int scale;

    // 0 for delay, 1 for delta time
    int time;

    // This flag is used as a sentinel value by the main loop.
    // As long as this value is 0, the main loop should continue to execute.
    // Once this flag is set to a non 0 value, any resources allocated by the
    // application should be freed and the application should terminate
    // gracefully.
    int done;

    // The pause flag prevents the normal update cycle, allowing for using
    // menus and dialogs.
    int pause;

    // common tick counter
    int ticks;

    int scene;

    // screen dimensions
    int screen_width;
    int screen_height;

    eg_camera cam;

    eg_func update;
    eg_func draw;

    // assets
    eg_texture **textures;
    int texture_count;

    eg_font **fonts;
    int font_count;

    eg_sound **sounds;
    int sound_count;

    // entities
    eg_entity *entities;
    int entity_count;
    int entity_cap;

    // entity type registry
    eg_entity_type *entity_types;

    // menus
    eg_entity **menus;
    int menu_count;

    // dialogs
    eg_entity **dialogs;
    int dialog_count;

    eg_entity **overlays;
    int overlay_count;

    // input handlers
    eg_func *input;
    int input_count;

    // scene transition
    eg_func transition_loader;
    eg_func transition_input_handler;

    eg_debug debug;

    float frame_check;

    // Generic values that can be used in any way.
    eg_entity *primary; // an entity of significance
    int *counters;      // generic counters
};

// definition of the eg_entity struct
struct eg_entity
{
    int present;

    int type; // identifies the entity type in the registry

    // position
    int x_pos;
    int y_pos;

    // velocity
    int x_vel;
    int y_vel;

    // acceleration
    int x_acc;
    int y_acc;

    // velocity correction factor
    int x_t;
    int y_t;

    // bit flags
    uint16_t flags;

    // custom data that may have special meaning in each entity
    uint8_t data;

    // animation counter
    int animation_ticks;

    // Used for actions that take multiple iterations of the main loop.
    int ticks;

    // Counter for invincibility frames.
    int iframes;

    // The carrier is an entity that is modifying another entity's position
    // and velocity.
    eg_entity *carrier;

    // A single string of text. (used for UI elements)
    const char *text;
    int text_len;

    // Tick limit (currently used for dialogs, but may be used elsewhere or moved)
    int tick_limit;

    // Result of a function call.
    // Used by dialogs and menus to get the result of a menu interaction.
    int result;

    // cursor position in a menu
    int cursor_x;
    int cursor_y;
    int scroll_x;
    int scroll_y;
};

struct eg_entity_type
{
    int id;
    int width;
    int height;
    eg_entity_func render;
    eg_entity_func update;
    eg_entity_func advance;
    eg_collider collide;

    int (*get_x_vel)(eg_entity *); // calculates current x velocity.
    int (*get_y_vel)(eg_entity *); // calculates current y velocity.

    // entity interaction
    int interactable;
    int (*interact)(eg_app *, eg_entity *, eg_entity *);

    // indicates whether this entity represents the user
    int control;

    // indicates that this entity shoudl override the spurious collision check.
    int spur;

    // indicates that an entity is a moving object that can carry another
    // entity.
    int move;

    // indicates that an entity is a sloped platform
    int slope;
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
CRUMBS_API int CRUMBS_CALL eg_initialize();

/**
 * Terminates the framework.
 * This should be called after freeing all other resources that were allocated
 * during execution.
 */
CRUMBS_API void CRUMBS_CALL eg_terminate();

/**
 * Creates a new instance of the app struct.
 *
 * Returns:
 *   app* - a pointer to an app struct
 */
CRUMBS_API eg_app *CRUMBS_CALL eg_create_app();

/**
 * Frees the memory allocated for an app struct.
 *
 * Params:
 *   app* - a pointer to an app struct
 */
CRUMBS_API void CRUMBS_CALL eg_destroy_app(eg_app *);

/**
 * Sets the title of the application. This string will be used by the
 * underlying platform to label the window.
 */
CRUMBS_API void CRUMBS_CALL eg_set_title(eg_app *, const char *);

/**
 * Initiates the current frame.
 * This should be called at the beginning of each iteration of the main loop.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 */
CRUMBS_API void CRUMBS_CALL eg_begin_frame(eg_app *);

/**
 * Concludes the current frame.
 * This should be called at the end of each iteration of the main loop.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 */
CRUMBS_API void CRUMBS_CALL eg_end_frame(eg_app *);

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
 *   eg_app* - a pointer to an app struct
 *   eg_color - a color
 */
CRUMBS_API void CRUMBS_CALL eg_set_color(eg_app *, eg_color);

/**
 * Draws a line connecting two points.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_point* - a pointer to the starting point of the line
 *   eg_point* - a pointer to the ending point of the line
 */
CRUMBS_API void CRUMBS_CALL eg_draw_line(eg_app *, eg_point *, eg_point *);

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
CRUMBS_API void CRUMBS_CALL eg_draw_rect(eg_app *, eg_rect *, int);

//----------------------------------------------------------------------------
// Input Handling Functions

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
CRUMBS_API int CRUMBS_CALL eg_peek_input(eg_app *, int);

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
CRUMBS_API int CRUMBS_CALL eg_consume_input(eg_app *, int);

/**
 * Pushes an input handler onto the top of the input handler stack.
 * Once pushed, the new input handler will be the current handler as long as
 * remains at the top.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_func - an input handliner function
 */
CRUMBS_API void CRUMBS_CALL eg_push_input_handler(eg_app *, eg_func);

/**
 * Pops an input handler off the top of the input handler stack.
 * This function returns a pointer to the input handler that was removed from
 * the stack. It does not free any memory. It is the repsonsibility of the
 * caller to dispose of any input handlers popped from the stack.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 */
CRUMBS_API void CRUMBS_CALL eg_pop_input_handler(eg_app *);

//----------------------------------------------------------------------------
// Entity Functions

/**
 * Creates a new entity.
 *
 * Returns:
 *   eg_entity* - a pointer to a new entity
 */
CRUMBS_API eg_entity *CRUMBS_CALL eg_create_entity(eg_app *);

/**
 * Removes an entity from the entity list.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_entity* - the entity to remove
 */
CRUMBS_API void CRUMBS_CALL eg_remove_entity(eg_app *, eg_entity *);

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
CRUMBS_API int CRUMBS_CALL eg_check_flag(eg_entity *, int);

/**
 * Sets a flag on an entity.
 *
 * Params:
 *   eg_entity* - a reference to an entity whose flag will be set
 *   int - the index of the flag to set, ranging from 0 to 7.
 */
CRUMBS_API void CRUMBS_CALL eg_set_flag(eg_entity *, int);

/**
 * Clears a flag on an entity.
 *
 * Params:
 *   eg_entity* - a reference to an entity whose flag will be cleared
 *   int - the index of the flag to clear, ranging from 0 to 7.
 */
CRUMBS_API void CRUMBS_CALL eg_clear_flag(eg_entity *, int);

/**
 * Toggles a flag on an entity.
 *
 * Params:
 *   eg_entity* - a reference to an entity whose flag will be toggled
 *   int - the index of the flag to toggle, ranging from 0 to 7.
 */
CRUMBS_API void CRUMBS_CALL eg_toggle_flag(eg_entity *, int);

//----------------------------------------------------------------------------
// Font Functions

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
CRUMBS_API eg_font *CRUMBS_CALL eg_load_font(eg_app *, const char *, int);

/**
 * Renders a string of text to the screen.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   const char* - the text to render
 *   int - the x position of the text on the screen
 *   int - the y position of the text on the screen
 */
CRUMBS_API void CRUMBS_CALL eg_draw_text(eg_app *, eg_font *, const char *, int, int);

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
CRUMBS_API void CRUMBS_CALL eg_draw_text_bounded(eg_app *, eg_font *, const char *, eg_rect *, int *);

//----------------------------------------------------------------------------
// Texture Functions

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
CRUMBS_API eg_texture *CRUMBS_CALL eg_load_texture(eg_app *, const char *);

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
CRUMBS_API void CRUMBS_CALL eg_draw_texture(eg_app *, eg_texture *, eg_rect *, eg_rect *, int);

//----------------------------------------------------------------------------
// Audio functions

/**
 * Loads a sound from a file.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   const char* - file path of the sound file
 */
CRUMBS_API eg_sound *CRUMBS_CALL eg_load_sound(eg_app *app, const char *, int);

/**
 * Plays the contents of a sound file.
 *
 * Params:
 *   eg_app* - a pointer to an app struct
 *   eg_sound* - a pointer to the sound to play
 */
CRUMBS_API void CRUMBS_CALL eg_play_sound(eg_app *, eg_sound *);

//----------------------------------------------------------------------------
// Reference Colors

// Palette Name: Vinik24
// Source: https://lospec.com/palette-list/vinik24
#define EG_COLOR_VINIK_BLACK 0xFF000000
#define EG_COLOR_VINIK_GRAY 0xFF6F6776
#define EG_COLOR_VINIK_LIGHT_GRAY 0xFF9A9A97
#define EG_COLOR_VINIK_GREEN_GRAY 0xFFC5CCB8
#define EG_COLOR_VINIK_MAUVE 0xFF8B5580
#define EG_COLOR_VINIK_PINK 0xFFC38890
#define EG_COLOR_VINIK_LAVENDAR 0xFFA593A5
#define EG_COLOR_VINIK_INDIGO 0xFF666092
#define EG_COLOR_VINIK_RED 0xFF9A4F50
#define EG_COLOR_VINIK_ORANGE 0xFFC28D75
#define EG_COLOR_VINIK_LIGHT_BLUE 0xFF7CA1C0
#define EG_COLOR_VINIK_BLUE 0xFF416AA3
#define EG_COLOR_VINIK_MAROON 0xFF8D6268
#define EG_COLOR_VINIK_YELLOW 0xFFBE955C
#define EG_COLOR_VINIK_CYAN 0xFF68ACA9
#define EG_COLOR_VINIK_TEAL 0xFF387080
#define EG_COLOR_VINIK_BROWN 0xFF6E6962
#define EG_COLOR_VINIK_CHARTEUSE 0xFF93A167
#define EG_COLOR_VINIK_SEA_GREEN 0xFF6EAA78
#define EG_COLOR_VINIK_FOREST_GREEN 0xFF557064
#define EG_COLOR_VINIK_MILITARY_GREEN 0xFF9D9F7F
#define EG_COLOR_VINIK_GREEMP 0xFF7E9E99
#define EG_COLOR_VINIK_GROOMP 0xFF5D6872
#define EG_COLOR_VINIK_PURPLE 0xFF433455

#ifdef __cplusplus
}
#endif

#endif