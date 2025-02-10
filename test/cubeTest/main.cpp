#include <splitgui/window.hpp>
#include <splitgui/result.hpp>
#include <splitgui/interface.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/scene.hpp>

#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

double degToRad(double degrees) {
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
    TRYRC(glfwRes, window.instanceGlfw());
    TRYRC(windowRes, window.createWindow("cube test"));
    window.attachEventHandler(eventHandler);

    SplitGui::Graphics graphics;
    TRYRC(instanceRes, graphics.instanceVulkan(true));
    graphics.submitWindow(window);
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

    SplitGui::Cube cube;
    cube.submitGraphics(graphics);
    cube.setSize(0.25);
    cube.setColor(SplitGui::HexColor(0xFF0000));
    cube.generate();

    SplitGui::Vec3 rotation = {0, 0, 0};
    SplitGui::Vec3 position = {0, 0, 0};

    ui.instance();
    cube.submit(0);
    graphics.submitBuffers();

    graphics.updateSceneCameraPosition(0, position);

    int prevXPos = 0;
    int prevYPos = 0;

    int xPos = 0;
    int yPos = 0;

    bool mouseDown = false;

    while (!window.shouldClose()) {
        while (eventHandler.popEvent()) {
            switch (eventHandler.getEvent().category) {

                case SplitGui::Event::Category::eWindow:

                    switch (eventHandler.getEvent().window) {

                        case SplitGui::Event::WindowType::eMouseMove:
                            xPos = eventHandler.getEvent().data.window.mouseMove.xPos;
                            yPos = eventHandler.getEvent().data.window.mouseMove.yPos;

                            break;
                        case SplitGui::Event::WindowType::eMouseButton:

                            if (eventHandler.getEvent().data.window.mouseButton.mouseCode == SplitGui::MouseCode::eOne) {

                                if (eventHandler.getEvent().data.window.mouseButton.mouseState == SplitGui::MouseState::ePress) {

                                    mouseDown = true;

                                } else if (eventHandler.getEvent().data.window.mouseButton.mouseState == SplitGui::MouseState::eRelease) {

                                    mouseDown = false;

                                }
                            }
                            break;
                        
                        default:break;
                    }
                    break;
                
                default:break;
            }
        }   

        if (mouseDown) {
            rotation.y -= degToRad(prevXPos - xPos);
            rotation.x -= degToRad(prevYPos - yPos);
            rotation.z -= degToRad(prevYPos - yPos);
        }

        prevXPos = xPos;
        prevYPos = yPos;

        SplitGui::Mat3 rotMat = SplitGui::Mat3::eulerRotationMatrix(rotation);
        graphics.updateSceneCameraRotation(0, rotMat);

        graphics.drawFrame();
        window.update();
    }
    
    return 0;
}