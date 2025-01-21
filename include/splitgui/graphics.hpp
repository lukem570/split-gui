#ifndef SPLITGUI_GRAPHICS_HPP
#define SPLITGUI_GRAPHICS_HPP

#include "lib.h"

#include <splitgui/structs.hpp>
#include <array>

namespace SplitGui {

    enum class GraphicsMode {
        eNull,
        eVulkan,
    };

    class SPLITGUI_EXPORT Window;

    class SPLITGUI_EXPORT GraphicsLibInterface {
        public:
            GraphicsLibInterface() {}
            ~GraphicsLibInterface() {}

            virtual void instance()                                                    { throw; }
            virtual void submitWindow(Window& window)                                  { throw; }
            virtual void drawFrame()                                                   { throw; }
            virtual void drawRect(Vec2 x1, Vec2 x2, Vec3 color, VertexFlags flags = 0) { throw; }
            virtual void instanceScene(Vec2 x1, Vec2 x2)                               { throw; }
            virtual void drawText(Vec2 x1, Vec2 x2, std::string& text)                 { throw; }
            virtual void loadFont(const char* path)                                    { throw; }
            virtual void drawMedia(Vec2 x1, Vec2 x2)                                   { throw; }
            virtual void submitBuffers()                                               { throw; }
            virtual void resizeEvent()                                                 { throw; }

        protected:
            SplitGui::Window* pWindow;
    };

    class SPLITGUI_EXPORT Graphics {
        public:

            friend class Window;

            Graphics();
            ~Graphics();

            void instanceVulkan(bool validation);
            void submitWindow(Window& window);
            void drawFrame();
            void drawRect(IVec2 x1, IVec2 x2, HexColor color);
            void instanceScene(Vec2 x1, Vec2 x2);
            void drawText(IVec2 x1, IVec2 x2, std::string text);
            void loadFont(const char* path);
            void drawMedia(Vec2 x1, Vec2 x2);
            void submitBuffers();
            void resizeEvent();


        private:
            GraphicsMode          mode       = GraphicsMode::eNull;
            Window*               pWindow    = nullptr;
            GraphicsLibInterface* pInterface;
    };
}
#endif