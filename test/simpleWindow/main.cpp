#define DEV

#include <splitgui/window.hpp>

int main() {
    SplitGui::Window window;
    window.createWindow("simple window");

    window.__devLoop();
    return 0;
}
