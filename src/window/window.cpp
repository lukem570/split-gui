#include <splitgui/window.hpp>
#include <splitgui/interface.hpp>
#include <glfwpp/glfwpp.h>
#include <cstdio>
#include <string>

namespace SplitGui {

    Window::Window() {

    }

    Window::~Window() {
        if (handle) {
            delete handle;
        }
        
        if (glfw) {
            delete glfw;
        }
    }

    void Window::createWindow(const char* title) {
        glfw = new glfw::GlfwLibrary(glfw::init());

        glfw::WindowHints hints;
        
        handle = new glfw::Window(640, 480, title);

        glfw::makeContextCurrent(*handle);

        handle->swapBuffers();
    }

    void Window::attachInterface(Interface& ui) {
        Window::interface = &ui;
    }

    void Window::updateInterface() {

    }

    void Window::__devLoop() {
        while (!handle->shouldClose()) {
            handle->swapBuffers();
            glfw::pollEvents();
        }
    }

}