#include "glfw.hpp"

namespace SplitGui {
    GlfwInterface::GlfwInterface() {
        glfwInit();
        window.type = FormatType::eGlfw;
    }

    inline Result GlfwInterface::createWindow(WindowFlags flags) {
        glfw::WindowHints hints;
        hints.clientApi = (glfw::ClientApi) 0L;
        hints.resizable = flags.resizeable;
        hints.visible   = flags.visible;
        hints.decorated = flags.decorated;

        hints.apply();
        
        window.handle = new(std::nothrow) glfw::Window(640, 480, flags.title);

        if (!window.handle) {
            return Result::eHeapAllocFailed;
        }

        glfw::makeContextCurrent(*window.handle);

        window.handle->framebufferSizeEvent.setCallback(resize_callback);
        window.handle->keyEvent.setCallback(key_callback);
        window.handle->mouseButtonEvent.setCallback(mouseButton_callback);
        window.handle->cursorPosEvent.setCallback(mouseMove_callback);
        window.handle->scrollEvent.setCallback(scroll_callback);
        
        window.handle->swapBuffers();

        return Result::eSuccess;
    }

    void GlfwInterface::submitEventHandler(EventHandler* pEventHandler) {
        window.handle->setUserPointer(pEventHandler);
    }
}