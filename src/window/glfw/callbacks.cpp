#include "glfw.hpp"

namespace SplitGui {
    void GlfwInterface::resize_callback(glfw::Window& window, int width, int height) {

        printf("resize requested\n");

        EventHandler* pEventHandler = ((EventHandler*)window.getUserPointer());

        Event* pEvent = pEventHandler->fetchEvent(
                EventAttachment(
                    EventAttachment::Category::eWindow, 
                    EventAttachment::WindowType::eResize
                )
            );

        if (!pEvent) {
            printf("invalid event\n");
            return;
        }

        pEvent->call(pEventHandler->getContext());
        
        printf("Window resized to %d x %d\n", width, height);
    }

    void GlfwInterface::key_callback(glfw::Window& window, glfw::KeyCode key, int scancode, glfw::KeyState action, glfw::ModifierKeyBit mods) {

        printf("key press: %d\n", key);

        EventHandler* pEventHandler = ((EventHandler*)window.getUserPointer());

        Event* pEvent = pEventHandler->fetchEvent(
                EventAttachment(
                    EventAttachment::Category::eWindow, 
                    EventAttachment::WindowType::eMouseButton
                )
            );
        
        if (!pEvent) {
            return;
        }

    }

    void GlfwInterface::mouseButton_callback(glfw::Window& window, glfw::MouseButton mouseButton, glfw::MouseButtonState mouseState, glfw::ModifierKeyBit mods) {
        printf("mouse button press: %d\n", mouseButton);

    }

}