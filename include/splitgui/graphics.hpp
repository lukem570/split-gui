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

[[nodiscard]] virtual Result                    instance()                                                                                                                         { throw; }
[[nodiscard]] virtual Result                    submitWindow(Window& window)                                                                                                       { throw; }
[[nodiscard]] virtual Result                    drawFrame()                                                                                                                        { throw; }
              virtual RectRef                   drawRect(Vec2 x1, Vec2 x2, Vec3 color, float depth = 0.0f, VertexFlags flags = 0, uint16_t textureIndex = 0)                       { throw; }
              virtual void                      updateRect(RectRef& ref, Vec2 x1, Vec2 x2, Vec3 color, float depth = 0.0f)                                                         { throw; }
              virtual void                      deleteRect(RectRef& ref)                                                                                                           { throw; }
[[nodiscard]] virtual Result                    submitRect(RectRef& ref)                                                                                                           { throw; }
[[nodiscard]] virtual ResultValue<SceneRef>     instanceScene(Vec2 x1, Vec2 x2, float depth = 0.0f)                                                                                { throw; }
[[nodiscard]] virtual Result                    updateScene(SceneRef& ref, Vec2 x1, Vec2 x2, float depth = 0.0f)                                                                   { throw; }
[[nodiscard]] virtual ResultValue<TriangleRef>  submitTriangleData(SceneRef& ref, std::vector<Vertex>& vertices, std::vector<uint16_t>& indices, int flags, int textureNumber = 0) { throw; }
[[nodiscard]] virtual Result                    deleteTriangles(SceneRef& sceneRef, TriangleRef& triangleRef)                                                                      { throw; }
[[nodiscard]] virtual Result                    submitSceneData(SceneRef& sceneRef)                                                                                                { throw; }
              virtual void                      updateSceneCameraPosition(SceneRef& ref, Vec3& position)                                                                           { throw; }
              virtual void                      updateSceneCameraView(SceneRef& ref, Mat4& view)                                                                                   { throw; }
              virtual void                      updateSceneCameraProjection(SceneRef& ref, Mat4& projection)                                                                       { throw; }
              virtual ModelRef                  createModel(SceneRef& ref, Mat4& model)                                                                                            { throw; }
[[nodiscard]] virtual ResultValue<TextRef>      drawText(Vec2 x1, std::string& text, Vec3 color, int fontSize, float depth = 0.0f)                                                 { throw; }
[[nodiscard]] virtual Result                    updateText(TextRef& ref, Vec2 x1, Vec3 color, int fontSize, float depth = 0.0f)                                                    { throw; }
[[nodiscard]] virtual Result                    loadFont(const char* path)                                                                                                         { throw; }
[[nodiscard]] virtual ResultValue<unsigned int> createContourImage(std::vector<Contour>& contours)                                                                                 { throw; }
[[nodiscard]] virtual Result                    submitBuffers()                                                                                                                    { throw; }
[[nodiscard]] virtual Result                    resizeEvent()                                                                                                                      { throw; }
              virtual void                      clearBuffers()                                                                                                                     { throw; }

        protected:
            SplitGui::Window* pWindow;
    };

    class SPLITGUI_EXPORT Graphics {
        public:

            friend class Window;

            Graphics();
            ~Graphics();

[[nodiscard]] Result                    instanceVulkan(VulkanFlags flags);
[[nodiscard]] Result                    submitWindow(Window& window);
[[nodiscard]] Result                    drawFrame();
              RectRef                   drawRect(IVec2 x1, IVec2 x2, HexColor color, int depth = 0, VertexFlags flags = 0, int textureIndex = 0);
              void                      updateRect(RectRef& ref, IVec2 x1, IVec2 x2, HexColor color, int depth = 0);
              void                      deleteRect(RectRef& ref);
[[nodiscard]] Result                    submitRect(RectRef& ref);
[[nodiscard]] ResultValue<SceneRef>     instanceScene(IVec2 x1, IVec2 x2, int depth = 0);
[[nodiscard]] Result                    updateScene(SceneRef& ref, IVec2 x1, IVec2 x2, int depth = 0);
[[nodiscard]] ResultValue<TriangleRef>  submitTriangleData(SceneRef& ref, std::vector<Vertex>& vertices, std::vector<uint16_t>& indices, int flags = 0, int textureNumber = 0);
[[nodiscard]] Result                    deleteTriangles(SceneRef& sceneRef, TriangleRef& triangleRef);
[[nodiscard]] Result                    submitSceneData(SceneRef& sceneRef);
              void                      updateSceneCameraPosition(SceneRef& ref, Vec3& position);
              void                      updateSceneCameraView(SceneRef& ref, Mat4& view);
              void                      updateSceneCameraProjection(SceneRef& ref, Mat4& projection);
              ModelRef                  createModel(SceneRef& ref, Mat4& model);
[[nodiscard]] ResultValue<TextRef>      drawText(IVec2 x1, std::string text, HexColor color, int fontSize, int depth = 0);
[[nodiscard]] Result                    updateText(TextRef& ref, IVec2 x1, HexColor color, int fontSize, int depth = 0);
[[nodiscard]] Result                    loadFont(const char* path);
[[nodiscard]] ResultValue<unsigned int> createContourImage(std::vector<Contour>& contours);
[[nodiscard]] Result                    submitBuffers();
[[nodiscard]] Result                    resizeEvent();
              void                      attachEventHandler(EventHandler& handler);
              void                      clearBuffers();

        private:
            Window*               pWindow    = nullptr;
            EventHandler*         pEventHandler = nullptr;
            GraphicsLibInterface* pInterface;
    };
}
#endif