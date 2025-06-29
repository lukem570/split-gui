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
    ma::Instance instance;

    SPLITGUI_PROFILE;

    std::ifstream indexFile("test/sceneTest/index.xml");

    if (!indexFile.is_open()) {
        printf("ERROR: error opening index.xml\n");
        return -1;
    }

    std::stringstream buffer;
    buffer << indexFile.rdbuf();
    std::string page = buffer.str();

    SplitGui::EventHandler eventHandler;

    SplitGui::WindowFlags flags;
    flags.title = "scene test";

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

    SplitGui::Default::SceneElement* scene = (SplitGui::Default::SceneElement*)ui.searchByReference("scene").back();
    SplitGui::SceneRef sceneRef = scene->getSceneRef();
    
    TRYRC(uiInstRes, ui.instance());

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
    std::vector<uint16_t> indices          = {0, 1, 2};

    SplitGui::ResultValue<SplitGui::ModelRef> model = graphics.createModel(sceneRef, SplitGui::Mat4::ident());
    TRYDC(model);
    SplitGui::ResultValue<SplitGui::TriangleRef> triangleRes = graphics.submitTriangleData(sceneRef, vertices, indices, model.value, 0);

    TRYDC(triangleRes);
    
    TRYRC(submitRes, graphics.submitBuffers());
    
    SplitGui::Vec3 rotation = {0, 0, 0};

    SplitGui::Mat4 projection = SplitGui::Mat4::orthographicProjection();
    graphics.updateSceneCameraProjection(sceneRef, projection);

    TRYRC(projectionRes, graphics.submitSceneData(sceneRef));

    SplitGui::Camera cam;
    cam.submitGraphics(graphics);

    while (!window.shouldClose()) {
        while (eventHandler.popEvent()) {
            
        }  
        
        rotation.y += degToRad(2);

        cam.setRotation(rotation);
        TRYRC(updateRes, cam.update(sceneRef));

        TRYRC(frameRes, graphics.drawFrame());
        window.update();
    }
    
    return 0;
}
