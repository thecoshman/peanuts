#pragma once
#include <boost/lexical_cast.hpp>
#include "common/windowOptions.hpp"

namespace Peanuts {
    struct WindowStyle {
        bool fullScreen, deborder, center, maximised;
        int x, y, width, height, BPP, redBits, greenBits, blueBits, alphaBits, depthBits, stencilBits;
    };

    struct WindowSizeVisitor : boost::static_visitor<> {
        WindowSizeVisitor(const WindowSizeVisitor&) = delete;
        WindowSizeVisitor(WindowSizeVisitor&&) = delete;
        WindowSizeVisitor(WindowStyle& windowStyle) : style(windowStyle) {}
        WindowStyle& style;
    
        void operator()(const Maximised& size) const{
            style.maximised = true;
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
            style.x = style.y = -666;
            style.center = true;
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
            style.fullScreen = style.deborder = true;
            style.width = mode.res.first;
            style.height = mode.res.second;
        }
        void operator()(const Windowed& mode) const {
            if(mode.borders == Borders::Off){
                style.deborder = true;
            }
            boost::apply_visitor(WindowPositionVisitor(style), mode.position);
            boost::apply_visitor(WindowSizeVisitor(style), mode.size);
        }
    };
}