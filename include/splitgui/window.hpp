#ifndef SPLITGUI_WINDOW_HPP
#define SPLITGUI_WINDOW_HPP

#include "lib.h"

#include <splitgui/interface.hpp>
#include <splitgui/graphics.hpp>

namespace SplitGui {

    enum class FormatType {
        eGlfw,
        eXcb,
        eXlib,
        eWayland,
        eWindows,
    };

    struct SPLITGUI_EXPORT RawWindow {
        FormatType type;

        #ifdef SPLIT_GUI_USE_GLFW
        
        glfw::Window* handle = nullptr;

        #else
            #error "not implemented"
        #endif
    };

    class SPLITGUI_EXPORT WindowLibInterface {
        public:
            WindowLibInterface() {}
            ~WindowLibInterface() {}

            virtual void       createWindow(const char* title) {}
            virtual RawWindow* getWindowData() { throw; return nullptr; }

        private:
            RawWindow window;

    };

    class SPLITGUI_EXPORT Window {
        public:

            friend class Graphics;
            friend class GraphicsLibInterface;
            friend class Interface;

            Window();
            ~Window();

            void       instanceGlfw();
            void       createWindow(const char* title);
            void       attachInterface(Interface& interface);
            void       updateInterface();
            void       __devLoop();
            RawWindow* getWindowData();

        private:
            Interface*          interface = nullptr;
            Graphics*           graphics  = nullptr;
            WindowLibInterface* windowLib = nullptr;

    };
}
#endif