#include <splitgui/window.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/structs.hpp>

int main() {

    SPLITGUI_PROFILE;

    SplitGui::EventHandler eventHandler;

    SplitGui::WindowFlags flags;
    flags.title = "quad test";

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

    graphics.drawRect(
        SplitGui::IVec2{0, 0}, 
        window.getSize(),
        0x222938
    );

    TRYRC(submitRes, graphics.submitBuffers());

    while (!window.shouldClose()) {
        while (eventHandler.popEvent()) {
            
        }  

        TRYRC(frameRes, graphics.drawFrame());
        window.update();
    }

    return 0;
}