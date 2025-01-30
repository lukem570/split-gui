#include <splitgui/window.hpp>
#include <splitgui/graphics.hpp>

int main() {

    SplitGui::EventHandler eventHandler;

    SplitGui::Window window;
    window.instanceGlfw();
    window.createWindow("graphics test");
    window.attachEventHandler(eventHandler);

    SplitGui::Graphics graphics;
    graphics.instanceVulkan(true);
    graphics.submitWindow(window);
    graphics.attachEventHandler(eventHandler);

    eventHandler.instanceBuiltinEvents();

    while (!window.shouldClose()) {
        graphics.drawFrame();
        window.update();
    }

    return 0;
}
