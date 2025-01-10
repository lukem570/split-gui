#include <splitgui/window.hpp>
#include <splitgui/interface.hpp>
#include <splitgui/structs.hpp>
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

    RawWindow* Window::getWindowData() {
        return windowLib->getWindowData();
    }

    // TODO: move to windowlib

    bool Window::shouldClose() {
        return windowLib->shouldClose();
    }

    void Window::update() {
        windowLib->update();
    }

    IVec2 Window::getSize() {
        return windowLib->getSize();
    }

#ifdef SPLIT_GUI_USE_VULKAN

    vk::SurfaceKHR Window::createSurface(vk::Instance instance) {
        return windowLib->createSurface(instance);
    }

#endif
}