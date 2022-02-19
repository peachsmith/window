#ifndef EG_KEYBOARD_H
#define EG_KEYBOARD_H

// Keyboard key enumeration.
typedef enum eg_keycode
{
    // dummy keycode
    EG_KEYCODE_UNKNOWN = 0,

    // alphabetic keys
    EG_KEYCODE_A = 1,
    EG_KEYCODE_B = 2,
    EG_KEYCODE_C = 3,
    EG_KEYCODE_D = 4,
    EG_KEYCODE_E = 5,
    EG_KEYCODE_F = 6,
    EG_KEYCODE_G = 7,
    EG_KEYCODE_H = 8,
    EG_KEYCODE_I = 9,
    EG_KEYCODE_J = 10,
    EG_KEYCODE_K = 11,
    EG_KEYCODE_L = 12,
    EG_KEYCODE_M = 13,
    EG_KEYCODE_N = 14,
    EG_KEYCODE_O = 15,
    EG_KEYCODE_P = 16,
    EG_KEYCODE_Q = 17,
    EG_KEYCODE_R = 18,
    EG_KEYCODE_S = 19,
    EG_KEYCODE_T = 20,
    EG_KEYCODE_U = 21,
    EG_KEYCODE_V = 22,
    EG_KEYCODE_W = 23,
    EG_KEYCODE_X = 24,
    EG_KEYCODE_Y = 25,
    EG_KEYCODE_Z = 26,

    // numbers
    EG_KEYCODE_1 = 27,
    EG_KEYCODE_2 = 28,
    EG_KEYCODE_3 = 29,
    EG_KEYCODE_4 = 30,
    EG_KEYCODE_5 = 31,
    EG_KEYCODE_6 = 32,
    EG_KEYCODE_7 = 33,
    EG_KEYCODE_8 = 34,
    EG_KEYCODE_9 = 35,
    EG_KEYCODE_0 = 36,

    EG_KEYCODE_RETURN = 37,
    EG_KEYCODE_ESCAPE = 38,
    EG_KEYCODE_BACKSPACE = 39,
    EG_KEYCODE_TAB = 40,
    EG_KEYCODE_SPACE = 41,

    // punctuation
    EG_KEYCODE_MINUS = 42,
    EG_KEYCODE_EQUALS = 43,
    EG_KEYCODE_LEFTBRACKET = 44,
    EG_KEYCODE_RIGHTBRACKET = 45,
    EG_KEYCODE_BACKSLASH = 46,
    EG_KEYCODE_NONUSHASH = 47, // unused, don't rely on it
    EG_KEYCODE_SEMICOLON = 48,
    EG_KEYCODE_APOSTROPHE = 49,
    EG_KEYCODE_GRAVE = 50, // back tick or tilde key in top left
    EG_KEYCODE_COMMA = 51,
    EG_KEYCODE_PERIOD = 52,
    EG_KEYCODE_SLASH = 53,
    EG_KEYCODE_CAPSLOCK = 54,

    // function keys
    EG_KEYCODE_F1 = 55,
    EG_KEYCODE_F2 = 56,
    EG_KEYCODE_F3 = 57,
    EG_KEYCODE_F4 = 58,
    EG_KEYCODE_F5 = 59,
    EG_KEYCODE_F6 = 60,
    EG_KEYCODE_F7 = 61,
    EG_KEYCODE_F8 = 62,
    EG_KEYCODE_F9 = 63,
    EG_KEYCODE_F10 = 64,
    EG_KEYCODE_F11 = 65,
    EG_KEYCODE_F12 = 66,

    EG_KEYCODE_PRINTSCREEN = 67,
    EG_KEYCODE_SCROLLLOCK = 68,
    EG_KEYCODE_PAUSE = 69,
    EG_KEYCODE_INSERT = 70,
    EG_KEYCODE_HOME = 71,
    EG_KEYCODE_PAGEUP = 72,
    EG_KEYCODE_DELETE = 73,
    EG_KEYCODE_END = 74,
    EG_KEYCODE_PAGEDOWN = 75,
    EG_KEYCODE_RIGHT = 76,
    EG_KEYCODE_LEFT = 77,
    EG_KEYCODE_DOWN = 78,
    EG_KEYCODE_UP = 79,

    EG_KEYCODE_NUMLOCKCLEAR = 80,

    // key pad (a.k.a. 10 key) keys
    EG_KEYCODE_KP_DIVIDE = 81,
    EG_KEYCODE_KP_MULTIPLY = 82,
    EG_KEYCODE_KP_MINUS = 83,
    EG_KEYCODE_KP_PLUS = 84,
    EG_KEYCODE_KP_ENTER = 85,
    EG_KEYCODE_KP_1 = 86,
    EG_KEYCODE_KP_2 = 87,
    EG_KEYCODE_KP_3 = 88,
    EG_KEYCODE_KP_4 = 89,
    EG_KEYCODE_KP_5 = 90,
    EG_KEYCODE_KP_6 = 91,
    EG_KEYCODE_KP_7 = 92,
    EG_KEYCODE_KP_8 = 93,
    EG_KEYCODE_KP_9 = 94,
    EG_KEYCODE_KP_0 = 95,
    EG_KEYCODE_KP_PERIOD = 96,

    // Control, Shift, Alt, and super
    EG_KEYCODE_LCTRL = 97,
    EG_KEYCODE_LSHIFT = 98,
    EG_KEYCODE_LALT = 99,  // left alt or option
    EG_KEYCODE_LGUI = 100, // super key (windows key, command, etc.)
    EG_KEYCODE_RCTRL = 101,
    EG_KEYCODE_RSHIFT = 102,
    EG_KEYCODE_RALT = 103, // right alt or option
    EG_KEYCODE_RGUI = 104, // right super key (windows key, command, etc.)

    // unknown keycodes
    EG_KEYCODE_NONUSBACKSLASH = 105,
    EG_KEYCODE_APPLICATION = 106,

    // maximum enum value
    // This is not represent a key, but any values greater than this are
    // not valid keycodes.
    EG_MAX_KEYCODE = 107
} eg_keycode;

#endif