#pragma once
#include "events.hpp"

namespace Peanuts {
    namespace Platform {
        struct genericEventHandler : boost::static_visitor<>{
            template <typename T>
            void operator()(const T& event){
                std::cout << "erm... where you wanting the genericEventHandler to handle this..." << std::endl;
            }
        };
    }
}