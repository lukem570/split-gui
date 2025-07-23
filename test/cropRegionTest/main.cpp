#include <splitgui/window.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/structs.hpp>

int main() {
    ma::Instance instance;

    SPLITGUI_PROFILE;

    SplitGui::EventHandler eventHandler;

    SplitGui::WindowFlags flags;
    flags.title = "crop test";

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

    SplitGui::IVec2 windowSize = window.getSize();

    SplitGui::ResultValue<SplitGui::CropRegionRef> region = graphics.createCropRegion({0, 0}, {windowSize.x / 2, windowSize.y});
    TRYDC(region);

    graphics.drawRect(
        SplitGui::IVec2{0, 0}, 
        windowSize,
        0xFF00FF,
        0,
        SplitGui::VertexFlagsBits::eCropEnable,
        0,
        region.value
    );

    TRYRC(submitRes, graphics.submitBuffers());

    while (!window.shouldClose()) {
        while (eventHandler.popEvent()) {
            if (eventHandler.getEvent().category == SplitGui::Event::Category::eWindow && eventHandler.getEvent().window == SplitGui::Event::WindowType::eResize) {

                windowSize = window.getSize();
                TRYRC(cropRes, graphics.updateCropRegion(region.value, {0, 0}, {windowSize.x / 2, windowSize.y}));
            }
        }  

        TRYRC(frameRes, graphics.drawFrame());
        window.update();
    }

    return 0;
}