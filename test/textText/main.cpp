#include <splitgui/window.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/structs.hpp>

int main() {

    SplitGui::EventHandler eventHandler;

    SplitGui::Window window;
    window.instanceGlfw();
    window.createWindow("text test");
    window.attachEventHandler(eventHandler);

    SplitGui::Graphics graphics;
    graphics.instanceVulkan(true);
    graphics.submitWindow(window);
    graphics.loadFont("fonts/Lato/Lato-Regular.ttf");
    graphics.attachEventHandler(eventHandler);

    graphics.drawRect(
        SplitGui::IVec2{0, 0}, 
        window.getSize(),
        0xFFFFFF
    );

    graphics.drawText(
        SplitGui::IVec2{0, 0},
        "abcdefghijklmnopqrstuvwxyz"
    );

    graphics.submitBuffers();

    while (!window.shouldClose()) {
        graphics.drawFrame();
        window.update();
    }

    return 0;
}