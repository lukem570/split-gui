#include "glfw.hpp"

namespace SplitGui {

    ResultValue<vk::SurfaceKHR> GlfwInterface::createSurface(vk::Instance instance) {
        SPLITGUI_PROFILE;

#if defined(__linux__)
    #if defined(VK_USE_PLATFORM_WAYLAND_KHR)

        vk::WaylandSurfaceCreateInfoKHR createInfo;
        createInfo.display = glfwGetWaylandDisplay();
        createInfo.surface = glfwGetWaylandWindow(*window.handle);

        if (createInfo.display == nullptr || createInfo.surface == nullptr) {
            return Result::eFailedToCreateSurface;
        }

        return instance.createWaylandSurfaceKHR(createInfo);

    #elif defined(VK_USE_PLATFORM_XCB_KHR)

        vk::XcbSurfaceCreateInfoKHR createInfo;
        createInfo.connection = XGetXCBConnection(glfwGetX11Display());
        createInfo.window     = glfwGetX11Window(*window.handle);

        if (createInfo.connection == nullptr || createInfo.window == 0) {
            return Result::eFailedToCreateSurface;
        }

        return instance.createXcbSurfaceKHR(createInfo);

    #elif defined(VK_USE_PLATFORM_XLIB_KHR)

        vk::XlibSurfaceCreateInfoKHR createInfo;
        createInfo.dpy    = glfwGetX11Display();
        createInfo.window = glfwGetX11Window(*window.handle);

        if (createInfo.dpy == None || createInfo.window == None) {
            return Result::eFailedToCreateSurface;
        }
        
        return instance.createXlibSurfaceKHR(createInfo);

    #endif
#elif defined(_WIN32)

        vk::Win32SurfaceCreateInfoKHR createInfo;
        createInfo.hinstance = GetModuleHandle(nullptr);
        createInfo.hwnd      = glfwGetWin32Window(*window.handle);

        if (createInfo.hwnd == nullptr) {
            return Result::eFailedToCreateSurface;
        }

        return instance.createWin32SurfaceKHR(createInfo);     

#endif

    }
}