#define DEV

#include <splitgui/window.hpp>

int main() {
    SplitGui::Window window;
    window.createWindow("graphics test");

    SplitGui::Graphics graphics;
    graphics.instanceVulkan();
    graphics.submitWindow(window);

    window.__devLoop();

    return 0;
}
