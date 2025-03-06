#include <splitgui/structs.hpp>
#include <splitgui/events.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/interface.hpp>
#include <splitgui/window.hpp>

namespace SplitGui {
    Result resizeEvent(Context eventContext) {
        if (!eventContext.pGraphics) {
            return Result::eSuccess;
        }

        TRYR(resizeRes, eventContext.pGraphics->resizeEvent());

        if (!eventContext.pInterface) {
            return Result::eSuccess;
        }
        
        // TODO: fix for not full window viewport sizes
        RectObj viewport;
        viewport.size = eventContext.pWindow->getSize();
        viewport.x    = 0;
        viewport.y    = 0;

        eventContext.pInterface->setViewport(viewport);
        eventContext.pInterface->update();
        TRYR(submitRes, eventContext.pGraphics->submitBuffers());

        return Result::eSuccess;
    }

    Result EventHandler::callBuiltinEvent(Event event) {

        switch (event.category) {
            case Event::Category::eWindow: {
                
                switch (event.window) {
                    case Event::WindowType::eResize: {
                        TRYR(resizeRes, resizeEvent(eventContext));
                        break;
                    }
                    default: break;
                }

                break;
            }
            default: break;
        }

        return Result::eSuccess;
    }
}