#pragma once
#include <stdexcept>
#include <X11/Xlib.h>
#include <glload/gl_core.hpp>
#include <glload/glx_exts.h>
//#include <glload/gl_3_2.hpp>
#include <glload/gll.hpp>
#include "common/window.hpp"
#include "windowOptionsVisitors.hpp"


namespace Peanuts {
    class WindowImplementation : public Window {
        Display *display;
        ::Window xWindow;
        GLXContext context;
        int width, height;
        long xEventMask = FocusChangeMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask | ExposureMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask;

        void initDisplay();
        void loadGLFunctions();
        GLXFBConfig findBestFrameBufferConfig(const WindowStyle& style);
        WindowStyle passWindowOptions(const WindowOptions& options);
    public:
        WindowImplementation(WindowOptions options);
        ~WindowImplementation();
        void swapBuffers();
        void pumpEvents();
    };
}
