#include "window.hpp"
#include "util.hpp"

#ifdef PEANUTS_WINDOWS
    #include "windows/windowImplementation.hpp"
#elif defined PEANUTS_LINUX
    #include "linux/windowImplementation.hpp"
#elif defined PEANUTS_OSX
    #include "osx/windowImplementation.hpp"
#endif

namespace Peanuts {
    std::unique_ptr<Window> Window::create(WindowOptions options){
        return Peanuts::make_unique<WindowImplementation>(options);
    }

    void Window::storeEvent(EventTypes& event){
        eventQueue.push_back(event);
    }

    boost::optional<EventTypes> Window::pollEvent(){
        if(eventQueue.empty()){
            return boost::optional<EventTypes>();
        }
        auto event = eventQueue.front();
        eventQueue.pop_front();
        return boost::optional<EventTypes>(event);
    }
}
