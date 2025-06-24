#include <splitgui/result.hpp>
#include <splitgui/window.hpp>
#include <splitgui/interface.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/scene.hpp>

#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <future>

#define PI 3.14159

struct Data {
    SplitGui::EventHandler eventHandler;
    SplitGui::Window window;
    SplitGui::Graphics graphics;
    SplitGui::Interface ui;
    SplitGui::Default::SceneElement* scene;
};

double degToRad(double degrees) {
    return degrees * (PI / 180.0);
}

int frame(Data* data, bool* running) {
    SPLITGUI_PROFILE;
    SPLITGUI_NAME_THREAD("Frame thread");

    while (*running) {

        SplitGui::Result frameRes = data->graphics.drawFrame();

        if (frameRes != SplitGui::Result::eSuccess) {
            *running = false;
            return (int) frameRes;
        }
    }

    return 0;
}

int window(Data* data, bool* running) {
    SPLITGUI_PROFILE;
    SPLITGUI_NAME_THREAD("Update thread");

    while (*running) {

        while (data->eventHandler.popEvent()) {
            
        } 
        
        SplitGui::Mat4 projection = SplitGui::Mat4::perspectiveProjection(degToRad(90), data->scene->getExtent());
        data->graphics.updateSceneCameraProjection(data->scene->getSceneRef(), projection);

        SplitGui::Result projectionRes = data->graphics.submitSceneData(data->scene->getSceneRef());

        if (projectionRes != SplitGui::Result::eSuccess) {
            *running = false;
            return (int) projectionRes;
        }
        
        if (data->window.shouldClose()) {
            *running = false;
        }

        data->window.update();
    }

    return 0;
}

int main() {
    ma::Instance instance;

    SPLITGUI_PROFILE;

    std::ifstream indexFile("test/threadTest/index.xml");

    if (!indexFile.is_open()) {
        printf("ERROR: error opening index.xml\n");
        return -1;
    }

    Data data;

    std::stringstream buffer;
    buffer << indexFile.rdbuf();
    std::string page = buffer.str();

    SplitGui::WindowFlags flags;
    flags.title = "thread test";

    TRYRC(glfwRes, data.window.instanceGlfw());
    TRYRC(windowRes, data.window.createWindow(flags));
    data.window.attachEventHandler(data.eventHandler);

    SplitGui::VulkanFlags vulkanFlags;
    vulkanFlags.enableValidationLayers = true;
    
    TRYRC(instanceRes, data.graphics.instanceVulkan(vulkanFlags));
    TRYRC(winSubRes, data.graphics.submitWindow(data.window));
    data.graphics.attachEventHandler(data.eventHandler);

    SplitGui::RectObj viewport;
    viewport.size = data.window.getSize();
    viewport.x    = 0;
    viewport.y    = 0;
    
    TRYRC(parseRes, data.ui.parseXml(page));
    data.ui.submitGraphics(data.graphics);
    data.ui.setViewport(viewport);
    data.ui.attachEventHandler(data.eventHandler);

    SplitGui::Cube cube;
    cube.submitGraphics(data.graphics);
    cube.setSize(0.25);
    cube.setPosition({0, 0, 0});
    cube.setColor(SplitGui::HexColor(0xFF0000));
    cube.generate();

    data.scene = (SplitGui::Default::SceneElement*)data.ui.searchByReference("scene").back();

    TRYRC(uiInstRes, data.ui.instance());
    TRYRC(cubeRes, cube.submit(data.scene->getSceneRef()));
    TRYRC(submitRes, data.graphics.submitBuffers());

    bool running = true;


    SplitGui::Mat4 projection = SplitGui::Mat4::perspectiveProjection(degToRad(90), data.scene->getExtent());
    data.graphics.updateSceneCameraProjection(data.scene->getSceneRef(), projection);

    TRYRC(projectionRes, data.graphics.submitSceneData(data.scene->getSceneRef()));

    std::future<int> frameLoop = std::async(std::launch::async, frame, &data, &running);
    std::future<int> windowLoop = std::async(std::launch::async, window, &data, &running);

    int frameLoopRes = frameLoop.get();
    int windowLoopRes = windowLoop.get();

    if (frameLoopRes != 0) {
        return frameLoopRes;
    }

    if (windowLoopRes != 0) {
        return windowLoopRes;
    }
    
    return 0;
}
