#pragma once

namespace Peanuts{
    namespace Platform {
        enum class KeyCode{
            // TODO add in all 'generic' keycodes. eg:
            A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
            ESC,
            F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
            // Perhaps mouse and joystick buttons will be via another enum
            UNKOWN_KEY,
        };
    }
}