#include "glfw.hpp"

namespace SplitGui {
    void GlfwInterface::resize_callback(glfw::Window& window, int width, int height) {

        EventHandler* pEventHandler = ((EventHandler*)window.getUserPointer());

        Event event = Event(Event::Category::eWindow, Event::WindowType::eResize);

        pEventHandler->pushResult(pEventHandler->pushEvent(event));
    }

    void GlfwInterface::key_callback(glfw::Window& window, glfw::KeyCode key, int scancode, glfw::KeyState state, glfw::ModifierKeyBit mods) {

        EventHandler* pEventHandler = ((EventHandler*)window.getUserPointer());

        Event event = Event(Event::Category::eWindow, Event::WindowType::eKeypress);

        event.data.type = EventData::Type::eWindow;

        event.data.window.keypress.keyCode  = convertGlfwKeyCode(key);
        event.data.window.keypress.keyState = convertGlfwKeyState(state);

        pEventHandler->pushResult(pEventHandler->pushEvent(event));
    }

    void GlfwInterface::mouseButton_callback(glfw::Window& window, glfw::MouseButton button, glfw::MouseButtonState state, glfw::ModifierKeyBit mods) {

        EventHandler* pEventHandler = ((EventHandler*)window.getUserPointer());

        Event event = Event(Event::Category::eWindow, Event::WindowType::eMouseButton);

        event.data.type = EventData::Type::eWindow;
        
        event.data.window.mouseButton.mouseCode  = convertGlfwMouseButton(button);
        event.data.window.mouseButton.mouseState = convertGlfwMouseState(state);

        pEventHandler->pushResult(pEventHandler->pushEvent(event));
    }

    void GlfwInterface::mouseMove_callback(glfw::Window& window, double posX, double posY) {

        EventHandler* pEventHandler = ((EventHandler*)window.getUserPointer());

        Event event = Event(Event::Category::eWindow, Event::WindowType::eMouseMove);

        event.data.type = EventData::Type::eWindow;
        
        event.data.window.mouseMove.xPos = (int) posX;
        event.data.window.mouseMove.yPos = (int) posY;

        pEventHandler->pushResult(pEventHandler->pushEvent(event));
    }
}