#include <splitgui/window.hpp>

int main() {
    SplitGui::Window window;
    window.instanceGlfw();
    window.createWindow("simple window");

    window.update();
    return 0;
}
