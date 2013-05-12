#include "platform/common/window.hpp"
#include <iostream>

namespace Peanuts {
    int Main() {
    	auto win  = Platform::Window::Create(Platform::Windowed(std::pair<int,int>(640,480),Platform::Centered()));
        return 0;
    }
}
