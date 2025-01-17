#include "glfw.hpp"

namespace SplitGui {
    GlfwInterface::~GlfwInterface() {
        if (window.handle) {
            delete window.handle;
        }

        glfwTerminate();
    }
}