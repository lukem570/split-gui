#ifndef SPLITGUI_WINDOW_HPP
#define SPLITGUI_WINDOW_HPP

#include "lib.h"

#include <splitgui/interface.hpp>
#include <splitgui/graphics.hpp>
#include <unordered_map>

namespace SplitGui {

    typedef void (*Callback)(SplitGui::Window* window);

    enum class FormatType {
        eGlfw,
        eXcb,
        eXlib,
        eWayland,
        eWindows,
    };

    enum class CallbackType {
        eWindowResize
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

            virtual void           createWindow(const char* title)   { throw; }
            virtual RawWindow*     getWindowData()                   { throw; }
            virtual IVec2          getSize()                         { throw; }

#ifdef SPLIT_GUI_USE_VULKAN
            
            virtual vk::SurfaceKHR createSurface(vk::Instance instance) { throw; }

#endif 
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
            void       setCallback(CallbackType type, Callback callback);
            bool       shouldClose();
            void       update();
            IVec2      getSize();
            RawWindow* getWindowData();

#ifdef SPLIT_GUI_USE_VULKAN
            
            vk::SurfaceKHR createSurface(vk::Instance instance);

#endif 

        private:
            Graphics*           graphics  = nullptr;
            WindowLibInterface* windowLib = nullptr;
            
    };
}
#endif