#pragma once
#include <boost/variant.hpp>
#include "util.hpp"

namespace Peanuts {
    struct FullScreen{
        FullScreen(std::pair<int, int> resolution): res(resolution) {} 
        std::pair<int, int> res;
    };
    
    struct Maximised {};
    typedef std::pair<int, int> size;
    typedef boost::variant<Maximised, size> WindowSize;

    struct Centered {};
    typedef std::pair<int, int> position;
    typedef boost::variant<Centered, position> WindowPosition;
    
    enum class Borders {
        On, Off
    };
    struct Windowed{
        Windowed(Maximised winSize, Borders winBorders = Borders::On): size(winSize), position(std::make_pair(0,0)), borders(winBorders) {}
        Windowed(std::pair<int, int> winSize, WindowPosition winPosition, Borders winBorders = Borders::On): size(winSize), position(winPosition), borders(winBorders) {}
        WindowSize size;
        WindowPosition position;
        Borders borders;
    };
    
    typedef boost::variant<FullScreen, Windowed> WindowMode;
    
    struct OpenGLVersion{
        OpenGLVersion(int versionMajor = 1, int versionMinor = 4)
            : versionMajor(versionMajor), versionMinor(versionMinor){}
        int versionMajor, versionMinor;
    };
    
    struct WindowOptions{
        WindowOptions(
            const std::string& title, WindowMode mode, OpenGLVersion glVersion,
            unsigned char redBits = 8, unsigned char greenBits = 8, unsigned char blueBits = 8, unsigned char alphaBits = 8,
            unsigned char depthBits = 24, unsigned char stencilBits = 8):
                title(title), mode(mode), glVersion(glVersion),
                redBits(redBits), greenBits(greenBits), blueBits(blueBits), alphaBits(alphaBits),
                depthBits(depthBits),stencilBits(stencilBits){}
        std::string     title;
        WindowMode      mode;
        OpenGLVersion   glVersion;
        unsigned char   redBits, greenBits, blueBits, alphaBits;
        unsigned char   depthBits, stencilBits;
    };
}
