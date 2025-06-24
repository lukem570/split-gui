#include <splitgui/window.hpp>
#include <splitgui/result.hpp>

int main() {
    ma::Instance instance;

    SPLITGUI_PROFILE;

    SplitGui::EventHandler eventHandler;

    SplitGui::WindowFlags flags;
    flags.title = "simple window";

    SplitGui::Window window;
    TRYRC(glfwRes, window.instanceGlfw());
    TRYRC(windowRes, window.createWindow(flags));
    window.attachEventHandler(eventHandler);

    while (!window.shouldClose()) {

        window.update();
    }
    
    return 0;
}
