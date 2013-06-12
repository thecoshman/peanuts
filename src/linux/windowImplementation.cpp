#include <string>
#include "windowImplementation.hpp"
#include "common/events.hpp"
#include "keySymToGeneric.hpp"

namespace Peanuts {
    static bool contextErrorOccurred = false; // If we go multi thread, may need to consider locking on this... 
    static int contextErrorHandler(Display *dpy, XErrorEvent *ev ) {
        char buffer[512];
        XGetErrorText(dpy, ev->error_code, buffer, 512);
        std::cout << "X reported an error: " << buffer << std::endl;
        contextErrorOccurred = true;
        return 0;
    }
    
    WindowImplementation::WindowImplementation(WindowOptions options) : display(XOpenDisplay(nullptr)), context(0) {
        initDisplay();
        auto style = passWindowOptions(options);
        auto frameBufferConfig = findBestFrameBufferConfig(style);
        auto visualInfo = glXGetVisualFromFBConfig(display, frameBufferConfig); // requires XFree
        XSetWindowAttributes windowAttribs;
        windowAttribs.colormap = XCreateColormap(display, RootWindow(display, visualInfo->screen), visualInfo->visual, AllocNone);
        //windowAttribs.event_mask = FocusChangeMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask | ExposureMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask;
        windowAttribs.event_mask = xeventMask;
        windowAttribs.border_pixel = 0;
        xWindow = XCreateWindow(
            display, RootWindow(display, visualInfo->screen), style.x, style.y, style.width, style.height, 0, visualInfo->depth,
            InputOutput, visualInfo->visual, CWBorderPixel | CWColormap | CWEventMask, &windowAttribs);
        if(!xWindow){
            throw std::runtime_error("XCreateWindow Failed");
        }
        // XStoreName(display, win, "GL 3.0 Window" );
        XMapWindow(display, xWindow);
        // Note this error handler is global.  All display connections in all threads
        // of a process use the same error handler, so be sure to guard against other
        // threads issuing X commands while this code is running.
        contextErrorOccurred = false;
        int (*oldContextErrorHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&contextErrorHandler);
        //auto startup_context = glXCreateNewContext(display, frameBufferConfig, GLX_RGBA_TYPE, 0, True);
        auto startup_context = glXCreateContext(display, visualInfo, nullptr, True);
        XFree(visualInfo);
        if(!startup_context){
            throw std::runtime_error("Failed to create basic OpenGL context");
        }
        int context_attribs[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
            GLX_CONTEXT_MINOR_VERSION_ARB, 0,
            //GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
            None
        };
        glXMakeCurrent(display, xWindow, startup_context);
        XSync(display, False);
        if (contextErrorOccurred || !context){
            std::runtime_error("Error whilst attempting tmake context current");
        }
        loadGLFunctions();
        context = glXCreateContextAttribsARB(display, frameBufferConfig, 0, True, context_attribs);
        // Sync to ensure any errors generated are processed.
        XSync(display, False);
        if (contextErrorOccurred || !context){
            std::runtime_error("Error whilst attempting to get OpenGL Context");
        }
        // Restore the original error handler
        XSetErrorHandler(oldContextErrorHandler);
        glXMakeCurrent(display, xWindow, context); // this may want to be moved to a seperate function, so it does not have to be done on window creation
        glXDestroyContext(display, startup_context);
        loadGLFunctions();
    }
    
    WindowImplementation::~WindowImplementation() {
        glXDestroyContext(display, context);
        XDestroyWindow(display, xWindow);
        // XFreeColormap(display, colormap);
        XCloseDisplay(display );
    }
    
    void WindowImplementation::swapBuffers(){
        glXSwapBuffers(display, xWindow);
    }

    void WindowImplementation::initDisplay(){
        if(!display) {
            throw std::runtime_error("Could not open X display");
        }
        int versionMajor = 0;
        int versionMinor = 0;
        if (!glXQueryVersion(display, &versionMajor, &versionMinor)){
            throw std::runtime_error("'glXQueryVersion' failed - cannot determine supported OpenGL version");
        }
        if ((versionMajor < 1) || (versionMajor == 1 && versionMinor < 4)){
            std::cout << "Detected Version is " << versionMajor << "." << versionMinor << std::endl;
            throw std::runtime_error("This version of OpenGL is not supported. Requires at least 1.4");
        }
    }
    
    void WindowImplementation::loadGLFunctions(){
        if(glload::LoadFunctions() == glload::LS_LOAD_FAILED){
            throw std::runtime_error("'glload::LoadFunctions' failed");
        }
        if(glload::LoadGLXFunctions(display, DefaultScreen(display)) == glload::LS_LOAD_FAILED){
            throw std::runtime_error("'glload::LoadGLXFunctions' failed");
        }
    }
    
    GLXFBConfig WindowImplementation::findBestFrameBufferConfig(const WindowStyle& style){
        int visualAttribs[] = {
           GLX_X_RENDERABLE,  True,
           GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
           GLX_RENDER_TYPE,   GLX_RGBA_BIT,
           GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
           GLX_RED_SIZE,      style.redBits,  // 8
           GLX_GREEN_SIZE,    style.greenBits, // 8
           GLX_BLUE_SIZE,     style.blueBits, // 8
           GLX_ALPHA_SIZE,    style.alphaBits, // 8
           GLX_DEPTH_SIZE,    style.depthBits,  // 24
           GLX_STENCIL_SIZE,  style.stencilBits, // 8
           GLX_DOUBLEBUFFER,  True,
           None
        };
        int frameBufferConfigCount;
        auto frameBufferConfigs = glXChooseFBConfig(display, DefaultScreen(display), visualAttribs, &frameBufferConfigCount); // requires XFree
        if (!frameBufferConfigs){
            throw std::runtime_error("Failed to get frame buffer configurations");
        }
        int best = -1, bestNumberSamples = -1;//,worst_fbc = -1, worst_num_samp = 999;
        for(int i = 0; i < frameBufferConfigCount; ++i){
            XVisualInfo *visualInfo = glXGetVisualFromFBConfig(display, frameBufferConfigs[i]);
            if (visualInfo){
                int sampleBuffer, samples;
                glXGetFBConfigAttrib(display, frameBufferConfigs[i], GLX_SAMPLE_BUFFERS, &sampleBuffer);
                glXGetFBConfigAttrib(display, frameBufferConfigs[i], GLX_SAMPLES, &samples);
                if (best < 0 || (sampleBuffer && (samples > bestNumberSamples))){
                    best = i, bestNumberSamples = samples;
                }
            }
            XFree(visualInfo);
        }
        GLXFBConfig bestFrameBufferConfig = frameBufferConfigs[best];
        //GLXFBConfig bestFrameBufferConfig = frameBufferConfigs[0]; // TODO update this so that it actually looks through the array to find 'best' config
        XFree (frameBufferConfigs );
        return bestFrameBufferConfig;
    }
    
    WindowStyle WindowImplementation::passWindowOptions(const WindowOptions& options){
        WindowStyle style;
        style.redBits = options.redBits;
        style.greenBits = options.greenBits;
        style.blueBits = options.blueBits;
        style.alphaBits = options.alphaBits;
        style.depthBits = options.depthBits;
        style.stencilBits = options.stencilBits;
        boost::apply_visitor(WindowModeVistitor(style), options.mode);
        //std::string     title;
        //OpenGLVersion   glVersion;
        return style;
    }
    
    void WindowImplementation::pumpEvents(){
        XEvent xEvent;
        EventTypes event;
        while(XCheckWindowEvent(display, xWindow, xeventMask, &xEvent)){
            EventTypes event;
            switch(xEvent.type){
                case DestroyNotify:
                    std::cout << xEvent.type << " Window Destroy Event" << std::endl;
                    event = Event::Close{};
                    break;
                case KeyPress:
                    event = Event::KeyDown(convertKeySymToGeneric(XLookupKeysym(&xEvent.xkey, 1)));
                    break;
                case KeyRelease:
                    event = Event::KeyUp(convertKeySymToGeneric(XLookupKeysym(&xEvent.xkey, 1)));
                    break;
                default:
                    //std::cout << xEvent.type << std::endl;
                    break;
            }
            storeEvent(event);
        }
    }
}