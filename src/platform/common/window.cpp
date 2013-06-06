#include "window.hpp"
#include "util.hpp"

#ifdef PEANUTS_WINDOWS
    #include "platform/windows/windowImplementation.hpp"
#elif defined PEANUTS_LINUX
    #include "platform/linux/windowImplementation.hpp"
#elif defined PEANUTS_OSX
    #include "platform/osx/windowImplementation.hpp"
#endif

namespace Peanuts {
    namespace Platform {
        std::unique_ptr<Window> Window::Create(WindowOptions options){
			return Peanuts::make_unique<WindowImplementation>(options);
        }

        EventTypes Window::pollEvent(){
            
        }
    }
}
