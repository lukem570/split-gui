#include <splitgui/window.hpp>
#include <splitgui/interface.hpp>
#include <cstdio>

namespace SplitGui {

    Window::Window() {

    }

    void Window::createWindow() {

    }

    

    void Window::attachInterface(Interface& ui) {
        Window::interface = &ui;
    }

    void Window::updateInterface() {

    }

    

}