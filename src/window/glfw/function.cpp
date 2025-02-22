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

}