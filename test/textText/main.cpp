#include <splitgui/result.hpp>
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
    TRYRC(instanceRes, graphics.instanceVulkan(true));
    graphics.submitWindow(window);
    graphics.loadFont("fonts/Lato/Lato-Regular.ttf");
    graphics.attachEventHandler(eventHandler);

    graphics.drawRect(
        SplitGui::IVec2{0, 0}, 
        window.getSize(),
        0xFFFFFF
    );

    TRYC(int, textRes, graphics.drawText(
        SplitGui::IVec2{0, 0},
        "abcdefghijklmnopqrstuvwxyz"
    ));

    eventHandler.instanceBuiltinEvents();
    graphics.submitBuffers();

    while (!window.shouldClose()) {
        graphics.drawFrame();
        window.update();
    }

    return 0;
}