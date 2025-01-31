#include <splitgui/result.hpp>
#include <splitgui/window.hpp>
#include <splitgui/interface.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/structs.hpp>

#include <fstream>
#include <sstream>
#include <string>

int main() {

    std::ifstream indexFile("test/xmlTest/index.xml");

    if (!indexFile.is_open()) {
        printf("ERROR: error opening index.xml\n");
        return -1;
    }

    std::stringstream buffer;
    buffer << indexFile.rdbuf();
    std::string page = buffer.str();

    SplitGui::EventHandler eventHandler;

    SplitGui::Window window;
    window.instanceGlfw();
    window.createWindow("xml test");
    window.attachEventHandler(eventHandler);

    SplitGui::Graphics graphics;
    TRYRC(graphics.instanceVulkan(true));
    graphics.submitWindow(window);
    graphics.attachEventHandler(eventHandler);

    SplitGui::RectObj viewport;
    viewport.size = window.getSize();
    viewport.x    = 0;
    viewport.y    = 0;
    
    SplitGui::Interface interface;
    TRYRC(interface.parseXml(page));
    interface.submitGraphics(graphics);
    interface.setViewport(viewport);
    interface.attachEventHandler(eventHandler);

    eventHandler.instanceBuiltinEvents();
    interface.instance();
    graphics.submitBuffers();

    while (!window.shouldClose()) {
        graphics.drawFrame();
        window.update();
    }
    
    return 0;
}
