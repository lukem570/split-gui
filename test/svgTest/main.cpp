#include <splitgui/window.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/structs.hpp>

#include <vector>
#include <sstream>
#include <fstream>

int main() {
    ma::Instance instance;

    SPLITGUI_PROFILE;

    std::ifstream indexFile("test/svgTest/background.svg");

    if (!indexFile.is_open()) {
        printf("ERROR: error opening index.xml\n");
        return -1;
    }

    std::stringstream buffer;
    buffer << indexFile.rdbuf();
    std::string svg = buffer.str();

    SplitGui::EventHandler eventHandler;

    SplitGui::WindowFlags flags;
    flags.title = "svg test";

    SplitGui::Window window;
    TRYRC(glfwRes, window.instanceGlfw());
    TRYRC(windowRes, window.createWindow(flags));
    window.attachEventHandler(eventHandler);

    SplitGui::VulkanFlags vulkanFlags;
    vulkanFlags.mdsfTextureSize = 512;
    vulkanFlags.enableValidationLayers = true;

    SplitGui::Graphics graphics;
    TRYRC(instanceRes, graphics.instanceVulkan(vulkanFlags));
    TRYRC(winSubRes, graphics.submitWindow(window));
    graphics.attachEventHandler(eventHandler);

    SplitGui::ResultValue<SplitGui::TextureRef> texture = graphics.rasterizeSvg(svg);

    TRYDC(texture);  

    graphics.drawRect(
        SplitGui::IVec2{0, 0}, 
        window.getSize(),
        0xFFFFFF,
        0,
        SplitGui::VertexFlagsBits::eUseTexture,
        texture.value.textureNumber
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