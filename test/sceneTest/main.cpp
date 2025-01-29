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

    std::ifstream indexFile("test/sceneTest/index.xml");

    if (!indexFile.is_open()) {
        printf("ERROR: error opening index.xml\n");
        return -1;
    }

    std::stringstream buffer;
    buffer << indexFile.rdbuf();
    std::string page = buffer.str();

    SplitGui::Window window;
    window.instanceGlfw();
    window.createWindow("scene test");

    SplitGui::Graphics graphics;
    graphics.instanceVulkan(true);
    graphics.submitWindow(window);

    SplitGui::RectObj viewport;
    viewport.size   = window.getSize();
    //viewport.width /= 2;
    viewport.x      = 0;
    viewport.y      = 0;
    
    SplitGui::Interface interface;
    interface.parseXml(page);
    interface.submitGraphics(graphics);
    interface.setViewport(viewport);

    SplitGui::Vertex vert1;
    vert1.color = SplitGui::HexColor(0xFF0000).normalize();
    vert1.pos = {0.0, -0.5};

    SplitGui::Vertex vert2;
    vert2.color = SplitGui::HexColor(0x00FF00).normalize();
    vert2.pos = {0.5, 0.5};

    SplitGui::Vertex vert3;
    vert3.color = SplitGui::HexColor(0x0000FF).normalize();
    vert3.pos = {-0.5, 0.5};

    std::vector<SplitGui::Vertex> vertices = {vert1, vert2, vert3};
    std::vector<uint16_t> indices          = {0, 1, 2};

    graphics.submitTriangleData(0, vertices, indices);

    interface.update();
    graphics.submitBuffers();

    SplitGui::Vec3 rotation = {0, 0, 0};

    while (!window.shouldClose()) {

        
        rotation.z += degToRad(2);
        graphics.updateSceneCameraRotation(0, rotation);

        graphics.drawFrame();
        window.update();
    }
    
    return 0;
}
