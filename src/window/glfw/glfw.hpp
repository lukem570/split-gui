#ifndef SPLITGUI_GLFW_HPP
#define SPLITGUI_GLFW_HPP

#include <splitgui/window.hpp>
#include <splitgui/structs.hpp>
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
            ~GlfwInterface();

            inline void createWindow(const char* title) override;
            
            RawWindow* getWindowData() override;
            IVec2      getSize() override;
            bool       shouldClose() override;
            void       update() override;

#ifdef SPLIT_GUI_USE_VULKAN
            
            vk::SurfaceKHR createSurface(vk::Instance instance) override;
#endif 

        private:
            RawWindow window;

            void submitGraphics(Graphics* pGraphics) override;

            static void resize_callback(glfw::Window& window, int width, int height);

    };
}

#endif