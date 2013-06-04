#pragma once
#include <boost/lexical_cast.hpp>
#include "../common/windowOptions.hpp"

namespace Peanuts {
    namespace Platform {
        struct WindowStyle {
            bool fullScreen, borders;
            int x, y, width, height, BPP, redBits, greenBits, blueBits, alphaBits, depthBits, stencilBits;
        };

        struct WindowSizeVisitor : boost::static_visitor<> {
            WindowSizeVisitor(const WindowSizeVisitor&) = delete;
            WindowSizeVisitor(WindowSizeVisitor&&) = delete;
            WindowSizeVisitor(WindowStyle& windowStyle) : style(windowStyle) {}
            WindowStyle& style;

            void operator()(const Maximised& size) const{
                style.width = style.height = 100; // need to find current screen size and use that, also need to consider borders
            }

            void operator()(const std::pair<int, int>& size) const{
                style.width = size.first;
                style.height = size.second;
            }
        };

        struct WindowPositionVisitor : boost::static_visitor<> {
            WindowPositionVisitor(const WindowPositionVisitor&) = delete;
            WindowPositionVisitor(WindowPositionVisitor&&) = delete;
            WindowPositionVisitor(WindowStyle& windowStyle) : style(windowStyle) {}
            WindowStyle& style;

            void operator()(const Centered& position) const{
                style.x = style.y = 100; // need to find current screen size and use that, also need to consider borders
            }

            void operator()(const std::pair<int, int>& postion) const{
                style.x = postion.first;
                style.y = postion.second;
            }
        };

        struct WindowModeVistitor : boost::static_visitor<> {
            WindowModeVistitor(const WindowModeVistitor&) = delete;
            WindowModeVistitor(WindowModeVistitor&&) = delete;
            WindowModeVistitor(WindowStyle& windowStyle) : style(windowStyle) {}
            WindowStyle& style;

            void operator()(const FullScreen& mode) const {
                style.x = style.y = 0;
                style.fullScreen = true;
                style.width = mode.res.first;
                style.height = mode.res.second;
                style.borders = false;
            }
            void operator()(const Windowed& mode) const {
                if(mode.borders == Borders::On){
                    style.borders = true;
                } else {
                    style.borders = false;
                }
                boost::apply_visitor(WindowSizeVisitor(style), mode.size);
                boost::apply_visitor(WindowPositionVisitor(style), mode.position);
            }
        };
    }
}