#include "glfw.hpp"

namespace SplitGui {
    RawWindow* GlfwInterface::getWindowData() {
        return &window;
    }

    IVec2 GlfwInterface::getSize() {
        IVec2 size;
        std::tuple<int, int> sizeTuple = window.handle->getSize();
        size.x = std::get<0>(sizeTuple);
        size.y = std::get<1>(sizeTuple);
        return size;
    }

    bool GlfwInterface::shouldClose() {
        return window.handle->shouldClose();
    }

    IVec2 GlfwInterface::getWindowPosition() {
        std::tuple<int, int> position = window.handle->getPos();
        return {std::get<0>(position), std::get<1>(position)};
    }

    IVec2 GlfwInterface::getCursorPosition() {
        std::tuple<int, int> position = window.handle->getCursorPos();
        return {std::get<0>(position), std::get<1>(position)};
    }

    bool GlfwInterface::maximized() {
        return window.handle->getAttribMaximized();
    }

    MouseState GlfwInterface::getMouseState(MouseCode code) {
        return convertGlfwMouseState((glfw::MouseButtonState)glfwGetMouseButton(*window.handle, (int)convertSplitGuiMouseCode(code)));
    }
}