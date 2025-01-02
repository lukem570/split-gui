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
            virtual void instance() {}
            virtual void submitWindow(Window& window) {}

        protected:
            SplitGui::Window* pWindow;
    };

    class SPLITGUI_EXPORT Graphics {
        public:

            friend class Window;

            Graphics();
            ~Graphics();

            void instanceVulkan();
            void submitWindow(Window& window);

        private:
            GraphicsMode mode = GraphicsMode::eNull;
            GraphicsLibInterface* pInterface;
    };
}
#endif