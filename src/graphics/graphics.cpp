#include <splitgui/result.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/window.hpp>
#include "vulkan/vulkan.cpp"

namespace SplitGui {

    Graphics::Graphics() {
        
    }

    Graphics::~Graphics() {
        SPLITGUI_PROFILE;

        if (pInterface) {
            ma::untrack(pInterface);
            delete pInterface;
        }
    }
    
    Result Graphics::instanceVulkan(VulkanFlags flags) {
        SPLITGUI_PROFILE;

        if (!glfw::vulkanSupported()) {
            return Result::eVulkanNotSupported;
        }

        pInterface = (GraphicsLibInterface*) new(std::nothrow) VulkanInterface(flags);
        ma::track(pInterface, "vulkan instance");
        if (!pInterface) {
            return Result::eHeapAllocFailed;
        }

        pInterface->instance();

        return Result::eSuccess;
    }

    Result Graphics::submitWindow(Window& window) {
        SPLITGUI_PROFILE;

        pWindow = &window;
        return pInterface->submitWindow(window);
    }

    Result Graphics::drawFrame() {
        SPLITGUI_PROFILE;
        SPLITGUI_PROFILE_FRAME;

        return pInterface->drawFrame();
    }

    RectRef Graphics::drawRect(IVec2 x1, IVec2 x2, HexColor color, int depth, VertexFlags flags,  int textureIndex, std::optional<CropRegionRef> crop) {
        SPLITGUI_PROFILE;

        IVec2 windowSize = pWindow->getSize();

        Vec2 newX1;
        newX1.x = (float)x1.x / windowSize.x * 2.0 - 1.0f;
        newX1.y = (float)x1.y / windowSize.y * 2.0 - 1.0f;

        Vec2 newX2;
        newX2.x = (float)x2.x / windowSize.x * 2.0 - 1.0f;
        newX2.y = (float)x2.y / windowSize.y * 2.0 - 1.0f;

        return pInterface->drawRect(newX1, newX2, color.normalize(), (float)depth / DEPTH_PLANE, flags, textureIndex, crop);
    }

    void Graphics::updateRect(RectRef& ref, IVec2 x1, IVec2 x2, HexColor color, int depth, uint16_t textureIndex) {
        SPLITGUI_PROFILE;

        IVec2 windowSize = pWindow->getSize();

        Vec2 newX1;
        newX1.x = (float)x1.x / (float)windowSize.x * 2.0f - 1.0f;
        newX1.y = (float)x1.y / (float)windowSize.y * 2.0f - 1.0f;

        Vec2 newX2;
        newX2.x = (float)x2.x / (float)windowSize.x * 2.0f - 1.0f;
        newX2.y = (float)x2.y / (float)windowSize.y * 2.0f - 1.0f;

        pInterface->updateRect(ref, newX1, newX2, color.normalize(),(float)depth / DEPTH_PLANE, textureIndex);
    }

    void Graphics::deleteRect(RectRef& ref) {
        SPLITGUI_PROFILE;

        pInterface->deleteRect(ref);
    }

    void Graphics::deleteText(TextRef& ref) {
        SPLITGUI_PROFILE;

        pInterface->deleteText(ref);
    }

    Result Graphics::submitRect(RectRef& ref) {
        SPLITGUI_PROFILE;

        return pInterface->submitRect(ref);
    }

    Result Graphics::submitBuffers() {
        SPLITGUI_PROFILE;

        return pInterface->submitBuffers();
    }

    ResultValue<SceneRef> Graphics::instanceScene(IVec2 x1, IVec2 x2, int depth, std::optional<CropRegionRef> crop) {
        SPLITGUI_PROFILE;

        IVec2 windowSize = pWindow->getSize();

        Vec2 newX1;
        newX1.x = (float)x1.x / (float)windowSize.x * 2.0f - 1.0f;
        newX1.y = (float)x1.y / (float)windowSize.y * 2.0f - 1.0f;

        Vec2 newX2;
        newX2.x = (float)x2.x / (float)windowSize.x * 2.0f - 1.0f;
        newX2.y = (float)x2.y / (float)windowSize.y * 2.0f - 1.0f;

        return pInterface->instanceScene(newX1, newX2, (float)depth / DEPTH_PLANE, crop);
    }

    ResultValue<VectorEngineRef> Graphics::instanceVectorEngine(SceneRef& ref) {
        SPLITGUI_PROFILE;

        return pInterface->instanceVectorEngine(ref);
    }

    Result Graphics::resizeEvent() {
        SPLITGUI_PROFILE;

        TRYR(resizeRes, pInterface->resizeEvent());
        return Result::eSuccess;
    }

    ResultValue<TextRef> Graphics::drawText(IVec2 x1, const std::string& text, HexColor color, float fontSize, int depth, std::optional<CropRegionRef> crop) {
        SPLITGUI_PROFILE;

        IVec2 windowSize = pWindow->getSize();

        Vec2 newX1;
        newX1.x = (float)x1.x / windowSize.x * 2.0 - 1.0f;
        newX1.y = (float)x1.y / windowSize.y * 2.0 - 1.0f;

        return pInterface->drawText(newX1, text, color.normalize(), fontSize, (float)depth / DEPTH_PLANE, crop);
    }

    Result Graphics::updateText(TextRef& ref, const std::string& text, IVec2 x1, HexColor color, float fontSize, int depth) {
        SPLITGUI_PROFILE;

        IVec2 windowSize = pWindow->getSize();

        Vec2 newX1;
        newX1.x = (float)x1.x / windowSize.x * 2.0 - 1.0f;
        newX1.y = (float)x1.y / windowSize.y * 2.0 - 1.0f;

        return pInterface->updateText(ref, text, newX1, color.normalize(), fontSize, (float)depth / DEPTH_PLANE);
    }

    Result Graphics::loadFont(const char* path) {
        SPLITGUI_PROFILE;

        return pInterface->loadFont(path);
    }

    void Graphics::clearBuffers() {
        SPLITGUI_PROFILE;

        pInterface->clearBuffers();
    }

    ResultValue<TriangleRef> Graphics::submitTriangleData(SceneRef& ref, std::vector<Vertex>& vertices, std::vector<uint16_t>& indices, ModelRef& model, int flags, int textureNumber) {
        SPLITGUI_PROFILE;

        return pInterface->submitTriangleData(ref, vertices, indices, model, flags, textureNumber);
    }

    ResultValue<EdgeRef> Graphics::submitEdgeData(VectorEngineRef& ref, std::vector<Edge>& edges, ModelRef model, HexColor color) {
        SPLITGUI_PROFILE;

        return pInterface->submitEdgeData(ref, edges, model, color.normalize());
    }

    Result Graphics::resubmitEdgeData(VectorEngineRef& veng, EdgeRef edgeRef, std::vector<Edge>& edges, ModelRef model, HexColor color) {
        SPLITGUI_PROFILE;

        return pInterface->resubmitEdgeData(veng, edgeRef, edges, model, color.normalize());
    }
    

    Result Graphics::updateTrianglesColor(SceneRef& sceneRef, TriangleRef& triangleRef, HexColor color, unsigned int opacity) {
        SPLITGUI_PROFILE;

        return pInterface->updateTrianglesColor(sceneRef, triangleRef, color.normalize(), (float) opacity / 255.f);
    }

    Result Graphics::deleteEdges(VectorEngineRef& vEngineRef, EdgeRef& edgeRef) {
        SPLITGUI_PROFILE;

        return pInterface->deleteEdges(vEngineRef, edgeRef);
    }

    void Graphics::updateSceneCameraPosition(SceneRef& ref, Vec3& position) {
        SPLITGUI_PROFILE;

        pInterface->updateSceneCameraPosition(ref, position);
    }

    void Graphics::updateSceneCameraView(SceneRef& ref, Mat4& view) {
        SPLITGUI_PROFILE;

        pInterface->updateSceneCameraView(ref, view);
    }

    void Graphics::updateSceneCameraProjection(SceneRef& ref, Mat4& projection) {
        SPLITGUI_PROFILE;

        pInterface->updateSceneCameraProjection(ref, projection);
    }

    Mat4& Graphics::getModel(ModelRef& model) {
        SPLITGUI_PROFILE;

        return pInterface->getModel(model);
    }

    ResultValue<ModelRef> Graphics::createModel(SceneRef& ref, const Mat4& model) {
        SPLITGUI_PROFILE;

        return pInterface->createModel(ref, model);
    }

    Result Graphics::deleteTriangles(SceneRef& sceneRef, TriangleRef& triangleRef) {
        SPLITGUI_PROFILE;

        return pInterface->deleteTriangles(sceneRef, triangleRef);
    }

    void Graphics::attachEventHandler(EventHandler& handler) {
        SPLITGUI_PROFILE;

        handler.attachGraphics(this);
        pEventHandler = &handler;
    }

    Result Graphics::updateScene(SceneRef& ref, IVec2 x1, IVec2 x2, int depth) {
        SPLITGUI_PROFILE;

        IVec2 windowSize = pWindow->getSize();

        Vec2 newX1;
        newX1.x = (float)x1.x / (float)windowSize.x * 2.0f - 1.0f;
        newX1.y = (float)x1.y / (float)windowSize.y * 2.0f - 1.0f;

        Vec2 newX2;
        newX2.x = (float)x2.x / (float)windowSize.x * 2.0f - 1.0f;
        newX2.y = (float)x2.y / (float)windowSize.y * 2.0f - 1.0f;

        return pInterface->updateScene(ref, newX1, newX2, (float)depth / DEPTH_PLANE);
    }

    Result Graphics::submitSceneData(SceneRef& sceneRef) {
        SPLITGUI_PROFILE;

        return pInterface->submitSceneData(sceneRef);
    }

    ResultValue<TextureRef> Graphics::createContourImage(std::vector<Contour>& contours) {
        SPLITGUI_PROFILE;

        return pInterface->createContourImage(contours);
    }

    ResultValue<IVec2> Graphics::getTextSize(const std::string& text, float fontSize) {
        SPLITGUI_PROFILE;

        ResultValue<Vec2> normSize = pInterface->getTextSize(text, fontSize);
        TRYD(normSize);

        IVec2 windowSize = pWindow->getSize();
        IVec2 size;
        size.x = (normSize.value.x + 1.0f) / 2.0f * windowSize.x;
        size.y = (normSize.value.y + 1.0f) / 2.0f * windowSize.y;

        return size;
    }

    ResultValue<TextureRef> Graphics::rasterizeSvg(const std::string& svg) {
        SPLITGUI_PROFILE;

        return pInterface->rasterizeSvg(svg);
    }

    Result Graphics::updateCropRegion(CropRegionRef& ref, IVec2 x1, IVec2 x2) {
        SPLITGUI_PROFILE;

        return pInterface->updateCropRegion(ref, x1, x2);
    }

    ResultValue<CropRegionRef> Graphics::createCropRegion(IVec2 x1, IVec2 x2) {
        SPLITGUI_PROFILE;

        return pInterface->createCropRegion(x1, x2);
    }

    Result Graphics::submitText(TextRef& ref) {
        SPLITGUI_PROFILE;

        return pInterface->submitText(ref);
    }
}
