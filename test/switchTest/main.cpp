#include <splitgui/result.hpp>
#include <splitgui/window.hpp>
#include <splitgui/interface.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/logger.hpp>

#include <fstream>
#include <sstream>
#include <string>
#include <chrono>

int main() {

    SPLITGUI_PROFILE;

    std::ifstream indexFile("test/switchTest/index.xml");

    if (!indexFile.is_open()) {
        printf("ERROR: error opening index.xml\n");
        return -1;
    }

    std::stringstream buffer;
    buffer << indexFile.rdbuf();
    std::string page = buffer.str();

    SplitGui::EventHandler eventHandler;

    SplitGui::WindowFlags flags;
    flags.title = "switch test";

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

    auto lastLogic = std::chrono::steady_clock::now();

    SplitGui::Default::Switch* swi = (SplitGui::Default::Switch*)ui.searchByReference("switch").back();

    unsigned int active = 0;

    while (!window.shouldClose()) {
        while (eventHandler.popEvent()) {
            
        }  

        auto now = std::chrono::steady_clock::now();

        if (std::chrono::duration_cast<std::chrono::seconds>(now - lastLogic).count() >= 1) {
            
            swi->setActive(active);

            active = (active + 1) % 3;

            TRYRC(switchRes, swi->update());
            TRYRC(submitRes, graphics.submitBuffers());

            lastLogic = now;
        }

        TRYRC(frameRes, graphics.drawFrame());
        window.update();
    }
    
    return 0;
}
