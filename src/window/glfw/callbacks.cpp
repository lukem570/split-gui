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

    void GlfwInterface::key_callback(glfw::Window& window, glfw::KeyCode key, int scancode, glfw::KeyState state, glfw::ModifierKeyBit mods) {

        printf("key press: %d\n", key);

        EventHandler* pEventHandler = ((EventHandler*)window.getUserPointer());

        Event event = Event(Event::Category::eWindow, Event::WindowType::eKeypress);

        event.data.window.keypress.keyCode = convertGlfwKeyCode(key);
        event.data.window.keypress.keyState = convertGlfwKeyState(state);

        pEventHandler->pushEvent(event);
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

        printf("mouse move: (%f, %f)\n", posX, posY);

        EventHandler* pEventHandler = ((EventHandler*)window.getUserPointer());

        pEventHandler->pushEvent(
            Event(
                Event::Category::eWindow, 
                Event::WindowType::eMouseMove
            )
        );
    }
}