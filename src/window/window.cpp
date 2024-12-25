#include <splitgui/window.hpp>
#include <splitgui/interface.hpp>
#include <glfwpp/glfwpp.h>
#include <cstdio>
#include <string>

namespace SplitGui {

    Window::Window() {

    }

    void Window::createWindow(const char* title) {
        glfw::GlfwLibrary GLFW = glfw::init();

        glfw::WindowHints hints;
        
        glfw::Window window(640, 480, title);

        glfw::makeContextCurrent(window);

        while (!window.shouldClose()) {
            glfw::pollEvents();
        }
        
    }

    void Window::attachInterface(Interface& ui) {
        Window::interface = &ui;
    }

    void Window::updateInterface() {

    }

    

}