#include <splitgui/window.hpp>

int main() {

    SplitGui::EventHandler eventHandler;

    SplitGui::Window window;
    window.instanceGlfw();
    window.createWindow("simple window");
    window.attachEventHandler(eventHandler);

    window.update();
    return 0;
}
