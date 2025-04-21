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

    SPLITGUI_PROFILE;

    std::ifstream indexFile("test/multiSceneTest/index.xml");

    if (!indexFile.is_open()) {
        printf("ERROR: error opening index.xml\n");
        return -1;
    }

    std::stringstream buffer;
    buffer << indexFile.rdbuf();
    std::string page = buffer.str();

    SplitGui::EventHandler eventHandler;

    SplitGui::WindowFlags flags;
    flags.title = "multi scene test";

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
    viewport.size   = window.getSize();
    viewport.x      = 0;
    viewport.y      = 0;
    
    SplitGui::Interface ui;
    TRYRC(interfaceRes, ui.parseXml(page));
    ui.submitGraphics(graphics);
    ui.setViewport(viewport);
    ui.attachEventHandler(eventHandler);

    TRYRC(uiInstRes, ui.instance());
    TRYRC(submitRes, graphics.submitBuffers());

    SplitGui::Default::SceneElement* scene1 = (SplitGui::Default::SceneElement*)ui.searchByReference("scene-1").back();
    SplitGui::Default::SceneElement* scene2 = (SplitGui::Default::SceneElement*)ui.searchByReference("scene-2").back();
    SplitGui::SceneRef sceneRef1 = scene1->getSceneRef();
    SplitGui::SceneRef sceneRef2 = scene2->getSceneRef();

    SplitGui::Vertex vert1;
    vert1.color = SplitGui::HexColor(0xFF0000).normalize();
    vert1.pos = {0.0, -0.5, 0};

    SplitGui::Vertex vert2;
    vert2.color = SplitGui::HexColor(0x00FF00).normalize();
    vert2.pos = {0.5, 0.5, 0};

    SplitGui::Vertex vert3;
    vert3.color = SplitGui::HexColor(0x0000FF).normalize();
    vert3.pos = {-0.5, 0.5, 0};

    std::vector<SplitGui::Vertex> vertices = {vert1, vert2, vert3};
    std::vector<uint16_t> indices          = {0, 1, 2, 1, 0, 2};

    SplitGui::ResultValue<SplitGui::TriangleRef> triangleRes1 = graphics.submitTriangleData(sceneRef1, vertices, indices, 0);

    TRYDC(triangleRes1);

    SplitGui::ResultValue<SplitGui::TriangleRef> triangleRes2 = graphics.submitTriangleData(sceneRef2, vertices, indices, 0);

    TRYDC(triangleRes2);

    SplitGui::Vec3 rotation1 = {degToRad(5), 0, 0};
    SplitGui::Vec3 rotation2 = {degToRad(5), 0, 0};

    SplitGui::Mat4 projection1 = SplitGui::Mat4::orthographicProjection();
    SplitGui::Mat4 projection2 = SplitGui::Mat4::orthographicProjection();

    graphics.updateSceneCameraProjection(sceneRef1, projection1);
    graphics.updateSceneCameraProjection(sceneRef2, projection2);

    TRYRC(projectionRes1, graphics.submitSceneData(sceneRef1));
    TRYRC(projectionRes2, graphics.submitSceneData(sceneRef2));

    SplitGui::Camera cam1;
    SplitGui::Camera cam2;

    cam1.submitGraphics(graphics);
    cam2.submitGraphics(graphics);

    while (!window.shouldClose()) {
        while (eventHandler.popEvent()) {
            
        }  
        
        rotation1.y += degToRad(3);
        rotation2.x += degToRad(3);

        cam1.setRotation(rotation1);
        cam2.setRotation(rotation2);

        TRYRC(updateRes1, cam1.update(sceneRef1));
        TRYRC(updateRes2, cam2.update(sceneRef2));

        TRYRC(frameRes, graphics.drawFrame());
        window.update();
    }
    
    return 0;
}
