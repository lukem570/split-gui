#include <splitgui/window.hpp>
#include <splitgui/result.hpp>

int main() {

    SplitGui::EventHandler eventHandler;

    SplitGui::Window window;
    TRYRC(glfwRes, window.instanceGlfw());
    TRYRC(windowRes, window.createWindow("simple window"));
    window.attachEventHandler(eventHandler);

    window.update();
    return 0;
}
