#pragma once
#include "keycode.hpp"

namespace Peanuts {
    namespace Event{
        struct MouseMove {
            std::pair<int,int> position;
        };
        struct KeyDown{
            KeyDown(KeyCode key): key(key){};
            KeyCode key;
        };
        struct KeyUp{
            KeyUp(KeyCode key): key(key){};
            KeyCode key;
        };
        struct Close{};
    }
    // TODO add in all the other events that the OS can provide    
    typedef boost::variant<Event::MouseMove, Event::KeyDown, Event::KeyUp, Event::Close> EventTypes;
}
