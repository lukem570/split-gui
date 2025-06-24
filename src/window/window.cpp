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
        SPLITGUI_PROFILE;
        
        if (windowLib) {
            ma::untrack(windowLib);
            delete windowLib;
        }
    }

    Result Window::instanceGlfw() {
        SPLITGUI_PROFILE;

        windowLib = (WindowLibInterface*) new(std::nothrow) GlfwInterface();
        ma::track(windowLib, "glfw instance");
        
        if (!windowLib) {
            return Result::eHeapAllocFailed;
        }

        return Result::eSuccess;
    }

    Result Window::createWindow(WindowFlags flags) {
        SPLITGUI_PROFILE;

        TRYR(windowRes, windowLib->createWindow(flags));

        return Result::eSuccess;
    }

    RawWindow* Window::getWindowData() {
        SPLITGUI_PROFILE;

        return windowLib->getWindowData();
    }

    bool Window::shouldClose() {
        SPLITGUI_PROFILE;

        return windowLib->shouldClose();
    }

    void Window::update() {
        SPLITGUI_PROFILE;

        windowLib->update();
    }

    IVec2 Window::getSize() {
        SPLITGUI_PROFILE;

        return windowLib->getSize();
    }

    void Window::attachEventHandler(EventHandler& handler) {
        SPLITGUI_PROFILE;

        handler.attachWindow(this);
        pEventHandler = &handler;
        windowLib->submitEventHandler(pEventHandler);
    }

    void Window::close() {
        SPLITGUI_PROFILE;

        windowLib->close();
    }

    void Window::minimize() {
        SPLITGUI_PROFILE;

        windowLib->minimize();
    }

    void Window::maximize() {
        SPLITGUI_PROFILE;

        windowLib->maximize();
    }

    void Window::restore() {
        SPLITGUI_PROFILE;

        windowLib->restore();
    }

    bool Window::maximized() {
        SPLITGUI_PROFILE;

        return windowLib->maximized();
    }

    void Window::setWindowPosition(IVec2 position) {
        SPLITGUI_PROFILE;

        windowLib->setWindowPosition(position);
    }

    IVec2 Window::getWindowPosition() {
        SPLITGUI_PROFILE;

        return windowLib->getWindowPosition();
    }

    IVec2 Window::getCursorPosition() {
        SPLITGUI_PROFILE;

        return windowLib->getCursorPosition();
    }

    void Window::setCursorShape(CursorType type) {
        SPLITGUI_PROFILE;

        windowLib->setCursorShape(type);
    }

    void Window::setSize(IVec2 size) {
        SPLITGUI_PROFILE;

        windowLib->setSize(size);
    }

    MouseState Window::getMouseState(MouseCode code) {
        SPLITGUI_PROFILE;

        return windowLib->getMouseState(code);
    }

    Vec2 Window::getMonitorContextScale() {
        SPLITGUI_PROFILE;

        return windowLib->getMonitorContextScale();
    }

#ifdef SPLIT_GUI_USE_VULKAN

    ResultValue<vk::SurfaceKHR> Window::createSurface(vk::Instance instance) {
        SPLITGUI_PROFILE;

        return windowLib->createSurface(instance);
    }

#endif
}