#include <splitgui/window.hpp>
#include <splitgui/result.hpp>
#include <splitgui/interface.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/scene.hpp>
#include <splitgui/events.hpp>

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

    SplitGui::VulkanFlags vulkanFlags;
    vulkanFlags.clearColor             = 0xFFFFFF;
    vulkanFlags.enableValidationLayers = true;

    SplitGui::Graphics graphics;
    TRYRC(instanceRes, graphics.instanceVulkan(vulkanFlags));
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
    cube.setPosition({0, 0, 0});
    cube.setColor(SplitGui::HexColor(0xFF0000));
    cube.generate();

    SplitGui::Grid grid;
    grid.submitGraphics(graphics);

    SplitGui::Vec3 rotation = {0, 0, 0};
    SplitGui::Vec3 position = {0, 0, 0};

    ui.instance();
    cube.submit(0);
    grid.submit(0);
    graphics.submitBuffers();

    int prevXPos = 0;
    int prevYPos = 0;

    int xPos = 0;
    int yPos = 0;

    bool mouseDown = false;

    bool wDown = false;
    bool aDown = false;
    bool sDown = false;
    bool dDown = false;

    bool spaceDown = false;
    bool shiftDown = false;

    SplitGui::Mat4 projection = SplitGui::Mat4::perspectiveProjection(degToRad(90));
    TRYRC(projectionRes, graphics.updateSceneCameraProjection(0, projection));

    SplitGui::Camera cam;
    cam.submitGraphics(graphics);

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

                        case SplitGui::Event::WindowType::eKeypress:

                            switch (eventHandler.getEvent().data.window.keypress.keyCode)
                            {
                                case SplitGui::KeyCode::eW:

                                    if (eventHandler.getEvent().data.window.keypress.keyState == SplitGui::KeyState::ePress) {
                                        wDown = true;
                                    } else if (eventHandler.getEvent().data.window.keypress.keyState == SplitGui::KeyState::eRelease) {
                                        wDown = false;
                                    }

                                    break;
                                case SplitGui::KeyCode::eA:

                                    if (eventHandler.getEvent().data.window.keypress.keyState == SplitGui::KeyState::ePress) {
                                        aDown = true;
                                    } else if (eventHandler.getEvent().data.window.keypress.keyState == SplitGui::KeyState::eRelease) {
                                        aDown = false;
                                    }

                                    break;
                                case SplitGui::KeyCode::eS:

                                    if (eventHandler.getEvent().data.window.keypress.keyState == SplitGui::KeyState::ePress) {
                                        sDown = true;
                                    } else if (eventHandler.getEvent().data.window.keypress.keyState == SplitGui::KeyState::eRelease) {
                                        sDown = false;
                                    }

                                    break;
                                case SplitGui::KeyCode::eD:

                                    if (eventHandler.getEvent().data.window.keypress.keyState == SplitGui::KeyState::ePress) {
                                        dDown = true;
                                    } else if (eventHandler.getEvent().data.window.keypress.keyState == SplitGui::KeyState::eRelease) {
                                        dDown = false;
                                    }

                                    break;
                                case SplitGui::KeyCode::eSpace:

                                    if (eventHandler.getEvent().data.window.keypress.keyState == SplitGui::KeyState::ePress) {
                                        spaceDown = true;
                                    } else if (eventHandler.getEvent().data.window.keypress.keyState == SplitGui::KeyState::eRelease) {
                                        spaceDown = false;
                                    }

                                    break;
                                case SplitGui::KeyCode::eLeftShift:

                                    if (eventHandler.getEvent().data.window.keypress.keyState == SplitGui::KeyState::ePress) {
                                        shiftDown = true;
                                    } else if (eventHandler.getEvent().data.window.keypress.keyState == SplitGui::KeyState::eRelease) {
                                        shiftDown = false;
                                    }

                                    break;

                                default:break;
                            }


                            break;
                        
                        
                        default:break;
                    }
                    break;
                
                default:break;
            }
        }   

        const float moveSpeed   = 0.05f;
        const float sensitivity = 0.4f;

        if (mouseDown) {
            rotation.y -= degToRad(prevXPos - xPos) * sensitivity;
            rotation.x += degToRad(prevYPos - yPos) * sensitivity;
        }

        if (wDown) {
            position.z -= std::cos(rotation.y) * moveSpeed;
            position.x += std::sin(rotation.y) * moveSpeed;
        }

        if (aDown) {
            position.x -= std::cos(rotation.y) * moveSpeed;
            position.z -= std::sin(rotation.y) * moveSpeed;
        }

        if (sDown) {
            position.z += std::cos(rotation.y) * moveSpeed;
            position.x -= std::sin(rotation.y) * moveSpeed;
        }

        if (dDown) {
            position.x += std::cos(rotation.y) * moveSpeed;
            position.z += std::sin(rotation.y) * moveSpeed;
        }

        if (spaceDown) {
            position.y -= moveSpeed;
        }

        if (shiftDown) {
            position.y += moveSpeed;
        }

        prevXPos = xPos;
        prevYPos = yPos;

        cam.setRotation(rotation);
        cam.setPosition(position);

        TRYRC(updateRes, cam.update(0));

        graphics.drawFrame();
        window.update();
    }
    
    return 0;
}