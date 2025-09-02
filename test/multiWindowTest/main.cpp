#include <splitgui/window.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/structs.hpp>

int main() {
    ma::Instance instance;

    SPLITGUI_PROFILE;

    SplitGui::EventHandler eventHandler1;
    SplitGui::EventHandler eventHandler2;

    SplitGui::WindowFlags flags;
    flags.title = "multi window test";

    SplitGui::Window window1;
    TRYRC(glfw1Res, window1.instanceGlfw());
    TRYRC(window1Res, window1.createWindow(flags));
    window1.attachEventHandler(eventHandler1);

    SplitGui::Window window2;
    TRYRC(glfw2Res, window2.instanceGlfw());
    TRYRC(window2Res, window2.createWindow(flags));
    window2.attachEventHandler(eventHandler2);

    SplitGui::VulkanFlags vulkanFlags;
    vulkanFlags.enableValidationLayers = true;

    SplitGui::Graphics graphics1;
    TRYRC(instance1Res, graphics1.instanceVulkan(vulkanFlags));
    TRYRC(winSub1Res, graphics1.submitWindow(window1));
    graphics1.attachEventHandler(eventHandler1);

    SplitGui::Graphics graphics2;
    TRYRC(instance2Res, graphics2.instanceVulkan(vulkanFlags));
    TRYRC(winSub2Res, graphics2.submitWindow(window2));
    graphics2.attachEventHandler(eventHandler2);

    graphics1.drawRect(
        SplitGui::IVec2{0, 0}, 
        window1.getSize(),
        0xFF00FF
    );

    graphics2.drawRect(
        SplitGui::IVec2{0, 0}, 
        window2.getSize(),
        0x00FFFF
    );

    TRYRC(submit1Res, graphics1.submitBuffers());
    TRYRC(submit2Res, graphics2.submitBuffers());

    while (!window1.shouldClose() && !window2.shouldClose()) {
        while (eventHandler1.popEvent()) {
            
        }  

        while (eventHandler2.popEvent()) {
            
        }  

        TRYRC(frame1Res, graphics1.drawFrame());
        TRYRC(frame2Res, graphics2.drawFrame());
        window1.update();
        window2.update();
    }

    if (window1.shouldClose()) {
        window2.close();
    }

    if (window2.shouldClose()) {
        window1.close();
    }

    return 0;
}