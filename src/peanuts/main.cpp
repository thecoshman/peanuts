#include "platform/common/window.hpp"
#include <iostream>

namespace Peanuts {
    int Main() {
    	Platform::WindowOptions windowOptions("GL test", Platform::Windowed(std::pair<int,int>(640,480),Platform::Centered()), Platform::OpenGLVersion(1, 4));
    	auto win  = Platform::Window::Create(windowOptions);
        return 0;
    }
}
