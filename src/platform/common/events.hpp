#pragma once
#include "keycode.hpp"

namespace Peanuts {
    namespace Platform {
        namespace Event{
            struct MouseMove {
                std::pair<int,int> position;
            };
            struct KeyDown{
                KeyCode key;
            };
            struct Close{};
        }
        // TODO add in all the other events that the OS can provide
        
        typedef boost::variant<Event::MouseMove, Event::KeyDown, Event::Close> EventTypes;
    }
}
