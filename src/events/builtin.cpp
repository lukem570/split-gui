#include <splitgui/structs.hpp>
#include <splitgui/events.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/interface.hpp>
#include <splitgui/window.hpp>

namespace SplitGui {
    static void resizeEvent(Context eventContext) {
        if (!eventContext.pGraphics) {
            return;
        }

        eventContext.pGraphics->resizeEvent();

        if (!eventContext.pInterface) {
            return;
        }
        
        // TODO: fix for not full window viewport sizes
        RectObj viewport;
        viewport.size = eventContext.pWindow->getSize();
        viewport.x    = 0;
        viewport.y    = 0;

        eventContext.pInterface->setViewport(viewport);
        eventContext.pInterface->update();
        eventContext.pGraphics->submitBuffers();
    }

    void EventHandler::callBuiltinEvent(Event event) {

        switch (event.category) {
            case Event::Category::eWindow: {
                
                switch (event.window) {
                    case Event::WindowType::eResize: {
                        resizeEvent(eventContext);
                        break;
                    }
                    default: break;
                }

                break;
            }
            default: break;
        }
    }
}