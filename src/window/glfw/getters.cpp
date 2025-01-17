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

    void GlfwInterface::update() {
        window.handle->swapBuffers();
        glfw::pollEvents();
    }
}