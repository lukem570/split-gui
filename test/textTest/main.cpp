#include <splitgui/result.hpp>
#include <splitgui/window.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/structs.hpp>

int main() {
    ma::Instance instance;

    SPLITGUI_PROFILE;

    SplitGui::EventHandler eventHandler;

    SplitGui::WindowFlags flags;
    flags.title = "text test";

    SplitGui::Window window;
    TRYRC(glfwRes, window.instanceGlfw());
    TRYRC(windowRes, window.createWindow(flags));
    window.attachEventHandler(eventHandler);

    SplitGui::VulkanFlags vulkanFlags;
    vulkanFlags.enableValidationLayers = true;
    vulkanFlags.clearColor = 0xFFFFFF;
    vulkanFlags.mdsfTextureSize = 128;

    SplitGui::Graphics graphics;
    TRYRC(instanceRes, graphics.instanceVulkan(vulkanFlags));
    TRYRC(winSubRes, graphics.submitWindow(window));
    TRYRC(fontRes, graphics.loadFont("fonts/roboto/Roboto-Regular.ttf"));
    graphics.attachEventHandler(eventHandler);

    std::string text = "Hello, world\nThis is a newline\nHi\nbearing test cpcpcp\njijj";

    TRYC(SplitGui::TextRef, textRes, graphics.drawText(
        SplitGui::IVec2{0, 0},
        text,
        0x000000,
        40.0
    ));

    TRYC(SplitGui::IVec2, sizeRes, graphics.getTextSize(text, 40.0));

    graphics.drawRect({0, 0}, sizeRes.value, 0xFF0000);

    TRYRC(submitRes, graphics.submitBuffers());

    while (!window.shouldClose()) {
        while (eventHandler.popEvent()) {

        }  

        TRYRC(frameRes, graphics.drawFrame());
        window.update();
    }

    return 0;
}