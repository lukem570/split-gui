#include <splitgui/window.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/structs.hpp>

int main() {

    SplitGui::EventHandler eventHandler;

    SplitGui::Window window;
    window.instanceGlfw();
    window.createWindow("quad test");
    window.attachEventHandler(eventHandler);

    SplitGui::Graphics graphics;
    TRYRC(graphics.instanceVulkan(true));
    graphics.submitWindow(window);
    graphics.attachEventHandler(eventHandler);

    graphics.drawRect(
        SplitGui::IVec2{0, 0}, 
        window.getSize(),
        0x222938
    );

    eventHandler.instanceBuiltinEvents();
    graphics.submitBuffers();

    while (!window.shouldClose()) {
        graphics.drawFrame();
        window.update();
    }

    return 0;
}