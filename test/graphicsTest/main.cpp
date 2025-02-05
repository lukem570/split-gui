#include <splitgui/window.hpp>
#include <splitgui/graphics.hpp>

int main() {

    SplitGui::EventHandler eventHandler;

    SplitGui::Window window;
    window.instanceGlfw();
    window.createWindow("graphics test");
    window.attachEventHandler(eventHandler);

    SplitGui::Graphics graphics;
    TRYRC(instanceRes, graphics.instanceVulkan(true));
    graphics.submitWindow(window);
    graphics.attachEventHandler(eventHandler);

    while (!window.shouldClose()) {
        while (eventHandler.popEvent()) {
            
        }  

        graphics.drawFrame();
        window.update();
    }

    return 0;
}
