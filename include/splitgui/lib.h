#ifndef SPLITGUI_EXPORT
#ifdef _WIN32
    #ifdef BUILD_SPLITGUI
        #define SPLITGUI_EXPORT __declspec(dllexport)
    #else
        #define SPLITGUI_EXPORT __declspec(dllimport)
    #endif
#elif defined(__GNUC__) || defined(__clang__)
    #ifdef BUILD_SPLITGUI
        #define SPLITGUI_EXPORT __attribute__((visibility("default")))
    #else
        #define SPLITGUI_EXPORT
    #endif
#else
    #error "Unknown platform, please define export/import macros."
#endif
#endif

#include <vulkan/vulkan.hpp>
#include <glfwpp/glfwpp.h>

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vulkan/vulkan_win32.h>

#elif defined(__linux__)  


    #if !defined(__WAYLAND__) || !defined(__USE_XLIB__) || !defined(__USE_XCB__)

    #define GLFW_EXPOSE_NATIVE_X11
    #include <X11/Xlib.h>
    #include <vulkan/vulkan_xlib.h>
    #define __USE_XLIB__

    #elif defined(__WAYLAND__)
        #define GLFW_EXPOSE_NATIVE_WAYLAND
        #include <vulkan/vulkan_wayland.h>
    #elif defined(__USE_XCB__)
        #define GLFW_EXPOSE_NATIVE_X11
        #include <X11/Xlib.h>
        #include <vulkan/vulkan_xcb.h>
    #elif defined(__USE_XLIB__)
        #define GLFW_EXPOSE_NATIVE_X11
        #include <X11/Xlib.h>
        #include <vulkan/vulkan_xlib.h>
    #endif

#endif

#include <glfwpp/native.h>