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
    
    Result Graphics::instanceVulkan(VulkanFlags flags) {
        if (!glfw::vulkanSupported()) {
            return Result::eVulkanNotSupported;
        }

        pInterface = (GraphicsLibInterface*) new(std::nothrow) VulkanInterface(flags);
        if (!pInterface) {
            return Result::eHeapAllocFailed;
        }

        pInterface->instance();

        return Result::eSuccess;
    }

    Result Graphics::submitWindow(Window& window) {
        pWindow = &window;
        return pInterface->submitWindow(window);
    }

    Result Graphics::drawFrame() {
        return pInterface->drawFrame();
    }

    RectRef Graphics::drawRect(IVec2 x1, IVec2 x2, HexColor color, int depth, VertexFlags flags,  int textureIndex) {

        IVec2 windowSize = pWindow->getSize();

        Vec2 newX1;
        newX1.x = (float)x1.x / windowSize.x * 2.0 - 1.0f;
        newX1.y = (float)x1.y / windowSize.y * 2.0 - 1.0f;

        Vec2 newX2;
        newX2.x = (float)x2.x / windowSize.x * 2.0 - 1.0f;
        newX2.y = (float)x2.y / windowSize.y * 2.0 - 1.0f;

        return pInterface->drawRect(newX1, newX2, color.normalize(), (float)depth / DEPTH_PLANE, flags, textureIndex);
    }

    void Graphics::updateRect(RectRef& ref, IVec2 x1, IVec2 x2, HexColor color, int depth) {
        IVec2 windowSize = pWindow->getSize();

        Vec2 newX1;
        newX1.x = (float)x1.x / (float)windowSize.x * 2.0f - 1.0f;
        newX1.y = (float)x1.y / (float)windowSize.y * 2.0f - 1.0f;

        Vec2 newX2;
        newX2.x = (float)x2.x / (float)windowSize.x * 2.0f - 1.0f;
        newX2.y = (float)x2.y / (float)windowSize.y * 2.0f - 1.0f;

        pInterface->updateRect(ref, newX1, newX2, color.normalize(),(float)depth / DEPTH_PLANE);
    }

    Result Graphics::submitRect(RectRef& ref) {
        return pInterface->submitRect(ref);
    }

    Result Graphics::submitBuffers() {
        return pInterface->submitBuffers();
    }

    ResultValue<SceneRef> Graphics::instanceScene(IVec2 x1, IVec2 x2, int depth) {
        IVec2 windowSize = pWindow->getSize();

        Vec2 newX1;
        newX1.x = (float)x1.x / (float)windowSize.x * 2.0f - 1.0f;
        newX1.y = (float)x1.y / (float)windowSize.y * 2.0f - 1.0f;

        Vec2 newX2;
        newX2.x = (float)x2.x / (float)windowSize.x * 2.0f - 1.0f;
        newX2.y = (float)x2.y / (float)windowSize.y * 2.0f - 1.0f;

        return pInterface->instanceScene(newX1, newX2, (float)depth / DEPTH_PLANE);
    }

    Result Graphics::resizeEvent() {
        TRYR(resizeRes, pInterface->resizeEvent());
        return Result::eSuccess;
    }

    ResultValue<TextRef> Graphics::drawText(IVec2 x1, std::string text, HexColor color, int fontSize, int depth) {
        IVec2 windowSize = pWindow->getSize();

        Vec2 newX1;
        newX1.x = (float)x1.x / windowSize.x * 2.0 - 1.0f;
        newX1.y = (float)x1.y / windowSize.y * 2.0 - 1.0f;

        return pInterface->drawText(newX1, text, color.normalize(), fontSize, (float)depth / DEPTH_PLANE);
    }

    Result Graphics::updateText(TextRef& ref, IVec2 x1, HexColor color, int fontSize, int depth) {
        IVec2 windowSize = pWindow->getSize();

        Vec2 newX1;
        newX1.x = (float)x1.x / windowSize.x * 2.0 - 1.0f;
        newX1.y = (float)x1.y / windowSize.y * 2.0 - 1.0f;

        return pInterface->updateText(ref, newX1, color.normalize(), fontSize, (float)depth / DEPTH_PLANE);
    }

    Result Graphics::loadFont(const char* path) {
        return pInterface->loadFont(path);
    }

    void Graphics::clearBuffers() {
        pInterface->clearBuffers();
    }

    Result Graphics::submitTriangleData(SceneRef& ref, std::vector<Vertex>& vertices, std::vector<uint16_t>& indices, int flags, int textureNumber) {
        return pInterface->submitTriangleData(ref, vertices, indices, flags, textureNumber);
    }

    Result Graphics::updateSceneCameraPosition(SceneRef& ref, Vec3& position) {
        return pInterface->updateSceneCameraPosition(ref, position);
    }

    Result Graphics::updateSceneCameraView(SceneRef& ref, Mat4& view) {
        return pInterface->updateSceneCameraView(ref, view);
    }

    Result Graphics::updateSceneCameraProjection(SceneRef& ref, Mat4& projection) {
        return pInterface->updateSceneCameraProjection(ref, projection);
    }

    void Graphics::attachEventHandler(EventHandler& handler) {
        handler.attachGraphics(this);
        pEventHandler = &handler;
    }

    Result Graphics::updateScene(SceneRef& ref, IVec2 x1, IVec2 x2) {
        return pInterface->updateScene(ref, x1, x2);
    }

    [[nodiscard]] ResultValue<unsigned int> Graphics::createContourImage(std::vector<Contour>& contours) {
        return pInterface->createContourImage(contours);
    }
}
