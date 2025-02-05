#include "glfw.hpp"

namespace SplitGui {
    void GlfwInterface::resize_callback(glfw::Window& window, int width, int height) {

        printf("resize requested\n");

        EventHandler* pEventHandler = ((EventHandler*)window.getUserPointer());

        pEventHandler->pushEvent(
            Event(
                Event::Category::eWindow, 
                Event::WindowType::eResize
            )
        );
        
        printf("Window resized to %d x %d\n", width, height);
    }

    void GlfwInterface::key_callback(glfw::Window& window, glfw::KeyCode key, int scancode, glfw::KeyState action, glfw::ModifierKeyBit mods) {

        printf("key press: %d\n", key);

        EventHandler* pEventHandler = ((EventHandler*)window.getUserPointer());

        pEventHandler->pushEvent(
            Event(
                Event::Category::eWindow, 
                Event::WindowType::eKeypress
            )
        );
    }

    void GlfwInterface::mouseButton_callback(glfw::Window& window, glfw::MouseButton mouseButton, glfw::MouseButtonState mouseState, glfw::ModifierKeyBit mods) {
        printf("mouse button press: %d\n", mouseButton);

        EventHandler* pEventHandler = ((EventHandler*)window.getUserPointer());

        pEventHandler->pushEvent(
            Event(
                Event::Category::eWindow, 
                Event::WindowType::eMouseButton
            )
        );
    }

    void GlfwInterface::mouseMove_callback(glfw::Window& window, double posX, double posY) {

        EventHandler* pEventHandler = ((EventHandler*)window.getUserPointer());

        pEventHandler->pushEvent(
            Event(
                Event::Category::eWindow, 
                Event::WindowType::eMouseMove
            )
        );
    }
}