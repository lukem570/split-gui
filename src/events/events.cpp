#include <splitgui/events.hpp>

#include "builtin.cpp"

namespace SplitGui {

    void EventHandler::attachWindow(Window* pWindow) {
        eventContext.pWindow = pWindow;
    }

    void EventHandler::attachGraphics(Graphics* pGraphics) {
        eventContext.pGraphics = pGraphics;
    }

    void EventHandler::attachInterface(Interface* pInterface) {
        eventContext.pInterface = pInterface;
    }

    void EventHandler::attachScene(Scene* pScene) {
        eventContext.pScene = pScene;
    }

    int EventHandler::popEvent() {
        if (events.size() > 0) {
            eventBuffer = events.top();
            events.pop();
            return events.size() + 1;
        } else {
            return 0;
        }
    }

    void EventHandler::pushEvent(Event event) {
        events.push(event);
        callBuiltinEvent(event);
    }

    Event EventHandler::getEvent() {
        return eventBuffer;
    }

    EventData::~EventData() {
        switch (type) {
            case Type::eWindow: window.~WindowEventData(); break;
            case Type::eInterface: interface.~InterfaceEventData(); break;
        }
    }
    
}