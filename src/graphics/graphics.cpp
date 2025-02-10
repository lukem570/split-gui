#include <splitgui/result.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/window.hpp>
#include "vulkan/vulkan.cpp"

namespace SplitGui {

    Graphics::Graphics() {
        
    }

    Graphics::~Graphics() {
        if (pInterface) {
            delete pInterface;
        }
    }
    
    Result Graphics::instanceVulkan(bool validation = false) {
        if (!glfw::vulkanSupported()) {
            return Result::eVulkanNotSupported;
        }

        pInterface = (GraphicsLibInterface*) new(std::nothrow) VulkanInterface(validation);
        if (!pInterface) {
            return Result::eHeapAllocFailed;
        }

        pInterface->instance();

        return Result::eSuccess;
    }

    void Graphics::submitWindow(Window& window) {
        pWindow = &window;
        pInterface->submitWindow(window);
    }

    void Graphics::drawFrame() {
        pInterface->drawFrame();
    }

    RectRef Graphics::drawRect(IVec2 x1, IVec2 x2, HexColor color) {

        IVec2 windowSize = pWindow->getSize();

        Vec2 newX1;
        newX1.x = (float)x1.x / windowSize.x * 2.0 - 1.0f;
        newX1.y = (float)x1.y / windowSize.y * 2.0 - 1.0f;

        Vec2 newX2;
        newX2.x = (float)x2.x / windowSize.x * 2.0 - 1.0f;
        newX2.y = (float)x2.y / windowSize.y * 2.0 - 1.0f;

        return pInterface->drawRect(newX1, newX2, color.normalize());
    }

    void Graphics::updateRect(RectRef& ref, IVec2 x1, IVec2 x2) {
        IVec2 windowSize = pWindow->getSize();

        Vec2 newX1;
        newX1.x = (float)x1.x / (float)windowSize.x * 2.0f - 1.0f;
        newX1.y = (float)x1.y / (float)windowSize.y * 2.0f - 1.0f;

        Vec2 newX2;
        newX2.x = (float)x2.x / (float)windowSize.x * 2.0f - 1.0f;
        newX2.y = (float)x2.y / (float)windowSize.y * 2.0f - 1.0f;

        pInterface->updateRect(ref, newX1, newX2);
    }

    void Graphics::submitBuffers() {
        pInterface->submitBuffers();
    }

    unsigned int Graphics::instanceScene(IVec2 x1, IVec2 x2) {
        return pInterface->instanceScene(x1, x2);
    }

    Result Graphics::resizeEvent() {
        TRYR(resizeRes, pInterface->resizeEvent());
        return Result::eSuccess;
    }

    ResultValue<int> Graphics::drawText(IVec2 x1, std::string text) {
        IVec2 windowSize = pWindow->getSize();

        Vec2 newX1;
        newX1.x = (float)x1.x / windowSize.x * 2.0 - 1.0f;
        newX1.y = (float)x1.y / windowSize.y * 2.0 - 1.0f;

        ResultValue<float> ret = pInterface->drawText(newX1, text);

        TRYD(ret);

        return ret.value * windowSize.x;
    }

    Result Graphics::loadFont(const char* path) {
        return pInterface->loadFont(path);
    }

    void Graphics::submitTriangleData(unsigned int sceneNumber, std::vector<Vertex>& vertices, std::vector<uint16_t>& indices) {
        pInterface->submitTriangleData(sceneNumber, vertices, indices);
    }

    void Graphics::updateSceneCameraRotation(unsigned int sceneNumber, Mat3& rotation) {
        pInterface->updateSceneCameraRotation(sceneNumber, rotation);
    }

    void Graphics::updateSceneCameraPosition(unsigned int sceneNumber, Vec3& position) {
        pInterface->updateSceneCameraPosition(sceneNumber, position);
    }

    void Graphics::attachEventHandler(EventHandler& handler) {
        handler.attachGraphics(this);
        pEventHandler = &handler;
    }

    void Graphics::updateScene(unsigned int sceneNumber, IVec2 x1, IVec2 x2) {
        pInterface->updateScene(sceneNumber, x1, x2);
    }
}
