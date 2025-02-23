#include "glfw.hpp"

namespace SplitGui {
    GlfwInterface::~GlfwInterface() {
        
    #if defined(__linux__)
        #if defined(VK_USE_PLATFORM_XLIB_KHR)

        Display* display = glfwGetX11Display(); 

        if (cursor != (long unsigned int)-1) {
            XFreeCursor(display, cursor);
        }
        
        #endif
    #endif

        if (window.handle) {
            delete window.handle;
        }

        glfwTerminate();
    }
}