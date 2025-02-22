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
        if (windowLib) {
            delete windowLib;
        }
    }

    Result Window::instanceGlfw() {
        windowLib = (WindowLibInterface*) new(std::nothrow) GlfwInterface();
        
        if (!windowLib) {
            return Result::eHeapAllocFailed;
        }

        return Result::eSuccess;
    }

    Result Window::createWindow(const char* title) {
        TRYR(windowRes, windowLib->createWindow(title));

        return Result::eSuccess;
    }

    RawWindow* Window::getWindowData() {
        return windowLib->getWindowData();
    }

    bool Window::shouldClose() {
        return windowLib->shouldClose();
    }

    void Window::update() {
        windowLib->update();
    }

    IVec2 Window::getSize() {
        return windowLib->getSize();
    }

    void Window::attachEventHandler(EventHandler& handler) {
        handler.attachWindow(this);
        pEventHandler = &handler;
        windowLib->submitEventHandler(pEventHandler);
    }

    void Window::close() {
        windowLib->close();
    }

    void Window::minimize() {
        windowLib->minimize();
    }

    void Window::maximize() {
        windowLib->maximize();
    }

#ifdef SPLIT_GUI_USE_VULKAN

    ResultValue<vk::SurfaceKHR> Window::createSurface(vk::Instance instance) {
        return windowLib->createSurface(instance);
    }

#endif
}