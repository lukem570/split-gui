#ifndef SPLITGUI_GRAPHICS_HPP
#define SPLITGUI_GRAPHICS_HPP

#include "lib.hpp"

#include <splitgui/result.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/events.hpp>
#include <array>

namespace SplitGui {

    class SPLITGUI_EXPORT Window;

    class SPLITGUI_EXPORT GraphicsLibInterface {
        public:
            GraphicsLibInterface() {}
            ~GraphicsLibInterface() {}

[[nodiscard]] virtual Result             instance()                                                                                                  { throw; }
[[nodiscard]] virtual Result             submitWindow(Window& window)                                                                                { throw; }
[[nodiscard]] virtual Result             drawFrame()                                                                                                 { throw; }
              virtual RectRef            drawRect(Vec2 x1, Vec2 x2, Vec3 color, VertexFlags flags = 0, uint16_t textureIndex = 0)                    { throw; }
              virtual void               updateRect(RectRef& ref, Vec2 x1, Vec2 x2)                                                                  { throw; }
              virtual SceneObj*          instanceScene(IVec2 x1, IVec2 x2)                                                                           { throw; }
              virtual void               updateScene(SceneObj* ref, IVec2 x1, IVec2 x2)                                                              { throw; }
              virtual void               submitTriangleData(unsigned int sceneNumber, std::vector<Vertex>& vertices, std::vector<uint16_t>& indices) { throw; }
[[nodiscard]] virtual Result             updateSceneCameraRotation(unsigned int sceneNumber, Vec3& rotation)                                         { throw; }
[[nodiscard]] virtual Result             updateSceneCameraPosition(unsigned int sceneNumber, Vec3& position)                                         { throw; }
[[nodiscard]] virtual ResultValue<float> drawText(Vec2 x1, std::string& text)                                                                        { throw; }
              virtual void               loadFont(const char* path)                                                                                  { throw; }
              virtual void               drawMedia(Vec2 x1, Vec2 x2)                                                                                 { throw; }
[[nodiscard]] virtual Result             submitBuffers()                                                                                             { throw; }
              virtual void               resizeEvent()                                                                                               { throw; }

        protected:
            SplitGui::Window* pWindow;
    };

    class SPLITGUI_EXPORT Graphics {
        public:

            friend class Window;

            Graphics();
            ~Graphics();

[[nodiscard]] Result           instanceVulkan(bool validation);
              void             submitWindow(Window& window);
              void             drawFrame();
              RectRef          drawRect(IVec2 x1, IVec2 x2, HexColor color);
              void             updateRect(RectRef& ref, IVec2 x1, IVec2 x2);
              SceneObj*        instanceScene(IVec2 x1, IVec2 x2);
              void             updateScene(SceneObj* ref, IVec2 x1, IVec2 x2);
              void             submitTriangleData(unsigned int sceneNumber, std::vector<Vertex>& vertices, std::vector<uint16_t>& indices);
              void             updateSceneCameraRotation(unsigned int sceneNumber, Vec3& rotation);
              void             updateSceneCameraPosition(unsigned int sceneNumber, Vec3& position);
[[nodiscard]] ResultValue<int> drawText(IVec2 x1, std::string text); // Returns the width in pixels of the text
              void             loadFont(const char* path);
              void             drawMedia(Vec2 x1, Vec2 x2);
              void             submitBuffers();
              void             resizeEvent();
              void             attachEventHandler(EventHandler& handler);

        private:
            Window*               pWindow    = nullptr;
            EventHandler*         pEventHandler = nullptr;
            GraphicsLibInterface* pInterface;
    };
}
#endif