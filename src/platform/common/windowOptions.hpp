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

        typedef boost::variant<FullScreen, Windowed> WindowOptions;
    }
}
