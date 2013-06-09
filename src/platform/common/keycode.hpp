#pragma once

namespace Peanuts{
    namespace Platform {
        enum class KeyCode{
            F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
            NUM_0, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9,
            A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
            BACK_TICK, PLUS, MINUS, BACK_SLASH, FORWARD_SLASH, SPACE,
            NUMPAD_0, NUMPAD_1, NUMPAD_2, NUMPAD_3, NUMPAD_4, NUMPAD_5, NUMPAD_6, NUMPAD_7, NUMPAD_8, NUMPAD_9,
            NUMPAD_ENTER, NUMPAD_MULTIPLY, NUMPAD_PLUS, NUMPAD_MINUS, NUMPAD_DECIMAL_POINT, NUMPAD_DIVIDE,
            LSHIFT, RSHIFT, LCTRL, RCTRL,
            BACKSPACE, ENTER, INSERT, DELETE,
            LEFT, UP, RIGHT, DOWN, TAB, HOME, END, PG_UP, PG_DOWN,
            NUM_LOCK, CAPS_LOCK, SCROLL_LOCK,
            ESC,
            UNKOWN_KEY,
        };
    }
}