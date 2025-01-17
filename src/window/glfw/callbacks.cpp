#include "glfw.hpp"

namespace SplitGui {
    void GlfwInterface::resize_callback(glfw::Window& window, int width, int height) {

        ((Graphics*)window.getUserPointer())->resizeEvent();
        
        printf("Window resized to %d x %d\n", width, height);
    }
}