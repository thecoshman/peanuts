#pragma once
#include "keycode.hpp"

namespace Peanuts {
    namespace Event {
        struct MouseMove {
            int x, y;
        };
        struct KeyDown{
            KeyDown(KeyCode key): key(key){};
            KeyCode key;
        };
        struct KeyUp{
            KeyUp(KeyCode key): key(key){};
            KeyCode key;
        };
        struct WindowResize {
            int width, height;
        };
        enum class FucosGrabState {
            Normal, WhileGrabbed, Grab, Ungrab,
        };
        struct FocusLoose {
            FocusLoose(FucosGrabState grabState = FucosGrabState::Normal): grabState(grabState){}
            FucosGrabState grabState;
        };
        struct FocusGain {
            FocusGain(FucosGrabState grabState = FucosGrabState::Normal): grabState(grabState){}
            FucosGrabState grabState;
        };
        struct Close{};
    }

    typedef boost::variant<Event::MouseMove, Event::KeyDown, Event::KeyUp,
        Event::WindowResize, Event::FocusLoose, Event::FocusGain,
        Event::Close> EventTypes;
}
