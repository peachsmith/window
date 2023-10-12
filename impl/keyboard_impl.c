#include "crumbs.h"

#include <SDL2/SDL.h>

// Scancode lookup table.
// This is used for converting EG keycodes into the underlying keyboard
// scancodes provided by SDL. The indices of the values in this table match
// the values of the corresponding EG keycode.
static const SDL_Scancode sdl_scancode_table[CR_MAX_KEYCODE + 1] = {
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

// cr_keycode lookup table.
// This is used for converting SDL_Scancode values into cr_keycode values.
// It must be populated with meaningful values before it can be used.
static cr_keycode cr_keycode_table[SDL_NUM_SCANCODES + 1];

// Converts an cr_keyode into an SDL_Scancode.
SDL_Scancode cr_impl_get_sdl_scancode(cr_keycode k)
{
    if (k < CR_KEYCODE_UNKNOWN || k >= CR_MAX_KEYCODE)
    {
        return SDL_SCANCODE_UNKNOWN;
    }

    return sdl_scancode_table[k];
}

// Converts an SDL_Scancode into an cr_keyode.
cr_keycode cr_impl_get_cr_keycode(SDL_Scancode sc)
{
    if (sc < SDL_SCANCODE_UNKNOWN || sc >= SDL_NUM_SCANCODES)
    {
        return CR_KEYCODE_UNKNOWN;
    }

    return cr_keycode_table[sc];
}

// Populates the cr_keycode lookup table.
void cr_impl_init_keyboard()
{
    // Populate the cr_keycode lookup table.
    for (int i = 0; i < SDL_NUM_SCANCODES + 1; i++)
    {
        cr_keycode_table[i] = CR_KEYCODE_UNKNOWN;
    }

    cr_keycode_table[SDL_SCANCODE_UNKNOWN] = CR_KEYCODE_UNKNOWN;
    cr_keycode_table[SDL_SCANCODE_A] = CR_KEYCODE_A;
    cr_keycode_table[SDL_SCANCODE_B] = CR_KEYCODE_B;
    cr_keycode_table[SDL_SCANCODE_C] = CR_KEYCODE_C;
    cr_keycode_table[SDL_SCANCODE_D] = CR_KEYCODE_D;
    cr_keycode_table[SDL_SCANCODE_E] = CR_KEYCODE_E;
    cr_keycode_table[SDL_SCANCODE_F] = CR_KEYCODE_F;
    cr_keycode_table[SDL_SCANCODE_G] = CR_KEYCODE_G;
    cr_keycode_table[SDL_SCANCODE_H] = CR_KEYCODE_H;
    cr_keycode_table[SDL_SCANCODE_I] = CR_KEYCODE_I;
    cr_keycode_table[SDL_SCANCODE_J] = CR_KEYCODE_J;
    cr_keycode_table[SDL_SCANCODE_K] = CR_KEYCODE_K;
    cr_keycode_table[SDL_SCANCODE_L] = CR_KEYCODE_L;
    cr_keycode_table[SDL_SCANCODE_M] = CR_KEYCODE_M;
    cr_keycode_table[SDL_SCANCODE_N] = CR_KEYCODE_N;
    cr_keycode_table[SDL_SCANCODE_O] = CR_KEYCODE_O;
    cr_keycode_table[SDL_SCANCODE_P] = CR_KEYCODE_P;
    cr_keycode_table[SDL_SCANCODE_Q] = CR_KEYCODE_Q;
    cr_keycode_table[SDL_SCANCODE_R] = CR_KEYCODE_R;
    cr_keycode_table[SDL_SCANCODE_S] = CR_KEYCODE_S;
    cr_keycode_table[SDL_SCANCODE_T] = CR_KEYCODE_T;
    cr_keycode_table[SDL_SCANCODE_U] = CR_KEYCODE_U;
    cr_keycode_table[SDL_SCANCODE_V] = CR_KEYCODE_V;
    cr_keycode_table[SDL_SCANCODE_W] = CR_KEYCODE_W;
    cr_keycode_table[SDL_SCANCODE_X] = CR_KEYCODE_X;
    cr_keycode_table[SDL_SCANCODE_Y] = CR_KEYCODE_Y;
    cr_keycode_table[SDL_SCANCODE_Z] = CR_KEYCODE_Z;
    cr_keycode_table[SDL_SCANCODE_1] = CR_KEYCODE_1;
    cr_keycode_table[SDL_SCANCODE_2] = CR_KEYCODE_2;
    cr_keycode_table[SDL_SCANCODE_3] = CR_KEYCODE_3;
    cr_keycode_table[SDL_SCANCODE_4] = CR_KEYCODE_4;
    cr_keycode_table[SDL_SCANCODE_5] = CR_KEYCODE_5;
    cr_keycode_table[SDL_SCANCODE_6] = CR_KEYCODE_6;
    cr_keycode_table[SDL_SCANCODE_7] = CR_KEYCODE_7;
    cr_keycode_table[SDL_SCANCODE_8] = CR_KEYCODE_8;
    cr_keycode_table[SDL_SCANCODE_9] = CR_KEYCODE_9;
    cr_keycode_table[SDL_SCANCODE_0] = CR_KEYCODE_0;
    cr_keycode_table[SDL_SCANCODE_RETURN] = CR_KEYCODE_RETURN;
    cr_keycode_table[SDL_SCANCODE_ESCAPE] = CR_KEYCODE_ESCAPE;
    cr_keycode_table[SDL_SCANCODE_BACKSPACE] = CR_KEYCODE_BACKSPACE;
    cr_keycode_table[SDL_SCANCODE_TAB] = CR_KEYCODE_TAB;
    cr_keycode_table[SDL_SCANCODE_SPACE] = CR_KEYCODE_SPACE;
    cr_keycode_table[SDL_SCANCODE_MINUS] = CR_KEYCODE_MINUS;
    cr_keycode_table[SDL_SCANCODE_EQUALS] = CR_KEYCODE_EQUALS;
    cr_keycode_table[SDL_SCANCODE_LEFTBRACKET] = CR_KEYCODE_LEFTBRACKET;
    cr_keycode_table[SDL_SCANCODE_RIGHTBRACKET] = CR_KEYCODE_RIGHTBRACKET;
    cr_keycode_table[SDL_SCANCODE_BACKSLASH] = CR_KEYCODE_BACKSLASH;
    cr_keycode_table[SDL_SCANCODE_NONUSHASH] = CR_KEYCODE_NONUSHASH;
    cr_keycode_table[SDL_SCANCODE_SEMICOLON] = CR_KEYCODE_SEMICOLON;
    cr_keycode_table[SDL_SCANCODE_APOSTROPHE] = CR_KEYCODE_APOSTROPHE;
    cr_keycode_table[SDL_SCANCODE_GRAVE] = CR_KEYCODE_GRAVE;
    cr_keycode_table[SDL_SCANCODE_COMMA] = CR_KEYCODE_COMMA;
    cr_keycode_table[SDL_SCANCODE_PERIOD] = CR_KEYCODE_PERIOD;
    cr_keycode_table[SDL_SCANCODE_SLASH] = CR_KEYCODE_SLASH;
    cr_keycode_table[SDL_SCANCODE_CAPSLOCK] = CR_KEYCODE_CAPSLOCK;
    cr_keycode_table[SDL_SCANCODE_F1] = CR_KEYCODE_F1;
    cr_keycode_table[SDL_SCANCODE_F2] = CR_KEYCODE_F2;
    cr_keycode_table[SDL_SCANCODE_F3] = CR_KEYCODE_F3;
    cr_keycode_table[SDL_SCANCODE_F4] = CR_KEYCODE_F4;
    cr_keycode_table[SDL_SCANCODE_F5] = CR_KEYCODE_F5;
    cr_keycode_table[SDL_SCANCODE_F6] = CR_KEYCODE_F6;
    cr_keycode_table[SDL_SCANCODE_F7] = CR_KEYCODE_F7;
    cr_keycode_table[SDL_SCANCODE_F8] = CR_KEYCODE_F8;
    cr_keycode_table[SDL_SCANCODE_F9] = CR_KEYCODE_F9;
    cr_keycode_table[SDL_SCANCODE_F10] = CR_KEYCODE_F10;
    cr_keycode_table[SDL_SCANCODE_F11] = CR_KEYCODE_F11;
    cr_keycode_table[SDL_SCANCODE_F12] = CR_KEYCODE_F12;
    cr_keycode_table[SDL_SCANCODE_PRINTSCREEN] = CR_KEYCODE_PRINTSCREEN;
    cr_keycode_table[SDL_SCANCODE_SCROLLLOCK] = CR_KEYCODE_SCROLLLOCK;
    cr_keycode_table[SDL_SCANCODE_PAUSE] = CR_KEYCODE_PAUSE;
    cr_keycode_table[SDL_SCANCODE_INSERT] = CR_KEYCODE_INSERT;
    cr_keycode_table[SDL_SCANCODE_HOME] = CR_KEYCODE_HOME;
    cr_keycode_table[SDL_SCANCODE_PAGEUP] = CR_KEYCODE_PAGEUP;
    cr_keycode_table[SDL_SCANCODE_DELETE] = CR_KEYCODE_DELETE;
    cr_keycode_table[SDL_SCANCODE_END] = CR_KEYCODE_END;
    cr_keycode_table[SDL_SCANCODE_PAGEDOWN] = CR_KEYCODE_PAGEDOWN;
    cr_keycode_table[SDL_SCANCODE_RIGHT] = CR_KEYCODE_RIGHT;
    cr_keycode_table[SDL_SCANCODE_LEFT] = CR_KEYCODE_LEFT;
    cr_keycode_table[SDL_SCANCODE_DOWN] = CR_KEYCODE_DOWN;
    cr_keycode_table[SDL_SCANCODE_UP] = CR_KEYCODE_UP;
    cr_keycode_table[SDL_SCANCODE_NUMLOCKCLEAR] = CR_KEYCODE_NUMLOCKCLEAR;
    cr_keycode_table[SDL_SCANCODE_KP_DIVIDE] = CR_KEYCODE_KP_DIVIDE;
    cr_keycode_table[SDL_SCANCODE_KP_MULTIPLY] = CR_KEYCODE_KP_MULTIPLY;
    cr_keycode_table[SDL_SCANCODE_KP_MINUS] = CR_KEYCODE_KP_MINUS;
    cr_keycode_table[SDL_SCANCODE_KP_PLUS] = CR_KEYCODE_KP_PLUS;
    cr_keycode_table[SDL_SCANCODE_KP_ENTER] = CR_KEYCODE_KP_ENTER;
    cr_keycode_table[SDL_SCANCODE_KP_1] = CR_KEYCODE_KP_1;
    cr_keycode_table[SDL_SCANCODE_KP_2] = CR_KEYCODE_KP_2;
    cr_keycode_table[SDL_SCANCODE_KP_3] = CR_KEYCODE_KP_3;
    cr_keycode_table[SDL_SCANCODE_KP_4] = CR_KEYCODE_KP_4;
    cr_keycode_table[SDL_SCANCODE_KP_5] = CR_KEYCODE_KP_5;
    cr_keycode_table[SDL_SCANCODE_KP_6] = CR_KEYCODE_KP_6;
    cr_keycode_table[SDL_SCANCODE_KP_7] = CR_KEYCODE_KP_7;
    cr_keycode_table[SDL_SCANCODE_KP_8] = CR_KEYCODE_KP_8;
    cr_keycode_table[SDL_SCANCODE_KP_9] = CR_KEYCODE_KP_9;
    cr_keycode_table[SDL_SCANCODE_KP_0] = CR_KEYCODE_KP_0;
    cr_keycode_table[SDL_SCANCODE_KP_PERIOD] = CR_KEYCODE_KP_PERIOD;
    cr_keycode_table[SDL_SCANCODE_LCTRL] = CR_KEYCODE_LCTRL;
    cr_keycode_table[SDL_SCANCODE_LSHIFT] = CR_KEYCODE_LSHIFT;
    cr_keycode_table[SDL_SCANCODE_LALT] = CR_KEYCODE_LALT;
    cr_keycode_table[SDL_SCANCODE_LGUI] = CR_KEYCODE_LGUI;
    cr_keycode_table[SDL_SCANCODE_RCTRL] = CR_KEYCODE_RCTRL;
    cr_keycode_table[SDL_SCANCODE_RSHIFT] = CR_KEYCODE_RSHIFT;
    cr_keycode_table[SDL_SCANCODE_RALT] = CR_KEYCODE_RALT;
    cr_keycode_table[SDL_SCANCODE_RGUI] = CR_KEYCODE_RGUI;
    cr_keycode_table[SDL_SCANCODE_NONUSBACKSLASH] = CR_KEYCODE_NONUSBACKSLASH;
    cr_keycode_table[SDL_SCANCODE_APPLICATION] = CR_KEYCODE_APPLICATION;
}
