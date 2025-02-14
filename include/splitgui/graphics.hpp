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

[[nodiscard]] virtual Result             instance()                                                                                                             { throw; }
[[nodiscard]] virtual Result             submitWindow(Window& window)                                                                                           { throw; }
[[nodiscard]] virtual Result             drawFrame()                                                                                                            { throw; }
              virtual RectRef            drawRect(Vec2 x1, Vec2 x2, Vec3 color, VertexFlags flags = 0, uint16_t textureIndex = 0)                               { throw; }
              virtual void               updateRect(RectRef& ref, Vec2 x1, Vec2 x2)                                                                             { throw; }
              virtual unsigned int       instanceScene(IVec2 x1, IVec2 x2)                                                                                      { throw; }
              virtual void               updateScene(unsigned int ref, IVec2 x1, IVec2 x2)                                                                      { throw; }
              virtual void               submitTriangleData(unsigned int sceneNumber, std::vector<Vertex>& vertices, std::vector<uint16_t>& indices, int flags) { throw; }
[[nodiscard]] virtual Result             updateSceneCameraPosition(unsigned int sceneNumber, Vec3& position)                                                    { throw; }
[[nodiscard]] virtual Result             updateSceneCameraView(unsigned int sceneNumber, Mat4& view)                                                            { throw; }
[[nodiscard]] virtual Result             updateSceneCameraProjection(unsigned int sceneNumber, Mat4& projection)                                                { throw; }
[[nodiscard]] virtual ResultValue<float> drawText(Vec2 x1, std::string& text)                                                                                   { throw; }
[[nodiscard]] virtual Result             loadFont(const char* path)                                                                                             { throw; }
              virtual void               drawMedia(Vec2 x1, Vec2 x2)                                                                                            { throw; }
[[nodiscard]] virtual Result             submitBuffers()                                                                                                        { throw; }
[[nodiscard]] virtual Result             resizeEvent()                                                                                                          { throw; }
              virtual void               clearBuffers()                                                                                                         { throw; }

        protected:
            SplitGui::Window* pWindow;
    };

    class SPLITGUI_EXPORT Graphics {
        public:

            friend class Window;

            Graphics();
            ~Graphics();

[[nodiscard]] Result           instanceVulkan(VulkanFlags flags);
              void             submitWindow(Window& window);
              void             drawFrame();
              RectRef          drawRect(IVec2 x1, IVec2 x2, HexColor color);
              void             updateRect(RectRef& ref, IVec2 x1, IVec2 x2);
              unsigned int     instanceScene(IVec2 x1, IVec2 x2);
              void             updateScene(unsigned int sceneNumber, IVec2 x1, IVec2 x2);
              void             submitTriangleData(unsigned int sceneNumber, std::vector<Vertex>& vertices, std::vector<uint16_t>& indices, int flags);
[[nodiscard]] Result           updateSceneCameraPosition(unsigned int sceneNumber, Vec3& position);
[[nodiscard]] Result           updateSceneCameraView(unsigned int sceneNumber, Mat4& view);
[[nodiscard]] Result           updateSceneCameraProjection(unsigned int sceneNumber, Mat4& projection);
[[nodiscard]] ResultValue<int> drawText(IVec2 x1, std::string text); // Returns the width in pixels of the text
[[nodiscard]] Result           loadFont(const char* path);
              void             drawMedia(Vec2 x1, Vec2 x2);
              void             submitBuffers();
[[nodiscard]] Result           resizeEvent();
              void             attachEventHandler(EventHandler& handler);
              void             clearBuffers();

        private:
            Window*               pWindow    = nullptr;
            EventHandler*         pEventHandler = nullptr;
            GraphicsLibInterface* pInterface;
    };
}
#endif