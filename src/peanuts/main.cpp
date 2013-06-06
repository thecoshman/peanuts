#include <chrono>
#include <thread>
#include <glload/gl_core.hpp>
#include "platform/common/window.hpp"
#include <iostream>

namespace Peanuts {

    bool run;
    struct EventHandler : Platform::genericEventHandler{
        void operator()(const Platform::Event::Close& event) const{
            std::cout << "Window closed!" << std::endl;
            run = false;
        }
    };
    int Main() {
        run = true;
        Platform::WindowOptions windowOptions("GL test", Platform::Windowed(std::pair<int,int>(640,480),Platform::Centered()), Platform::OpenGLVersion(1, 4));
        auto win  = Platform::Window::Create(windowOptions);
        EventHandler eventHandler;
        //win->newEventHandler(eventHandler);

        gl::ClearColor(1.0f, 0.0f, 0.0f, 0.0f);
        std::chrono::milliseconds dura( 2000 );
        while (run) {
            win->pumpEvents();
            gl::Clear(gl::GL_COLOR_BUFFER_BIT);
            win->swapBuffers();
            std::this_thread::sleep_for(dura);
        }
        return 0;
    }
}
