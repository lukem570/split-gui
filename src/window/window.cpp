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

    Result Window::createWindow(WindowFlags flags) {
        TRYR(windowRes, windowLib->createWindow(flags));

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

    void Window::restore() {
        windowLib->restore();
    }

    bool Window::maximized() {
        return windowLib->maximized();
    }

    void Window::setWindowPosition(IVec2 position) {
        windowLib->setWindowPosition(position);
    }

    IVec2 Window::getWindowPosition() {
        return windowLib->getWindowPosition();
    }

    IVec2 Window::getCursorPosition() {
        return windowLib->getCursorPosition();
    }

#ifdef SPLIT_GUI_USE_VULKAN

    ResultValue<vk::SurfaceKHR> Window::createSurface(vk::Instance instance) {
        return windowLib->createSurface(instance);
    }

#endif
}