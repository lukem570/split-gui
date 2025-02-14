#include <splitgui/window.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/structs.hpp>

int main() {

    SplitGui::EventHandler eventHandler;

    SplitGui::Window window;
    TRYRC(glfwRes, window.instanceGlfw());
    TRYRC(windowRes, window.createWindow("quad test"));
    window.attachEventHandler(eventHandler);

    SplitGui::VulkanFlags vulkanFlags;
    vulkanFlags.enableValidationLayers = true;

    SplitGui::Graphics graphics;
    TRYRC(instanceRes, graphics.instanceVulkan(vulkanFlags));
    graphics.submitWindow(window);
    graphics.attachEventHandler(eventHandler);

    graphics.drawRect(
        SplitGui::IVec2{0, 0}, 
        window.getSize(),
        0x222938
    );

    graphics.submitBuffers();

    while (!window.shouldClose()) {
        while (eventHandler.popEvent()) {
            
        }  

        graphics.drawFrame();
        window.update();
    }

    return 0;
}