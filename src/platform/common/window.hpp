#pragma once
#include <deque>
#include <memory>
#include <boost/optional/optional.hpp>
#include "windowOptions.hpp"
#include "genericEventHandler.hpp"

namespace Peanuts {
    namespace Platform {
        class Window {
            private:                
                std::deque<EventTypes> eventQueue;
            protected:
                void storeEvent(EventTypes& event);
            public:
                boost::optional<EventTypes> pollEvent();
                virtual void pumpEvents()=0;
                virtual void swapBuffers()=0;

                static std::unique_ptr<Window> create(WindowOptions options);
        };
        
    }
}
