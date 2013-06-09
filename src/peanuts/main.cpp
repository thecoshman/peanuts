#include <chrono>
#include <thread>
#include <glload/gl_core.hpp>
#include "platform/common/window.hpp"
#include <iostream>

namespace Peanuts {

    bool run;
    struct EventHandler : Platform::genericEventHandler{
        using Platform::genericEventHandler::operator();
        void operator()(const Platform::Event::Close& event) const{
            std::cout << "Window closed!" << std::endl;
            run = false;
        }
        void operator()(const Platform::Event::KeyDown& event) const{
            if (event.key == Platform::KeyCode::Q){
                run = false;
            }
        }
    };
    int Main() {
        run = true;
        Platform::WindowOptions windowOptions("GL test", Platform::Windowed(std::pair<int,int>(640,480),Platform::Centered()), Platform::OpenGLVersion(1, 4));
        auto win  = Platform::Window::create(windowOptions);
        EventHandler eventHandler;

        gl::ClearColor(1.0f, 0.0f, 0.0f, 0.0f);
        std::chrono::milliseconds dura( 2000 );
        while (run) {
            gl::Clear(gl::GL_COLOR_BUFFER_BIT);
            std::this_thread::sleep_for(dura);
            win->pumpEvents();
            while(auto event = win->pollEvent()){
                boost::apply_visitor(eventHandler, *event);
            }
            win->swapBuffers();
        }
        return 0;
    }
}
