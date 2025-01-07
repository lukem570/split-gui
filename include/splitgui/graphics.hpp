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

            virtual void instance()                                         { throw; }
            virtual void submitWindow(Window& window)                       { throw; }
            virtual void drawFrame()                                        { throw; }
            virtual void drawRect(Vec2 x1, Vec2 x2, Vec3 color)             { throw; }
            virtual void submitBuffers()                                    { throw; }

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
            void drawRect(Vec2 x1, Vec2 x2, Vec3 color);

        private:
            GraphicsMode mode = GraphicsMode::eNull;
            GraphicsLibInterface* pInterface;
    };
}
#endif