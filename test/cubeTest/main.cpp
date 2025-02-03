#include <splitgui/window.hpp>
#include <splitgui/interface.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/scene.hpp>

#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

constexpr double degToRad(double degrees) {
    return degrees * (M_PI / 180.0);
}

int main() {

    std::ifstream indexFile("test/cubeTest/index.xml");

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
    window.createWindow("cube test");
    window.attachEventHandler(eventHandler);

    SplitGui::Graphics graphics;
    TRYRC(instanceRes, graphics.instanceVulkan(true));
    graphics.submitWindow(window);
    graphics.attachEventHandler(eventHandler);

    SplitGui::RectObj viewport;
    viewport.size   = window.getSize();
    viewport.x      = 0;
    viewport.y      = 0;
    
    SplitGui::Interface interface;
    TRYRC(interfaceRes, interface.parseXml(page));
    interface.submitGraphics(graphics);
    interface.setViewport(viewport);
    interface.attachEventHandler(eventHandler);

    SplitGui::Cube cube;
    cube.submitGraphics(graphics);
    cube.setSize(0.25);
    cube.setColor(0xFF00FF);
    cube.generate();

    cube.submit(0);

    SplitGui::Vec3 rotation;

    eventHandler.instanceBuiltinEvents();
    interface.instance();
    graphics.submitBuffers();

    while (!window.shouldClose()) {

        rotation.x += degToRad(2);

        graphics.updateSceneCameraRotation(0, rotation);

        graphics.drawFrame();
        window.update();
    }
    
    return 0;
}