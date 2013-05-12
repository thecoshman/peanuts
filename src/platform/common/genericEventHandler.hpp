#pragma once
#include "events.hpp"

namespace Peanuts {
    namespace Platform {
        struct genericEventHandler{
            // These functions probably need to be made virtual
            void handle(MouseMove e){
            }
            void handle(KeyDown e){
            }
        };
    }
}