#include "glfw.hpp"

namespace SplitGui {
    void GlfwInterface::close() {
        window.handle->setShouldClose(true);
    }

    void GlfwInterface::minimize() {
        window.handle->iconify();
    }

    void GlfwInterface::maximize() {
        window.handle->maximize();
    }

    void GlfwInterface::restore() {
        window.handle->restore();
    }

    bool GlfwInterface::maximized() {
        return window.handle->getAttribMaximized();
    }

    void GlfwInterface::setWindowPosition(IVec2 position) {
        window.handle->setPos(position.x, position.y);
    }

    IVec2 GlfwInterface::getWindowPosition() {
        std::tuple<int, int> position = window.handle->getPos();
        return {std::get<0>(position), std::get<1>(position)};
    }

    IVec2 GlfwInterface::getCursorPosition() {
        std::tuple<int, int> position = window.handle->getCursorPos();
        return {std::get<0>(position), std::get<1>(position)};
    }

    void GlfwInterface::setCursorShape(CursorType type) {
        switch (type) {
            case CursorType::eArrow:           window.handle->setCursor(glfw::Cursor::createStandardCursorArrow());            break;
            case CursorType::eCrosshair:       window.handle->setCursor(glfw::Cursor::createStandardCursorCrosshair());        break;
            case CursorType::eIBeam:           window.handle->setCursor(glfw::Cursor::createStandardCursorIBeam());            break;
            case CursorType::eHand:            window.handle->setCursor(glfw::Cursor::createStandardCursorHand());             break;
            case CursorType::eHorizontalArrow: window.handle->setCursor(glfw::Cursor::createStandardCursorHorizontalResize()); break;
            case CursorType::eVerticalArrow:   window.handle->setCursor(glfw::Cursor::createStandardCursorVerticalResize());   break;
            default: break;
        }
        
    }
}