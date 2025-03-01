#include <splitgui/result.hpp>
#include <splitgui/window.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/structs.hpp>

int main() {

    SplitGui::EventHandler eventHandler;

    SplitGui::WindowFlags flags;
    flags.title = "text test";

    SplitGui::Window window;
    TRYRC(glfwRes, window.instanceGlfw());
    TRYRC(windowRes, window.createWindow(flags));
    window.attachEventHandler(eventHandler);

    SplitGui::VulkanFlags vulkanFlags;
    vulkanFlags.enableValidationLayers = true;

    SplitGui::Graphics graphics;
    TRYRC(instanceRes, graphics.instanceVulkan(vulkanFlags));
    graphics.submitWindow(window);
    TRYRC(fontRes, graphics.loadFont("fonts/roboto/Roboto-Regular.ttf"));
    graphics.attachEventHandler(eventHandler);

    graphics.drawRect(
        SplitGui::IVec2{0, 0}, 
        window.getSize(),
        0xFFFFFF,
        1
    );

    TRYC(SplitGui::TextRef, textRes, graphics.drawText(
        SplitGui::IVec2{0, 0},
        "Hello, world\nThis is a newline\nHi",
        0x000000,
        20
    ));

    graphics.submitBuffers();

    while (!window.shouldClose()) {
        while (eventHandler.popEvent()) {
            
        }  

        graphics.drawFrame();
        window.update();
    }

    return 0;
}