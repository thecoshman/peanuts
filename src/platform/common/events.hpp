#pragma once
#include "keycode.hpp"

namespace Peanuts {
    namespace Platform {
        struct MouseMove {
            std::pair<int,int> position;
        };
        struct KeyDown{
            KeyCode key;
        };
        // TODO add in all the other events that the OS can provide
        
        typedef boost::variant<MouseMove, KeyDown> Event;
    }
}
