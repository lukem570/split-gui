#ifndef SPLITGUI_GRAPHICS_HPP
#define SPLITGUI_GRAPHICS_HPP

#include "lib.h"

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

            virtual void instance() { throw; }
            virtual void submitWindow(Window& window) { throw; }
            virtual void drawFrame() { throw; }

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

        private:
            GraphicsMode mode = GraphicsMode::eNull;
            GraphicsLibInterface* pInterface;
    };
}
#endif