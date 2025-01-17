#include "glfw.hpp"

namespace SplitGui {
    GlfwInterface::GlfwInterface() {
        glfwInit();
        window.type = FormatType::eGlfw;
    }

    inline void GlfwInterface::createWindow(const char* title) {
        glfw::WindowHints hints;
        hints.clientApi = (glfw::ClientApi) 0L;
        hints.resizable = true;
        hints.visible   = true;
        hints.decorated = true;

        hints.apply();
        
        window.handle = new glfw::Window(640, 480, title);

        glfw::makeContextCurrent(*window.handle);

        window.handle->framebufferSizeEvent.setCallback(resize_callback);
        
        window.handle->swapBuffers();
    }

    void GlfwInterface::submitGraphics(Graphics* pGraphics) {

        printf("graphics submitted\n");

        window.pGraphics = pGraphics;

        window.handle->setUserPointer(pGraphics);
    }
}