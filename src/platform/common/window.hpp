#pragma once
#include <deque>
#include <memory>
#include "windowOptions.hpp"
#include "genericEventHandler.hpp"

namespace Peanuts {
    namespace Platform {
        class Window {
            protected:                
                std::deque<EventTypes> events;
            private:
                void sendEvent(EventTypes e);
            public:
                EventTypes pollEvent();
                virtual void pumpEvents()=0;
                virtual void swapBuffers()=0;

                static std::unique_ptr<Window> Create(WindowOptions options);
        };
        
    }
}
