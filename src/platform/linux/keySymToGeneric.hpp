// This file is intended to be included only by 'windowImplementation.cpp' and only after 'keycode.hpp' will have been included
namespace Peanuts {
    KeyCode convertKeySymToGeneric(KeySym keysym){
        switch (keysym){
            case 65:    return KeyCode::A;
            case 66:    return KeyCode::B;
            case 67:    return KeyCode::C;
            case 68:    return KeyCode::D;
            case 69:    return KeyCode::E;
            case 70:    return KeyCode::F;
            case 71:    return KeyCode::G;
            case 72:    return KeyCode::H;
            case 73:    return KeyCode::I;
            case 74:    return KeyCode::J;
            case 75:    return KeyCode::K;
            case 76:    return KeyCode::L;
            case 77:    return KeyCode::M;
            case 78:    return KeyCode::N;
            case 79:    return KeyCode::O;
            case 80:    return KeyCode::P;
            case 81:    return KeyCode::Q;
            case 82:    return KeyCode::R;
            case 83:    return KeyCode::S;
            case 84:    return KeyCode::T;
            case 85:    return KeyCode::U;
            case 86:    return KeyCode::V;
            case 87:    return KeyCode::W;
            case 88:    return KeyCode::X;
            case 89:    return KeyCode::Y;
            case 90:    return KeyCode::Z;

            case 32:    return KeyCode::SPACE;
            case 43:    return KeyCode::PLUS;
            case 95:    return KeyCode::MINUS;
            case 58:    return KeyCode::COLON;                
            case 60:    return KeyCode::COMMA;
            case 62:    return KeyCode::PERIOD;
            case 63:    return KeyCode::FORWARD_SLASH;
            case 124:   return KeyCode::BACK_SLASH;
            case 123:   return KeyCode::BRACE_OPEN;
            case 125:   return KeyCode::BRACE_CLOSE;
            case 126:   return KeyCode::TILDE;
            case 172:   return KeyCode::BACK_TICK;
            case 64:    return KeyCode::AT;

            case 33:    return KeyCode::NUM_1;
            case 34:    return KeyCode::NUM_2;
            case 163:   return KeyCode::NUM_3;
            case 36:    return KeyCode::NUM_4;
            case 37:    return KeyCode::NUM_5;
            case 94:    return KeyCode::NUM_6;
            case 38:    return KeyCode::NUM_7;
            case 42:    return KeyCode::NUM_8;
            case 40:    return KeyCode::NUM_9;
            case 41:    return KeyCode::NUM_0;
                
            case 65470: return KeyCode::F1;
            case 65471: return KeyCode::F2;
            case 65472: return KeyCode::F3;
            case 65473: return KeyCode::F4;
            case 65474: return KeyCode::F5;
            case 65475: return KeyCode::F6;
            case 65476: return KeyCode::F7;
            case 65477: return KeyCode::F8;
            case 65478: return KeyCode::F9;
            case 65479: return KeyCode::F10;
            case 65480: return KeyCode::F11;
            case 65481: return KeyCode::F12;

            case 65407: return KeyCode::NUM_LOCK;
            case 65509: return KeyCode::CAPS_LOCK;
            case 65300: return KeyCode::SCROLL_LOCK;
                
            case 65421: return KeyCode::NUMPAD_ENTER;
            case 65450: return KeyCode::NUMPAD_MULTIPLY;
            case 65451: return KeyCode::NUMPAD_PLUS;
            case 65453: return KeyCode::NUMPAD_MINUS;
            case 65454: return KeyCode::NUMPAD_DECIMAL_POINT;
            case 65455: return KeyCode::NUMPAD_DIVIDE;

            case 65456: return KeyCode::NUMPAD_0;
            case 65457: return KeyCode::NUMPAD_1;
            case 65458: return KeyCode::NUMPAD_2;
            case 65459: return KeyCode::NUMPAD_3;
            case 65460: return KeyCode::NUMPAD_4;
            case 65461: return KeyCode::NUMPAD_5;
            case 65462: return KeyCode::NUMPAD_6;
            case 65463: return KeyCode::NUMPAD_7;
            case 65464: return KeyCode::NUMPAD_8;
            case 65465: return KeyCode::NUMPAD_9;

            case 65505: return KeyCode::LSHIFT;
            case 65506: return KeyCode::RSHIFT;
            case 65507: return KeyCode::LCTRL;
            case 65508: return KeyCode::RCTRL;
            case 65511: return KeyCode::ALT;
            case 65312: return KeyCode::ALT_GR;
            case 65383: return KeyCode::MENU;
            case 65515: return KeyCode::WINDOWS;

            case 65056: return KeyCode::TAB;
            case 65361: return KeyCode::LEFT;
            case 65362: return KeyCode::UP;
            case 65363: return KeyCode::RIGHT;
            case 65364: return KeyCode::DOWN;
            case 65360: return KeyCode::HOME;
            case 65367: return KeyCode::END;
            case 65365: return KeyCode::PG_UP;
            case 65366: return KeyCode::PG_DOWN;

            case 65288: return KeyCode::BACKSPACE;
            case 65293: return KeyCode::ENTER;
            case 65379: return KeyCode::INSERT;
            case 65535: return KeyCode::DELETE;
                
            case 65307: return KeyCode::ESC;
            case 65387: return KeyCode::PAUSE_BREAK;

            default:
                std::cout << "No idea about keysym: '" << keysym << "'" << std::endl;
        }
        return KeyCode::UNKOWN_KEY;
    }
}