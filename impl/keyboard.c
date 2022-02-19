#include "keyboard.h"

#include <SDL2/SDL.h>

// Scancode lookup table.
// This is used for converting EG keycodes into the underlying keyboard
// scancodes provided by SDL. The indices of the values in this table match
// the values of the corresponding EG keycode.
static const SDL_Scancode sdl_scancode_table[EG_MAX_KEYCODE + 1] = {
    SDL_SCANCODE_UNKNOWN,
    SDL_SCANCODE_A,
    SDL_SCANCODE_B,
    SDL_SCANCODE_C,
    SDL_SCANCODE_D,
    SDL_SCANCODE_E,
    SDL_SCANCODE_F,
    SDL_SCANCODE_G,
    SDL_SCANCODE_H,
    SDL_SCANCODE_I,
    SDL_SCANCODE_J,
    SDL_SCANCODE_K,
    SDL_SCANCODE_L,
    SDL_SCANCODE_M,
    SDL_SCANCODE_N,
    SDL_SCANCODE_O,
    SDL_SCANCODE_P,
    SDL_SCANCODE_Q,
    SDL_SCANCODE_R,
    SDL_SCANCODE_S,
    SDL_SCANCODE_T,
    SDL_SCANCODE_U,
    SDL_SCANCODE_V,
    SDL_SCANCODE_W,
    SDL_SCANCODE_X,
    SDL_SCANCODE_Y,
    SDL_SCANCODE_Z,
    SDL_SCANCODE_1,
    SDL_SCANCODE_2,
    SDL_SCANCODE_3,
    SDL_SCANCODE_4,
    SDL_SCANCODE_5,
    SDL_SCANCODE_6,
    SDL_SCANCODE_7,
    SDL_SCANCODE_8,
    SDL_SCANCODE_9,
    SDL_SCANCODE_0,
    SDL_SCANCODE_RETURN,
    SDL_SCANCODE_ESCAPE,
    SDL_SCANCODE_BACKSPACE,
    SDL_SCANCODE_TAB,
    SDL_SCANCODE_SPACE,
    SDL_SCANCODE_MINUS,
    SDL_SCANCODE_EQUALS,
    SDL_SCANCODE_LEFTBRACKET,
    SDL_SCANCODE_RIGHTBRACKET,
    SDL_SCANCODE_BACKSLASH,
    SDL_SCANCODE_NONUSHASH,
    SDL_SCANCODE_SEMICOLON,
    SDL_SCANCODE_APOSTROPHE,
    SDL_SCANCODE_GRAVE,
    SDL_SCANCODE_COMMA,
    SDL_SCANCODE_PERIOD,
    SDL_SCANCODE_SLASH,
    SDL_SCANCODE_CAPSLOCK,
    SDL_SCANCODE_F1,
    SDL_SCANCODE_F2,
    SDL_SCANCODE_F3,
    SDL_SCANCODE_F4,
    SDL_SCANCODE_F5,
    SDL_SCANCODE_F6,
    SDL_SCANCODE_F7,
    SDL_SCANCODE_F8,
    SDL_SCANCODE_F9,
    SDL_SCANCODE_F10,
    SDL_SCANCODE_F11,
    SDL_SCANCODE_F12,
    SDL_SCANCODE_PRINTSCREEN,
    SDL_SCANCODE_SCROLLLOCK,
    SDL_SCANCODE_PAUSE,
    SDL_SCANCODE_INSERT,
    SDL_SCANCODE_HOME,
    SDL_SCANCODE_PAGEUP,
    SDL_SCANCODE_DELETE,
    SDL_SCANCODE_END,
    SDL_SCANCODE_PAGEDOWN,
    SDL_SCANCODE_RIGHT,
    SDL_SCANCODE_LEFT,
    SDL_SCANCODE_DOWN,
    SDL_SCANCODE_UP,
    SDL_SCANCODE_NUMLOCKCLEAR,
    SDL_SCANCODE_KP_DIVIDE,
    SDL_SCANCODE_KP_MULTIPLY,
    SDL_SCANCODE_KP_MINUS,
    SDL_SCANCODE_KP_PLUS,
    SDL_SCANCODE_KP_ENTER,
    SDL_SCANCODE_KP_1,
    SDL_SCANCODE_KP_2,
    SDL_SCANCODE_KP_3,
    SDL_SCANCODE_KP_4,
    SDL_SCANCODE_KP_5,
    SDL_SCANCODE_KP_6,
    SDL_SCANCODE_KP_7,
    SDL_SCANCODE_KP_8,
    SDL_SCANCODE_KP_9,
    SDL_SCANCODE_KP_0,
    SDL_SCANCODE_KP_PERIOD,
    SDL_SCANCODE_LCTRL,
    SDL_SCANCODE_LSHIFT,
    SDL_SCANCODE_LALT,
    SDL_SCANCODE_LGUI,
    SDL_SCANCODE_RCTRL,
    SDL_SCANCODE_RSHIFT,
    SDL_SCANCODE_RALT,
    SDL_SCANCODE_RGUI,
    SDL_SCANCODE_NONUSBACKSLASH,
    SDL_SCANCODE_APPLICATION,
    SDL_NUM_SCANCODES};

// eg_keycode lookup table.
// This is used for converting SDL_Scancode values into eg_keycode values.
// It must be populated with meaningful values before it can be used.
static eg_keycode eg_keycode_table[SDL_NUM_SCANCODES + 1];

// Converts an eg_keyode into an SDL_Scancode.
SDL_Scancode eg_impl_get_sdl_scancode(eg_keycode k)
{
    if (k < EG_KEYCODE_UNKNOWN || k >= EG_MAX_KEYCODE)
    {
        return SDL_SCANCODE_UNKNOWN;
    }

    return sdl_scancode_table[k];
}

// Converts an SDL_Scancode into an eg_keyode.
eg_keycode eg_impl_get_eg_keycode(SDL_Scancode sc)
{
    if (sc < SDL_SCANCODE_UNKNOWN || sc >= SDL_NUM_SCANCODES)
    {
        return EG_KEYCODE_UNKNOWN;
    }

    return eg_keycode_table[sc];
}

// Populates the eg_keycode lookup table.
void eg_impl_init_keyboard()
{
    // Populate the eg_keycode lookup table.
    for (int i = 0; i < SDL_NUM_SCANCODES + 1; i++)
    {
        eg_keycode_table[i] = EG_KEYCODE_UNKNOWN;
    }

    eg_keycode_table[SDL_SCANCODE_UNKNOWN] = EG_KEYCODE_UNKNOWN;
    eg_keycode_table[SDL_SCANCODE_A] = EG_KEYCODE_A;
    eg_keycode_table[SDL_SCANCODE_B] = EG_KEYCODE_B;
    eg_keycode_table[SDL_SCANCODE_C] = EG_KEYCODE_C;
    eg_keycode_table[SDL_SCANCODE_D] = EG_KEYCODE_D;
    eg_keycode_table[SDL_SCANCODE_E] = EG_KEYCODE_E;
    eg_keycode_table[SDL_SCANCODE_F] = EG_KEYCODE_F;
    eg_keycode_table[SDL_SCANCODE_G] = EG_KEYCODE_G;
    eg_keycode_table[SDL_SCANCODE_H] = EG_KEYCODE_H;
    eg_keycode_table[SDL_SCANCODE_I] = EG_KEYCODE_I;
    eg_keycode_table[SDL_SCANCODE_J] = EG_KEYCODE_J;
    eg_keycode_table[SDL_SCANCODE_K] = EG_KEYCODE_K;
    eg_keycode_table[SDL_SCANCODE_L] = EG_KEYCODE_L;
    eg_keycode_table[SDL_SCANCODE_M] = EG_KEYCODE_M;
    eg_keycode_table[SDL_SCANCODE_N] = EG_KEYCODE_N;
    eg_keycode_table[SDL_SCANCODE_O] = EG_KEYCODE_O;
    eg_keycode_table[SDL_SCANCODE_P] = EG_KEYCODE_P;
    eg_keycode_table[SDL_SCANCODE_Q] = EG_KEYCODE_Q;
    eg_keycode_table[SDL_SCANCODE_R] = EG_KEYCODE_R;
    eg_keycode_table[SDL_SCANCODE_S] = EG_KEYCODE_S;
    eg_keycode_table[SDL_SCANCODE_T] = EG_KEYCODE_T;
    eg_keycode_table[SDL_SCANCODE_U] = EG_KEYCODE_U;
    eg_keycode_table[SDL_SCANCODE_V] = EG_KEYCODE_V;
    eg_keycode_table[SDL_SCANCODE_W] = EG_KEYCODE_W;
    eg_keycode_table[SDL_SCANCODE_X] = EG_KEYCODE_X;
    eg_keycode_table[SDL_SCANCODE_Y] = EG_KEYCODE_Y;
    eg_keycode_table[SDL_SCANCODE_Z] = EG_KEYCODE_Z;
    eg_keycode_table[SDL_SCANCODE_1] = EG_KEYCODE_1;
    eg_keycode_table[SDL_SCANCODE_2] = EG_KEYCODE_2;
    eg_keycode_table[SDL_SCANCODE_3] = EG_KEYCODE_3;
    eg_keycode_table[SDL_SCANCODE_4] = EG_KEYCODE_4;
    eg_keycode_table[SDL_SCANCODE_5] = EG_KEYCODE_5;
    eg_keycode_table[SDL_SCANCODE_6] = EG_KEYCODE_6;
    eg_keycode_table[SDL_SCANCODE_7] = EG_KEYCODE_7;
    eg_keycode_table[SDL_SCANCODE_8] = EG_KEYCODE_8;
    eg_keycode_table[SDL_SCANCODE_9] = EG_KEYCODE_9;
    eg_keycode_table[SDL_SCANCODE_0] = EG_KEYCODE_0;
    eg_keycode_table[SDL_SCANCODE_RETURN] = EG_KEYCODE_RETURN;
    eg_keycode_table[SDL_SCANCODE_ESCAPE] = EG_KEYCODE_ESCAPE;
    eg_keycode_table[SDL_SCANCODE_BACKSPACE] = EG_KEYCODE_BACKSPACE;
    eg_keycode_table[SDL_SCANCODE_TAB] = EG_KEYCODE_TAB;
    eg_keycode_table[SDL_SCANCODE_SPACE] = EG_KEYCODE_SPACE;
    eg_keycode_table[SDL_SCANCODE_MINUS] = EG_KEYCODE_MINUS;
    eg_keycode_table[SDL_SCANCODE_EQUALS] = EG_KEYCODE_EQUALS;
    eg_keycode_table[SDL_SCANCODE_LEFTBRACKET] = EG_KEYCODE_LEFTBRACKET;
    eg_keycode_table[SDL_SCANCODE_RIGHTBRACKET] = EG_KEYCODE_RIGHTBRACKET;
    eg_keycode_table[SDL_SCANCODE_BACKSLASH] = EG_KEYCODE_BACKSLASH;
    eg_keycode_table[SDL_SCANCODE_NONUSHASH] = EG_KEYCODE_NONUSHASH;
    eg_keycode_table[SDL_SCANCODE_SEMICOLON] = EG_KEYCODE_SEMICOLON;
    eg_keycode_table[SDL_SCANCODE_APOSTROPHE] = EG_KEYCODE_APOSTROPHE;
    eg_keycode_table[SDL_SCANCODE_GRAVE] = EG_KEYCODE_GRAVE;
    eg_keycode_table[SDL_SCANCODE_COMMA] = EG_KEYCODE_COMMA;
    eg_keycode_table[SDL_SCANCODE_PERIOD] = EG_KEYCODE_PERIOD;
    eg_keycode_table[SDL_SCANCODE_SLASH] = EG_KEYCODE_SLASH;
    eg_keycode_table[SDL_SCANCODE_CAPSLOCK] = EG_KEYCODE_CAPSLOCK;
    eg_keycode_table[SDL_SCANCODE_F1] = EG_KEYCODE_F1;
    eg_keycode_table[SDL_SCANCODE_F2] = EG_KEYCODE_F2;
    eg_keycode_table[SDL_SCANCODE_F3] = EG_KEYCODE_F3;
    eg_keycode_table[SDL_SCANCODE_F4] = EG_KEYCODE_F4;
    eg_keycode_table[SDL_SCANCODE_F5] = EG_KEYCODE_F5;
    eg_keycode_table[SDL_SCANCODE_F6] = EG_KEYCODE_F6;
    eg_keycode_table[SDL_SCANCODE_F7] = EG_KEYCODE_F7;
    eg_keycode_table[SDL_SCANCODE_F8] = EG_KEYCODE_F8;
    eg_keycode_table[SDL_SCANCODE_F9] = EG_KEYCODE_F9;
    eg_keycode_table[SDL_SCANCODE_F10] = EG_KEYCODE_F10;
    eg_keycode_table[SDL_SCANCODE_F11] = EG_KEYCODE_F11;
    eg_keycode_table[SDL_SCANCODE_F12] = EG_KEYCODE_F12;
    eg_keycode_table[SDL_SCANCODE_PRINTSCREEN] = EG_KEYCODE_PRINTSCREEN;
    eg_keycode_table[SDL_SCANCODE_SCROLLLOCK] = EG_KEYCODE_SCROLLLOCK;
    eg_keycode_table[SDL_SCANCODE_PAUSE] = EG_KEYCODE_PAUSE;
    eg_keycode_table[SDL_SCANCODE_INSERT] = EG_KEYCODE_INSERT;
    eg_keycode_table[SDL_SCANCODE_HOME] = EG_KEYCODE_HOME;
    eg_keycode_table[SDL_SCANCODE_PAGEUP] = EG_KEYCODE_PAGEUP;
    eg_keycode_table[SDL_SCANCODE_DELETE] = EG_KEYCODE_DELETE;
    eg_keycode_table[SDL_SCANCODE_END] = EG_KEYCODE_END;
    eg_keycode_table[SDL_SCANCODE_PAGEDOWN] = EG_KEYCODE_PAGEDOWN;
    eg_keycode_table[SDL_SCANCODE_RIGHT] = EG_KEYCODE_RIGHT;
    eg_keycode_table[SDL_SCANCODE_LEFT] = EG_KEYCODE_LEFT;
    eg_keycode_table[SDL_SCANCODE_DOWN] = EG_KEYCODE_DOWN;
    eg_keycode_table[SDL_SCANCODE_UP] = EG_KEYCODE_UP;
    eg_keycode_table[SDL_SCANCODE_NUMLOCKCLEAR] = EG_KEYCODE_NUMLOCKCLEAR;
    eg_keycode_table[SDL_SCANCODE_KP_DIVIDE] = EG_KEYCODE_KP_DIVIDE;
    eg_keycode_table[SDL_SCANCODE_KP_MULTIPLY] = EG_KEYCODE_KP_MULTIPLY;
    eg_keycode_table[SDL_SCANCODE_KP_MINUS] = EG_KEYCODE_KP_MINUS;
    eg_keycode_table[SDL_SCANCODE_KP_PLUS] = EG_KEYCODE_KP_PLUS;
    eg_keycode_table[SDL_SCANCODE_KP_ENTER] = EG_KEYCODE_KP_ENTER;
    eg_keycode_table[SDL_SCANCODE_KP_1] = EG_KEYCODE_KP_1;
    eg_keycode_table[SDL_SCANCODE_KP_2] = EG_KEYCODE_KP_2;
    eg_keycode_table[SDL_SCANCODE_KP_3] = EG_KEYCODE_KP_3;
    eg_keycode_table[SDL_SCANCODE_KP_4] = EG_KEYCODE_KP_4;
    eg_keycode_table[SDL_SCANCODE_KP_5] = EG_KEYCODE_KP_5;
    eg_keycode_table[SDL_SCANCODE_KP_6] = EG_KEYCODE_KP_6;
    eg_keycode_table[SDL_SCANCODE_KP_7] = EG_KEYCODE_KP_7;
    eg_keycode_table[SDL_SCANCODE_KP_8] = EG_KEYCODE_KP_8;
    eg_keycode_table[SDL_SCANCODE_KP_9] = EG_KEYCODE_KP_9;
    eg_keycode_table[SDL_SCANCODE_KP_0] = EG_KEYCODE_KP_0;
    eg_keycode_table[SDL_SCANCODE_KP_PERIOD] = EG_KEYCODE_KP_PERIOD;
    eg_keycode_table[SDL_SCANCODE_LCTRL] = EG_KEYCODE_LCTRL;
    eg_keycode_table[SDL_SCANCODE_LSHIFT] = EG_KEYCODE_LSHIFT;
    eg_keycode_table[SDL_SCANCODE_LALT] = EG_KEYCODE_LALT;
    eg_keycode_table[SDL_SCANCODE_LGUI] = EG_KEYCODE_LGUI;
    eg_keycode_table[SDL_SCANCODE_RCTRL] = EG_KEYCODE_RCTRL;
    eg_keycode_table[SDL_SCANCODE_RSHIFT] = EG_KEYCODE_RSHIFT;
    eg_keycode_table[SDL_SCANCODE_RALT] = EG_KEYCODE_RALT;
    eg_keycode_table[SDL_SCANCODE_RGUI] = EG_KEYCODE_RGUI;
    eg_keycode_table[SDL_SCANCODE_NONUSBACKSLASH] = EG_KEYCODE_NONUSBACKSLASH;
    eg_keycode_table[SDL_SCANCODE_APPLICATION] = EG_KEYCODE_APPLICATION;
}
