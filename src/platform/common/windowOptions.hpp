#pragma once
#include <boost/variant.hpp>
#include <utility>

namespace Peanuts {
    namespace Platform {
        struct FullScreen{
            FullScreen(std::pair<int, int> resolution): res(resolution) {} 
            std::pair<int, int> res;
        };

        struct Maximised {};
        typedef boost::variant<Maximised, std::pair<int, int> > WindowSize;

        struct Centered {};
        typedef boost::variant<Centered, std::pair<int, int> > WindowPosition;

        enum class Borders {
            On, Off
        };

        struct Windowed{
            Windowed(WindowSize winSize, WindowPosition winPosition, Borders winBorders = Borders::On): size(winSize), position(winPosition), borders(winBorders) {}
            WindowSize size;
            WindowPosition position;
            Borders borders;
        };

        typedef boost::variant<FullScreen, Windowed> WindowMode;

        struct OpenGLVersion{
            OpenGLVersion(unsigned int versionMajor = 1, unsigned int versionMinor = 4)
                : versionMajor(versionMajor), versionMinor(versionMinor){}
            unsigned int versionMajor, versionMinor;
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
}
