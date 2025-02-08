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
            case Type::eWindow:    window.~WindowEventData();       break;
            case Type::eInterface: interface.~InterfaceEventData(); break;
        }
    }

    EventData EventData::operator=(const EventData& data) {
        EventData dataRet = data;

        switch (data.type) {
            case Type::eWindow:    dataRet.window    = data.window;    break;
            case Type::eInterface: dataRet.interface = data.interface; break;
        }

        return dataRet;
    }

    EventData::EventData(const EventData& data) {
        switch (data.type) {
            case Type::eWindow:    type = data.type; window    = data.window;    break;
            case Type::eInterface: type = data.type; interface = data.interface; break;
        }
    }

    InterfaceEventData InterfaceEventData::operator=(const InterfaceEventData& data) {
        InterfaceEventData dataRet = data;

        switch (data.type) {
            case Type::eFunctionCall: dataRet.functionCall = data.functionCall; break;
        }

        return dataRet;
    }

    InterfaceEventData::InterfaceEventData(const InterfaceEventData& data) {
        switch (data.type) {
            case Type::eFunctionCall: functionCall.alias = data.functionCall.alias; functionCall.params = data.functionCall.params; functionCall.returnValue = data.functionCall.returnValue; break;
        }
    }

    InterfaceEventData::~InterfaceEventData() {
        switch (type) {
            case Type::eFunctionCall: functionCall.~FunctionCall(); break;
        }
    }
    
}