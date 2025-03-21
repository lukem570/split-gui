#include <splitgui/events.hpp>
#include <splitgui/logger.hpp>

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

    EventHandler::~EventHandler() {
        Logger::cleanup();
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

    Result EventHandler::pushEvent(Event event) {
        TRYR(builtinRes, callBuiltinEvent(event));
        events.push(event);

        return Result::eSuccess;
    }

    Event EventHandler::getEvent() {
        return eventBuffer;
    }
    
    Result EventHandler::getResult() {
        return currentResult;
    }
    
    void EventHandler::pushResult(Result res) {
        currentResult = res;
    }
    
}