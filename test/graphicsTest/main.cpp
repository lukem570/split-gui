#include <splitgui/window.hpp>
#include <splitgui/graphics.hpp>

int main() {
    ma::Instance instance;

    SPLITGUI_PROFILE;

    SplitGui::EventHandler eventHandler;

    SplitGui::WindowFlags flags;
    flags.title = "graphics test";

    SplitGui::Window window;
    TRYRC(glfwRes, window.instanceGlfw());
    TRYRC(windowRes, window.createWindow(flags));
    window.attachEventHandler(eventHandler);

    SplitGui::VulkanFlags vulkanFlags;
    vulkanFlags.enableValidationLayers = true;

    SplitGui::Graphics graphics;
    TRYRC(instanceRes, graphics.instanceVulkan(vulkanFlags));
    TRYRC(winSubRes, graphics.submitWindow(window));
    graphics.attachEventHandler(eventHandler);

    while (!window.shouldClose()) {
        while (eventHandler.popEvent()) {
            
        }  

        TRYRC(frameRes, graphics.drawFrame());
        window.update();
    }

    return 0;
}
