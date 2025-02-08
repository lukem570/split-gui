#ifndef SPLITGUI_WINDOW_HPP
#define SPLITGUI_WINDOW_HPP

#include "lib.hpp"

#include <splitgui/events.hpp>
#include <splitgui/interface.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/events.hpp>
#include <unordered_map>

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
        Graphics*  pGraphics;

#ifdef SPLIT_GUI_USE_GLFW
        
        glfw::Window* handle = nullptr;

#else
    #error "not implemented"
#endif
    };

    class SPLITGUI_EXPORT WindowLibInterface {
        public:

                                 WindowLibInterface() {}
              virtual           ~WindowLibInterface() {}

[[nodiscard]] virtual Result     createWindow(const char* title)                 { throw; }
              virtual RawWindow* getWindowData()                                 { throw; }
              virtual IVec2      getSize()                                       { throw; }
              virtual void       update()                                        { throw; }
              virtual bool       shouldClose()                                   { throw; }
              virtual void       submitEventHandler(EventHandler* pEventHandler) { throw; }

#ifdef SPLIT_GUI_USE_VULKAN
            
[[nodiscard]] virtual ResultValue<vk::SurfaceKHR> createSurface(vk::Instance instance) { throw; }

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

[[nodiscard]] Result     instanceGlfw();
[[nodiscard]] Result     createWindow(const char* title);
              bool       shouldClose();
              void       update();
              IVec2      getSize();
              RawWindow* getWindowData();
              void       attachEventHandler(EventHandler& handler);

#ifdef SPLIT_GUI_USE_VULKAN
            
[[nodiscard]] ResultValue<vk::SurfaceKHR> createSurface(vk::Instance instance);

#endif 

        private:
            Graphics*           pGraphics     = nullptr;
            WindowLibInterface* windowLib     = nullptr;
            EventHandler*       pEventHandler = nullptr;
            
    };
}
#endif