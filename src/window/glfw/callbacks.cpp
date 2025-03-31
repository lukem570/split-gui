#include "glfw.hpp"

namespace SplitGui {
    void GlfwInterface::resize_callback(glfw::Window& window, int width, int height) {
        SPLITGUI_PROFILE;

        EventHandler* pEventHandler = ((EventHandler*)window.getUserPointer());

        Event event = Event(Event::Category::eWindow, Event::WindowType::eResize);

        pEventHandler->pushResult(pEventHandler->pushEvent(event));
    }

    void GlfwInterface::key_callback(glfw::Window& window, glfw::KeyCode key, int scancode, glfw::KeyState state, glfw::ModifierKeyBit mods) {
        SPLITGUI_PROFILE;

        EventHandler* pEventHandler = ((EventHandler*)window.getUserPointer());

        Event event = Event(Event::Category::eWindow, Event::WindowType::eKeypress);

        event.data.type = EventData::Type::eWindow;

        event.data.window.keypress.keyCode  = convertGlfwKeyCode(key);
        event.data.window.keypress.keyState = convertGlfwKeyState(state);

        pEventHandler->pushResult(pEventHandler->pushEvent(event));
    }

    void GlfwInterface::mouseButton_callback(glfw::Window& window, glfw::MouseButton button, glfw::MouseButtonState state, glfw::ModifierKeyBit mods) {
        SPLITGUI_PROFILE;

        EventHandler* pEventHandler = ((EventHandler*)window.getUserPointer());

        Event event = Event(Event::Category::eWindow, Event::WindowType::eMouseButton);

        event.data.type = EventData::Type::eWindow;
        
        event.data.window.mouseButton.mouseCode  = convertGlfwMouseButton(button);
        event.data.window.mouseButton.mouseState = convertGlfwMouseState(state);

        pEventHandler->pushResult(pEventHandler->pushEvent(event));
    }

    void GlfwInterface::mouseMove_callback(glfw::Window& window, double posX, double posY) {
        SPLITGUI_PROFILE;

        EventHandler* pEventHandler = ((EventHandler*)window.getUserPointer());

        Event event = Event(Event::Category::eWindow, Event::WindowType::eMouseMove);

        event.data.type = EventData::Type::eWindow;
        
        event.data.window.mouseMove.xPos = (int) posX;
        event.data.window.mouseMove.yPos = (int) posY;

        pEventHandler->pushResult(pEventHandler->pushEvent(event));
    }

    void GlfwInterface::scroll_callback(glfw::Window& window, double offX, double offY) {
        SPLITGUI_PROFILE;

        EventHandler* pEventHandler = ((EventHandler*)window.getUserPointer());

        Event event = Event(Event::Category::eWindow, Event::WindowType::eScroll);

        event.data.type = EventData::Type::eWindow;

        event.data.window.mouseScroll.xOff = (int) offX;
        event.data.window.mouseScroll.yOff = (int) offY;

        pEventHandler->pushResult(pEventHandler->pushEvent(event));
    }

    void GlfwInterface::char_callback(glfw::Window& window, unsigned int key) {
        SPLITGUI_PROFILE;

        EventHandler* pEventHandler = ((EventHandler*)window.getUserPointer());

        Event event = Event(Event::Category::eWindow, Event::WindowType::eLetter);

        event.data.type = EventData::Type::eWindow;

        event.data.window.letter.letter = key;

        pEventHandler->pushResult(pEventHandler->pushEvent(event));
    }
}