#pragma once
#include "keycode.hpp"
#include "mousebutton.hpp"

namespace Peanuts {
    namespace Event {
        struct MouseMove {
            int x, y;
        };
        struct MouseDown {
            MouseButton button;
        };
        struct MouseUp {
            MouseButton button;
        };
        struct KeyDown{
            KeyCode key;
        };
        struct KeyUp{
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

    typedef boost::variant<Event::MouseMove, Event::MouseDown, Event::MouseUp, Event::KeyDown, Event::KeyUp,
        Event::WindowResize, Event::FocusLoose, Event::FocusGain,
        Event::Close> EventTypes;
}
