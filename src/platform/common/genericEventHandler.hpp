#pragma once
#include "events.hpp"

namespace Peanuts {
    namespace Platform {
        struct genericEventHandler : boost::static_visitor<>{
            // These functions probably need to be made virtual
            template <typename T>
            void operator()(const T& event){

            }
        };
    }
}