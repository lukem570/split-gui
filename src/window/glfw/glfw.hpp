#ifndef SPLITGUI_GLFW_HPP
#define SPLITGUI_GLFW_HPP

#include <splitgui/window.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/events.hpp>
#include <tuple>

/*
#include "glfw.hpp"

namespace SplitGui {
    
}
*/

namespace SplitGui {


    class GlfwInterface : WindowLibInterface {
        public:
                          GlfwInterface();
                         ~GlfwInterface() override;

[[nodiscard]] inline Result createWindow(WindowFlags flags) override;
            
            RawWindow*    getWindowData() override;
            IVec2         getSize() override;
            bool          shouldClose() override;
            void          update() override;
            void          submitEventHandler(EventHandler* pEventHandler) override;
            void          close() override;
            void          minimize() override;
            void          maximize() override;
            void          restore() override;
            bool          maximized() override;

#ifdef SPLIT_GUI_USE_VULKAN
            
[[nodiscard]] ResultValue<vk::SurfaceKHR> createSurface(vk::Instance instance) override;
#endif 

        private:
            RawWindow window;


            static void resize_callback(glfw::Window& window, int width, int height);
            static void key_callback(glfw::Window& window, glfw::KeyCode key, int scancode, glfw::KeyState action, glfw::ModifierKeyBit mods);
            static void mouseButton_callback(glfw::Window& window, glfw::MouseButton mouseButton, glfw::MouseButtonState mouseState, glfw::ModifierKeyBit mods);
            static void mouseMove_callback(glfw::Window& window, double posX, double posY);
            static void scroll_callback(glfw::Window& window, double offX, double offY);

            static inline KeyCode    convertGlfwKeyCode(glfw::KeyCode code);
            static inline KeyState   convertGlfwKeyState(glfw::KeyState state);
            static inline MouseCode  convertGlfwMouseButton(glfw::MouseButton code);
            static inline MouseState convertGlfwMouseState(glfw::MouseButtonState state);

    };
}

#endif