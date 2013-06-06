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
                WindowStyle passWindowOptions(const WindowOptions& options);
                long xeventMask = FocusChangeMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask | ExposureMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask;
            public:
                WindowImplementation(WindowOptions options);
                ~WindowImplementation();
                void swapBuffers();
                void pumpEvents();
        };
    }
}
