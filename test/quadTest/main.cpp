#include <splitgui/window.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/structs.hpp>

int main() {
    SplitGui::Window window;
    window.instanceGlfw();
    window.createWindow("quad test");

    SplitGui::Graphics graphics;
    graphics.instanceVulkan(true);
    graphics.submitWindow(window);

    graphics.drawRect(
        SplitGui::IVec2{0, 0}, 
        window.getSize(),
        0x222938
    );

    graphics.submitBuffers();

    while (!window.shouldClose()) {
        graphics.drawFrame();
        window.update();
    }

    return 0;
}