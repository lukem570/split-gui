#ifndef SPLITGUI_WINDOW_HPP
#define SPLITGUI_WINDOW_HPP

#include "lib.h"

#include <splitgui/interface.hpp>
#include <splitgui/graphics.hpp>
#include <vulkan/vulkan.hpp>
#include <glfwpp/glfwpp.h>

namespace SplitGui {

    class SPLITGUI_EXPORT Window {
        public:

            friend class Graphics;
            friend class Interface;

            Window();
            ~Window();

            void createWindow(const char* title);
            void attachInterface(Interface& interface);
            void updateInterface();
            void __devLoop();

        private:
            Interface* interface = nullptr;
            Graphics* graphics = nullptr;
            glfw::Window* handle = nullptr;
            glfw::GlfwLibrary* glfw = nullptr;

    };
}
#endif