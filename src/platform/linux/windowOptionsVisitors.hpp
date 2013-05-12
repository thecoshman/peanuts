#pragma once
#include "../common/windowOptions.hpp"

namespace Peanuts {
    namespace Platform {
        struct WindowStyle {
            bool fullScreen, borders;
            int x, y, width, height, BPP, redBits, greenBits, blueBits, alphaBits, depthBits, stencilBits;
        };

        struct WindowOptionsVistitor : boost::static_visitor<> {

            WindowOptionsVistitor(WindowStyle& windowStyle) : style(windowStyle) {}
            WindowStyle& style;

            void operator()(FullScreen & options) const {
                style.x = style.y = 0;
                style.fullScreen = true;
                style.width = options.res.first;
                style.height = options.res.second;
                style.borders = 0;
            }
            void operator()(Windowed & options) const {
                if(options.borders == Borders::On){
                    style.borders = true;
                } else {
                    style.borders = false;
                }
            }
        };
    }
}