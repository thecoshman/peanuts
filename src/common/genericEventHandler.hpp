#pragma once
#include "events.hpp"

namespace Peanuts {
    struct genericEventHandler : boost::static_visitor<>{
        template <typename T> void operator()(const T& event) const {} 
    };
}