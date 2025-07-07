#include "glfw.hpp"

namespace SplitGui {
    RawWindow* GlfwInterface::getWindowData() {
        SPLITGUI_PROFILE;

        return &window;
    }

    IVec2 GlfwInterface::getSize() {
        SPLITGUI_PROFILE;

        IVec2 size;
        std::tuple<int, int> sizeTuple = window.handle->getSize();
        size.x = std::get<0>(sizeTuple);
        size.y = std::get<1>(sizeTuple);
        return size;
    }

    bool GlfwInterface::shouldClose() {
        SPLITGUI_PROFILE;

        return window.handle->shouldClose();
    }

    IVec2 GlfwInterface::getWindowPosition() {
        SPLITGUI_PROFILE;

        std::tuple<int, int> position = window.handle->getPos();
        return {std::get<0>(position), std::get<1>(position)};
    }

    IVec2 GlfwInterface::getCursorPosition() {
        SPLITGUI_PROFILE;

        std::tuple<int, int> position = window.handle->getCursorPos();
        return {std::get<0>(position), std::get<1>(position)};
    }

    bool GlfwInterface::maximized() {
        SPLITGUI_PROFILE;

        return window.handle->getAttribMaximized();
    }

    Vec2 GlfwInterface::getMonitorContextScale() {
        SPLITGUI_PROFILE;

        auto [retX, retY] = window.handle->getContentScale();
        return { retX, retY };
    }

    MouseState GlfwInterface::getMouseState(MouseCode code) {
        SPLITGUI_PROFILE;

        return convertGlfwMouseState((glfw::MouseButtonState)glfwGetMouseButton(*window.handle, (int)convertSplitGuiMouseCode(code)));
    }

    KeyState GlfwInterface::getKeyState(KeyCode code) {
        SPLITGUI_PROFILE;

        return convertGlfwKeyState((glfw::KeyState)glfwGetKey(*window.handle, (int)convertSplitGuiKeyCode(code)));
    }
}