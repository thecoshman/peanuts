#pragma once
#include "events.hpp"

namespace Peanuts {
    namespace Platform {
        struct genericEventHandler : boost::static_visitor<>{
            template <typename T> void operator()(const T& event) const {} 
        };
    }
}