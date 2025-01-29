#include <splitgui/events.hpp>
#include <splitgui/graphics.hpp>

namespace SplitGui {
    static void resizeEvent(Context eventContext) {
        eventContext.pGraphics->resizeEvent();
    }

    EventHandler::EventHandler() {
        bindFunction(resizeEvent, "builtin-resize", EventAttachment(EventAttachment::Category::eWindow, EventAttachment::WindowType::eResize));
    }
}