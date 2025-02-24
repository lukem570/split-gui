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

    enum class CursorType {
        eArrow,
        eIBeam,
        eCrosshair,
        eHand,
        eHorizontalArrow,
        eVerticalArrow,
        eDiagonal,
        eDiagonalInverse, // in the math sense of -1 slope
    };

    struct SPLITGUI_EXPORT RawWindow {
        FormatType type;
        Graphics*  pGraphics;

#ifdef SPLIT_GUI_USE_GLFW
        
        glfw::Window* handle = nullptr;

#endif
    };

    struct WindowFlags {
        const char* title;
        bool  resizeable = true;
        bool  visible    = true;
        bool  decorated  = true;
    };

    class SPLITGUI_EXPORT WindowLibInterface {
        public:

                                 WindowLibInterface() = default;
              virtual           ~WindowLibInterface() = default;

[[nodiscard]] virtual Result     createWindow(WindowFlags flags)                 { throw; }
              virtual RawWindow* getWindowData()                                 { throw; }
              virtual IVec2      getSize()                                       { throw; }
              virtual void       setSize(IVec2 size)                             { throw; }
              virtual void       update()                                        { throw; }
              virtual bool       shouldClose()                                   { throw; }
              virtual void       submitEventHandler(EventHandler* pEventHandler) { throw; }
              virtual void       close()                                         { throw; }
              virtual void       minimize()                                      { throw; }
              virtual void       maximize()                                      { throw; }
              virtual void       restore()                                       { throw; }
              virtual bool       maximized()                                     { throw; }
              virtual void       setWindowPosition(IVec2 position)               { throw; }
              virtual IVec2      getWindowPosition()                             { throw; }
              virtual IVec2      getCursorPosition()                             { throw; }
              virtual void       setCursorShape(CursorType type)                 { throw; }

#ifdef SPLIT_GUI_USE_VULKAN
#ifdef BUILD_SPLITGUI   

[[nodiscard]] virtual ResultValue<vk::SurfaceKHR> createSurface(vk::Instance instance) { throw; }

#endif
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
[[nodiscard]] Result     createWindow(WindowFlags flags);
              bool       shouldClose();
              void       update();
              IVec2      getSize();
              void       setSize(IVec2 size);
              RawWindow* getWindowData();
              void       attachEventHandler(EventHandler& handler);
              void       close();
              void       minimize();
              void       maximize();
              void       restore();
              bool       maximized();
              void       setWindowPosition(IVec2 position);
              IVec2      getWindowPosition();
              IVec2      getCursorPosition();
              void       setCursorShape(CursorType type);

#ifdef SPLIT_GUI_USE_VULKAN
#ifdef BUILD_SPLITGUI   

[[nodiscard]] ResultValue<vk::SurfaceKHR> createSurface(vk::Instance instance);

#endif 
#endif 

        private:
            Graphics*           pGraphics     = nullptr;
            WindowLibInterface* windowLib     = nullptr;
            EventHandler*       pEventHandler = nullptr;
            
    };
}
#endif