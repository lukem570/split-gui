#include "glfw.hpp"

namespace SplitGui {

    vk::SurfaceKHR GlfwInterface::createSurface(vk::Instance instance) {

#if defined(__linux__)
    #if defined(VK_USE_PLATFORM_WAYLAND_KHR)

        vk::WaylandSurfaceCreateInfoKHR createInfo;
        createInfo.display = glfwGetWaylandDisplay();
        createInfo.surface = glfwGetWaylandWindow(window);

        if (createInfo.display == nullptr || createInfo.surface == nullptr) {
            printf("Error getting window handles for Wayland\n");
            throw;
        }

        return instance.createWaylandSurfaceKHR(createInfo);

    #elif defined(VK_USE_PLATFORM_XCB_KHR)

        vk::XcbSurfaceCreateInfoKHR createInfo;
        createInfo.connection = glfwGetX11Display();
        createInfo.window     = glfwGetX11Window(window);

        if (createInfo.connection == nullptr || createInfo.window == 0) {
            printf("Error getting window handles for XCB\n");
            throw;
        }

        return instance.createXcbSurfaceKHR(createInfo);

    #elif defined(VK_USE_PLATFORM_XLIB_KHR)

        vk::XlibSurfaceCreateInfoKHR createInfo;
        createInfo.dpy    = glfwGetX11Display();
        createInfo.window = glfwGetX11Window(*window.handle);

        if (createInfo.dpy == None || createInfo.window == None) {
            printf("Error getting window handles for xlib\n");
            throw;
        }
        
        return instance.createXlibSurfaceKHR(createInfo);

    #endif
#elif defined(_WIN32)

        vk::Win32SurfaceCreateInfoKHR createInfo;
        createInfo.hinstance = GetModuleHandle(nullptr);
        createInfo.hwnd      = glfwGetWin32Window(window);

        if (createInfo.hwnd == nullptr) {
            printf("Error getting window handle for win32\n");
            throw;
        }

        return instance.createWin32SurfaceKHR(createInfo);     

#endif

    }
}