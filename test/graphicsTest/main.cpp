#include <splitgui/window.hpp>
#include <splitgui/graphics.hpp>

int main() {
    SplitGui::Window window;
    window.instanceGlfw();
    window.createWindow("graphics test");


    SplitGui::Graphics graphics;
    graphics.instanceVulkan(false);
    graphics.submitWindow(window);


    while (!window.shouldClose()) {
        graphics.drawFrame();
        window.update();
    }

    return 0;
}
