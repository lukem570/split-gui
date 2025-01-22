#include <splitgui/window.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/structs.hpp>

int main() {
    SplitGui::Window window;
    window.instanceGlfw();
    window.createWindow("text test");

    SplitGui::Graphics graphics;
    graphics.instanceVulkan(true);
    graphics.submitWindow(window);
    graphics.loadFont("fonts/Lato/Lato-Bold.ttf");

    graphics.drawText(
        SplitGui::IVec2{0, 0}, 
        window.getSize(),
        "A"
    );

    graphics.submitBuffers();

    while (!window.shouldClose()) {
        graphics.drawFrame();
        window.update();
    }

    return 0;
}