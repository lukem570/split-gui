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
        SplitGui::Vec2{ 1.0f,  1.0f}, 
        SplitGui::Vec2{-1.0f, -1.0f},
        {0.211764f, 0.270588f, 0.309803f}
    );

    graphics.submitBuffers();

    while (!window.shouldClose()) {
        graphics.drawFrame();
        window.update();
    }

    return 0;
}