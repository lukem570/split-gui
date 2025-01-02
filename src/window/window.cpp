#include <splitgui/window.hpp>
#include <splitgui/interface.hpp>
#include <cstdio>
#include <string>

#ifdef SPLIT_GUI_USE_GLFW
    #include "glfw/glfw.cpp"
#else
    #error "windowing lib not implemented"
#endif

namespace SplitGui {

    Window::Window() {

    }

    Window::~Window() {     
        #ifdef SPLIT_GUI_USE_GLFW
        
        if (windowLib) {
            delete windowLib;
        }

        #else
            #error "not implemented"
        #endif

    }

    void Window::instanceGlfw() {
        #ifdef SPLIT_GUI_USE_GLFW

        windowLib = (WindowLibInterface*) new GlfwInterface();

        #else
        printf("Missing: \n'#define SPLIT_GUI_USE_GLFW'");
        throw;
        #endif
    }

    void Window::createWindow(const char* title) {
        windowLib->createWindow(title);
    }

    void Window::attachInterface(Interface& ui) {
        Window::interface = &ui;
    }

    void Window::updateInterface() {

    }

    RawWindow* Window::getWindowData() {
        return windowLib->getWindowData();
    }

    bool Window::shouldClose() {
        return windowLib->getWindowData()->handle->shouldClose();
    }

    void Window::update() {
        windowLib->getWindowData()->handle->swapBuffers();
        glfw::pollEvents();
    }

    void Window::__devLoop() {
        while (!windowLib->getWindowData()->handle->shouldClose()) {
            windowLib->getWindowData()->handle->swapBuffers();
            glfw::pollEvents();
        }
    }

}