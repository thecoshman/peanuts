#pragma once
#include <stack>
#include <memory>
#include "windowOptions.hpp"
#include "genericEventHandler.hpp"

namespace Peanuts {
    namespace Platform {
        class Window {
            private:
                std::stack<std::weak_ptr<genericEventHandler>> eventHandlers; /*
my thinking is, just before we attempt to send events, we check if still valid, if not pop it off and try again */
                void sendEvent(Event e); /*
This will be used to save the indavidual platforms from all re-implimenting the event system, they just send the events to this */
            public:
                void pumpEvents();
                void newEventHandler(std::weak_ptr<genericEventHandler> newHandler);
                void swapBuffers();

                static std::unique_ptr<Window> Create(WindowOptions options, int BPP = 32);
        };
        
    }
}
