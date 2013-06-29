#include <string>
#include "windowImplementation.hpp"
#include "common/events.hpp"
#include "keySymToGeneric.hpp"
#include "X11/Xatom.h"

namespace Peanuts {
    namespace linuxDetails{
        Peanuts::Event::FucosGrabState grabModeToGrabState(int x11EventGrabMode) {
            switch(x11EventGrabMode){
            case NotifyNormal:
                return Peanuts::Event::FucosGrabState::Normal; 
                break;
            case NotifyWhileGrabbed:
                return Peanuts::Event::FucosGrabState::WhileGrabbed; 
                break;
            case NotifyGrab:
                return Peanuts::Event::FucosGrabState::Grab; 
                break;
            case NotifyUngrab:
                return Peanuts::Event::FucosGrabState::Ungrab; 
                break;                
            }
        };

        Peanuts::MouseButton xButtonStateToMouseButton(unsigned int xButtonState){
            if(xButtonState == Button1){ return MouseButton::LEFT; }
            if(xButtonState == Button2){ return MouseButton::MIDDLE; }
            if(xButtonState == Button3){ return MouseButton::RIGHT; }
            if(xButtonState == Button4){ return MouseButton::SCROLL_UP; }
            if(xButtonState == Button5){ return MouseButton::SCROLL_DOWN; }
            std::cout << "button state : " << xButtonState << std::endl;
            return MouseButton::UNKOWN_BUTTON;
        }

        int matchMotion(Display *display, XEvent* xEvent, XPointer arguments){
            if(xEvent->type == MotionNotify){
                return 1;
            }
            return 0;
        } 
    }

    bool contextErrorOccurred = false; // If we go multi thread, may need to consider locking on this... 
    int contextErrorHandler(Display *display, XErrorEvent *xErrorEvent ) {
        char buffer[512];
        XGetErrorText(display, xErrorEvent->error_code, buffer, 512);
        std::cout << "X reported an error: " << buffer << std::endl;
        contextErrorOccurred = true;
        return 0;
    }
    
    WindowImplementation::WindowImplementation(WindowOptions options) : display(XOpenDisplay(nullptr)), context(0) {
        initDisplay();
        auto style = passWindowOptions(options);
        if(style.center){
            style.x = (DisplayWidth(display, DefaultScreen(display)) - style.width) / 2;
            style.y = (DisplayHeight(display, DefaultScreen(display)) - style.height) / 2;
        }
        if(style.maximised){
            style.width = DisplayWidth(display, DefaultScreen(display));
            style.height = DisplayHeight(display, DefaultScreen(display));
        }
        // These are stored for the sake of sending events
        width = style.width; height = style.height;

        auto frameBufferConfig = findBestFrameBufferConfig(style);
        auto visualInfo = glXGetVisualFromFBConfig(display, frameBufferConfig);

        XSetWindowAttributes windowAttribs;
        windowAttribs.colormap = XCreateColormap(display, RootWindow(display, visualInfo->screen), visualInfo->visual, AllocNone);
        windowAttribs.event_mask = xEventMask;
        windowAttribs.border_pixel = 0;
        windowAttribs.override_redirect = True; 
        xWindow = XCreateWindow(
            display, RootWindow(display, visualInfo->screen), style.x, style.y, style.width, style.height, 0, visualInfo->depth,
            InputOutput, visualInfo->visual, CWBorderPixel | CWColormap | CWEventMask, &windowAttribs);
        if(!xWindow){
            throw std::runtime_error("XCreateWindow Failed");
        }
        if(style.deborder){
            unsigned long HINTS_FUNCTIONS   = 1 << 0;
            unsigned long HINTS_DECORATIONS = 1 << 1;
            //unsigned long DECOR_BORDER      = 1 << 1;
            //unsigned long DECOR_RESIZE     = 1 << 2;
            //unsigned long DECOR_TITLE       = 1 << 3;
            //unsigned long DECOR_MENU        = 1 << 4;
            //unsigned long DECOR_MINIMIZE    = 1 << 5;
            //unsigned long DECOR_MAXIMIZE    = 1 << 6;
            //unsigned long FUNC_RESIZE       = 1 << 1;
            //unsigned long FUNC_MOVE         = 1 << 2;
            //unsigned long FUNC_MINIMIZE     = 1 << 3;
            //unsigned long FUNC_MAXIMIZE     = 1 << 4;
            //unsigned long FUNC_CLOSE        = 1 << 5;
            
            struct Hints{
                unsigned long   flags;
                unsigned long   functions;
                unsigned long   decorations;
                long            inputMode;
                unsigned long   status;
            };
            Hints hints;
            hints.flags = HINTS_FUNCTIONS | HINTS_DECORATIONS;
            hints.decorations = 0; // turn off
            hints.functions = 0; // turn off
            auto property = XInternAtom(display, "_MOTIF_WM_HINTS", True);
            if(property){
                XChangeProperty(display, xWindow, property, property, 32, PropModeReplace, reinterpret_cast<const unsigned char*>(&hints), 5);
            }
        }
        if(style.fullScreen){
            long mask = StructureNotifyMask | ResizeRedirectMask;
            XEvent event; {
                event.xclient.type = ClientMessage;
                event.xclient.serial = 0;
                event.xclient.send_event = True;
                event.xclient.display = display;
                event.xclient.window = xWindow;
                event.xclient.message_type = XInternAtom(display, "_NET_WM_STATE", False);
                event.xclient.format = 32;
                event.xclient.data.l[0] = 1;  /* set (2 is toggle) */
                event.xclient.data.l[1] = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);
                event.xclient.data.l[2] = 0;
                event.xclient.data.l[3] = 0;
                event.xclient.data.l[4] = 0;
            }
            XSendEvent(display, XDefaultRootWindow(display), False, mask, &event);
        } 

        XStoreName(display, xWindow, options.title.c_str());
        XMapWindow(display, xWindow);
        XMoveWindow(display, xWindow, style.x, style.y);
        XFlush(display);

        // Note this error handler is global.  All display connections in all threads
        // of a process use the same error handler, so be sure to guard against other
        // threads issuing X commands while this code is running.
        contextErrorOccurred = false;
        int (*oldContextErrorHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&contextErrorHandler);

        auto startup_context = glXCreateContext(display, visualInfo, nullptr, True);
        XFree(visualInfo);
        if(!startup_context){
            throw std::runtime_error("Failed to create basic OpenGL context");
        }
        int context_attribs[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, options.glVersion.versionMajor,
            GLX_CONTEXT_MINOR_VERSION_ARB, options.glVersion.versionMinor,
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
        glXMakeCurrent(display, xWindow, context);
        glXDestroyContext(display, startup_context);
        XSelectInput(display, xWindow, xEventMask);
        loadGLFunctions();
    }
    
    WindowImplementation::~WindowImplementation(){
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
        style.fullScreen = style.deborder = style.center = style.maximised = false;

        boost::apply_visitor(WindowModeVistitor(style), options.mode);
        return style;
    }
    
    void WindowImplementation::pumpEvents(){
        XEvent xEvent;
        EventTypes event;
        while(XPending(display)){
            XNextEvent(display, &xEvent);
            EventTypes event;
            switch(xEvent.type){
                case DestroyNotify:
                    std::cout << xEvent.type << " Window Destroy Event" << std::endl;
                    event = Event::Close{};
                    break;
                case KeyPress:
                    event = Event::KeyDown{convertKeySymToGeneric(XLookupKeysym(&xEvent.xkey, 1))};
                    break;
                case KeyRelease:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
                    event = Event::KeyUp{convertKeySymToGeneric(XLookupKeysym(&xEvent.xkey, 1))};
                    break;
                case ConfigureNotify:
                    break;
                    if(xEvent.xconfigure.width != width || xEvent.xconfigure.height != height){
                        event = Event::WindowResize{xEvent.xconfigure.width, xEvent.xconfigure.height};
                        width = xEvent.xconfigure.width;
                        height = xEvent.xconfigure.height;
                    }
                    break;
                case FocusIn:
                    event = Event::FocusGain(linuxDetails::grabModeToGrabState(xEvent.xfocus.mode));
                    break;
                case FocusOut:
                    event = Event::FocusLoose(linuxDetails::grabModeToGrabState(xEvent.xfocus.mode));
                    break;
                case MotionNotify:
                    while(XCheckIfEvent(display, &xEvent, linuxDetails::matchMotion, 0)){
                        if(xEvent.xmotion.window == xWindow){
                            event = Event::MouseMove{xEvent.xmotion.x, xEvent.xmotion.y};
                        }
                    }
                case ButtonPress:
                    event = Event::MouseDown{linuxDetails::xButtonStateToMouseButton(xEvent.xbutton.button)};
                    break;
                case ButtonRelease:
                    event = Event::MouseUp{linuxDetails::xButtonStateToMouseButton(xEvent.xbutton.button)};
                    break;
                default:
                    std::cout << xEvent.type << std::endl;
                    return;
            }
            storeEvent(event);
        }
    }
}