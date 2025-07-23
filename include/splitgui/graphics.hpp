#ifndef SPLITGUI_GRAPHICS_HPP
#define SPLITGUI_GRAPHICS_HPP

#include "lib.hpp"

#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/events.hpp>
#include <array>

namespace SplitGui {

    class SPLITGUI_EXPORT Window;

    struct VulkanFlags {
        unsigned int mdsfTextureSize        = 64;
        HexColor     clearColor             = 0x000000;
        bool         enableValidationLayers = false;
    };

    class SPLITGUI_EXPORT GraphicsLibInterface {
        public:
                       GraphicsLibInterface() {}
              virtual ~GraphicsLibInterface() {}

[[nodiscard]] virtual Result                        instance()                                                                                                                         { throw; }
[[nodiscard]] virtual Result                        submitWindow(Window& window)                                                                                                       { throw; }
[[nodiscard]] virtual Result                        drawFrame()                                                                                                                        { throw; }
              virtual RectRef                       drawRect(Vec2 x1, Vec2 x2, Vec3 color, float depth = 0.0f, VertexFlags flags = 0, uint16_t textureIndex = 0, std::optional<CropRegionRef> crop = std::nullopt) { throw; }
              virtual void                          updateRect(RectRef& ref, Vec2 x1, Vec2 x2, Vec3 color, float depth = 0.0f, uint16_t textureIndex = 0)                              { throw; }
              virtual void                          deleteRect(RectRef& ref)                                                                                                           { throw; }
              virtual void                          deleteText(TextRef& ref)                                                                                                           { throw; }
[[nodiscard]] virtual ResultValue<Vec2>             getTextSize(const std::string& text, float fontSize)                                                                               { throw; }
              virtual void                          deleteEdges(VectorEngineRef& vEngineRef, EdgeRef& edgeRef)                                                                         { throw; }
[[nodiscard]] virtual Result                        updateTrianglesColor(SceneRef& sceneRef, TriangleRef& triangleRef, Vec3 color, float opacity = 1.0f)                               { throw; }
[[nodiscard]] virtual Result                        submitRect(RectRef& ref)                                                                                                           { throw; }
[[nodiscard]] virtual ResultValue<SceneRef>         instanceScene(Vec2 x1, Vec2 x2, float depth = 0.0f)                                                                                { throw; }
[[nodiscard]] virtual ResultValue<VectorEngineRef>  instanceVectorEngine(SceneRef& ref)                                                                                                { throw; }
[[nodiscard]] virtual Result                        updateScene(SceneRef& ref, Vec2 x1, Vec2 x2, float depth = 0.0f)                                                                   { throw; }
[[nodiscard]] virtual ResultValue<EdgeRef>          submitEdgeData(VectorEngineRef& ref, std::vector<Edge>& edges, ModelRef model)                                                                      { throw; }
[[nodiscard]] virtual ResultValue<TriangleRef>      submitTriangleData(SceneRef& ref, std::vector<Vertex>& vertices, std::vector<uint16_t>& indices, ModelRef& model, int flags, int textureNumber = 0) { throw; }
[[nodiscard]] virtual Result                        deleteTriangles(SceneRef& sceneRef, TriangleRef& triangleRef)                                                                      { throw; }
[[nodiscard]] virtual Result                        submitSceneData(SceneRef& sceneRef)                                                                                                { throw; }
              virtual void                          updateSceneCameraPosition(SceneRef& ref, Vec3& position)                                                                           { throw; }
              virtual void                          updateSceneCameraView(SceneRef& ref, Mat4& view)                                                                                   { throw; }
              virtual void                          updateSceneCameraProjection(SceneRef& ref, Mat4& projection)                                                                       { throw; }
              virtual Mat4&                         getModel(ModelRef& model)                                                                                                          { throw; }
[[nodiscard]] virtual ResultValue<ModelRef>         createModel(SceneRef& ref, const Mat4& model)                                                                                      { throw; }
[[nodiscard]] virtual ResultValue<TextRef>          drawText(Vec2 x1, const std::string& text, Vec3 color, float fontSize, float depth = 0.0f)                                         { throw; }
[[nodiscard]] virtual Result                        updateText(TextRef& ref, const std::string& text, Vec2 x1, Vec3 color, float fontSize, float depth = 0.0f)                         { throw; }
[[nodiscard]] virtual Result                        loadFont(const char* path)                                                                                                         { throw; }
[[nodiscard]] virtual ResultValue<TextureRef>       createContourImage(std::vector<Contour>& contours)                                                                                 { throw; }
[[nodiscard]] virtual Result                        submitBuffers()                                                                                                                    { throw; }
[[nodiscard]] virtual Result                        resizeEvent()                                                                                                                      { throw; }
[[nodiscard]] virtual ResultValue<unsigned int>     offsetOf(SceneRef& scene, ModelRef& ref)                                                                                           { throw; }
[[nodiscard]] virtual ResultValue<TextureRef>       rasterizeSvg(const std::string& svg)                                                                                               { throw; }
[[nodiscard]] virtual Result                        updateCropRegion(CropRegionRef& ref, IVec2 x1, IVec2 x2)                                                                           { throw; }
[[nodiscard]] virtual ResultValue<CropRegionRef>    createCropRegion(IVec2 x1, IVec2 x2)                                                                                               { throw; }
              virtual void                          clearBuffers()                                                                                                                     { throw; }

        protected:
            SplitGui::Window* pWindow;
    };

    class SPLITGUI_EXPORT Graphics {
        public:

            friend class Window;

            Graphics();
            ~Graphics();

[[nodiscard]] Result                        instanceVulkan(VulkanFlags flags);
[[nodiscard]] Result                        submitWindow(Window& window);
[[nodiscard]] Result                        drawFrame();
              RectRef                       drawRect(IVec2 x1, IVec2 x2, HexColor color, int depth = 0, VertexFlags flags = 0, int textureIndex = 0, std::optional<CropRegionRef> crop = std::nullopt);
              void                          updateRect(RectRef& ref, IVec2 x1, IVec2 x2, HexColor color, int depth = 0, uint16_t textureIndex = 0);
              void                          deleteRect(RectRef& ref);
              void                          deleteText(TextRef& ref);
              ResultValue<IVec2>            getTextSize(const std::string& text, float fontSize);
              void                          deleteEdges(VectorEngineRef& vEngineRef, EdgeRef& edgeRef);
[[nodiscard]] Result                        updateTrianglesColor(SceneRef& sceneRef, TriangleRef& triangleRef, HexColor color, unsigned int opacity);
[[nodiscard]] Result                        submitRect(RectRef& ref);
[[nodiscard]] ResultValue<SceneRef>         instanceScene(IVec2 x1, IVec2 x2, int depth = 0);
[[nodiscard]] ResultValue<VectorEngineRef>  instanceVectorEngine(SceneRef& ref);
[[nodiscard]] Result                        updateScene(SceneRef& ref, IVec2 x1, IVec2 x2, int depth = 0);
[[nodiscard]] ResultValue<EdgeRef>          submitEdgeData(VectorEngineRef& ref, std::vector<Edge>& edges, ModelRef model);
[[nodiscard]] ResultValue<TriangleRef>      submitTriangleData(SceneRef& ref, std::vector<Vertex>& vertices, std::vector<uint16_t>& indices, ModelRef& model, int flags = 0, int textureNumber = 0);
[[nodiscard]] Result                        deleteTriangles(SceneRef& sceneRef, TriangleRef& triangleRef);
[[nodiscard]] Result                        submitSceneData(SceneRef& sceneRef);
              void                          updateSceneCameraPosition(SceneRef& ref, Vec3& position);
              void                          updateSceneCameraView(SceneRef& ref, Mat4& view);
              void                          updateSceneCameraProjection(SceneRef& ref, Mat4& projection);
              Mat4&                         getModel(ModelRef& model);
[[nodiscard]] ResultValue<ModelRef>         createModel(SceneRef& ref, const Mat4& model);
[[nodiscard]] ResultValue<TextRef>          drawText(IVec2 x1, const std::string& text, HexColor color, float fontSize, int depth = 0);
[[nodiscard]] Result                        updateText(TextRef& ref, const std::string& text, IVec2 x1, HexColor color, float fontSize, int depth = 0);
[[nodiscard]] Result                        loadFont(const char* path);
[[nodiscard]] ResultValue<TextureRef>       createContourImage(std::vector<Contour>& contours);
[[nodiscard]] Result                        submitBuffers();
[[nodiscard]] Result                        resizeEvent();
[[nodiscard]] ResultValue<unsigned int>     offsetOf(SceneRef& scene, ModelRef& ref);
[[nodiscard]] ResultValue<TextureRef>       rasterizeSvg(const std::string& svg);
[[nodiscard]] Result                        updateCropRegion(CropRegionRef& ref, IVec2 x1, IVec2 x2);
[[nodiscard]] ResultValue<CropRegionRef>    createCropRegion(IVec2 x1, IVec2 x2);
              void                          attachEventHandler(EventHandler& handler);
              void                          clearBuffers();

        private:
            Window*               pWindow    = nullptr;
            EventHandler*         pEventHandler = nullptr;
            GraphicsLibInterface* pInterface;
    };
}
#endif