#include <chrono>
#include <thread>
#include <glload/gl_core.hpp>
#include "platform/common/window.hpp"
#include <iostream>

bool run;
struct EventHandler : Peanuts::genericEventHandler{
    using Peanuts::genericEventHandler::operator();
    void operator()(const Peanuts::Event::Close& event) const{
        std::cout << "Window closed!" << std::endl;
        run = false;
    }
    void operator()(const Peanuts::Event::KeyDown& event) const{
        if (event.key == Peanuts::KeyCode::Q){
            run = false;
        }
    }
};

int Main() {
    run = true;
    Peanuts::WindowOptions windowOptions("GL test", Peanuts::Windowed(std::pair<int,int>(640,480),Peanuts::Centered()), Peanuts::OpenGLVersion(1, 4));
    auto win  = Peanuts::Window::create(windowOptions);
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
