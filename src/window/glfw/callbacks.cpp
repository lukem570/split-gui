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
            fflush(stdout);
        }

        pEvent->call(pEventHandler->getContext());
        
        printf("Window resized to %d x %d\n", width, height);
    }
}