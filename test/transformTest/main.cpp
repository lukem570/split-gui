#include <splitgui/result.hpp>
#include <splitgui/window.hpp>
#include <splitgui/interface.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/structs.hpp>

#include <fstream>
#include <sstream>
#include <string>

int main() {

    std::ifstream indexFile("test/transformTest/index.xml");

    if (!indexFile.is_open()) {
        printf("ERROR: error opening index.xml\n");
        return -1;
    }

    std::stringstream buffer;
    buffer << indexFile.rdbuf();
    std::string page = buffer.str();

    SplitGui::EventHandler eventHandler;

    SplitGui::WindowFlags flags;
    flags.title = "transform test";

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

    SplitGui::RectObj viewport;
    viewport.size = window.getSize();
    viewport.x    = 0;
    viewport.y    = 0;
    
    SplitGui::Interface ui;
    TRYRC(parseRes, ui.parseXml(page));
    ui.submitGraphics(graphics);
    ui.setViewport(viewport);
    ui.attachEventHandler(eventHandler);

    TRYRC(uiInstRes, ui.instance());
    TRYRC(submitRes, graphics.submitBuffers());

    while (!window.shouldClose()) {
        while (eventHandler.popEvent()) {
            
        }  

        TRYRC(frameRes, graphics.drawFrame());
        window.update();
    }
    
    return 0;
}
