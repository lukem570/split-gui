#define DEV

#include <splitgui/window.hpp>

int main() {
    SplitGui::Window window;
    window.instanceGlfw();
    window.createWindow("simple window");

    window.__devLoop();
    return 0;
}
