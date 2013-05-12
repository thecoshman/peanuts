#pragma once
#include <stdexcept>
#include <X11/Xlib.h>
#include <glload/gl_core.hpp>
#include <glload/glx_exts.h>
//#include <glload/gl_3_2.hpp>
#include <glload/gll.hpp>
#include "platform/common/window.hpp"
#include "windowOptionsVisitors.hpp"


namespace Peanuts {
    namespace Platform {
        class WindowImplementation : public Window {
                Display *display;
                ::Window xWindow;
                GLXContext context;

                void initDisplay();
                void loadGLFunctions();
                GLXFBConfig findBestFrameBufferConfig(const WindowStyle& style);
            public:
                WindowImplementation(WindowOptions options, int BPP = 32);
                ~WindowImplementation();
                void swapBuffers();
        };
    }
}
