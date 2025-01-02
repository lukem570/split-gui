#ifndef SPLITGUI_EXPORT
// TODO: THIS FILE IS CURSED NEEDS REFACTOR

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

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#define VULKAN_HPP_NO_EXCEPTIONS
#define VULKAN_HPP_TYPESAFE_CONVERSION 1

#if !defined(VK_USE_PLATFORM_WAYLAND_KHR) && \
    !defined(VK_USE_PLATFORM_XCB_KHR) && \
    !defined(VK_USE_PLATFORM_XLIB_KHR)
#define VK_USE_PLATFORM_XLIB_KHR
#endif

#ifdef VULKAN_H_
#error "vulkan previously defined"
#endif

#include <volk/volk.h>

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.hpp>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

#if !defined(SPLIT_GUI_USE_GLFW)  && \
    !defined(SPLIT_GUI_USE_XCB)   && \
    !defined(SPLIT_GUI_USE_XLIB)  && \
    !defined(SPLIT_GUI_USE_WIN32)

    #include <glfwpp/glfwpp.h>

    #define SPLIT_GUI_USE_GLFW
#elif defined(SPLIT_GUI_USE_GLFW)
    #include <glfwpp/glfwpp.h>
#elif defined(SPLIT_GUI_USE_XCB)
    #include <X11/Xlib.h>
#elif defined(SPLIT_GUI_USE_XLIB)
    #include <X11/Xlib.h>
#elif defined(SPLIT_GUI_USE_WIN32)
    #include <windows.h>
#endif

#ifdef _WIN32

#ifdef SPLIT_GUI_USE_GLFW
    #define GLFW_EXPOSE_NATIVE_WIN32
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vulkan/vulkan_win32.h>
#define VK_KHR_WM_SURFACE_EXTENSION_NAME VK_KHR_WIN32_SURFACE_EXTENSION_NAME

#elif defined(__linux__)  

    #if defined(VK_USE_PLATFORM_WAYLAND_KHR)

        #ifdef SPLIT_GUI_USE_GLFW
            #define GLFW_EXPOSE_NATIVE_WAYLAND
        #endif

        #include <X11/Xlib.h>
        #include <vulkan/vulkan_wayland.h>
        #define VK_KHR_WM_SURFACE_EXTENSION_NAME VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME

    #elif defined(VK_USE_PLATFORM_XCB_KHR)

        #ifdef SPLIT_GUI_USE_GLFW
            #define GLFW_EXPOSE_NATIVE_X11
        #endif

        #include <X11/Xlib.h>
        #include <vulkan/vulkan_xcb.h>
        #define VK_KHR_WM_SURFACE_EXTENSION_NAME VK_KHR_XCB_SURFACE_EXTENSION_NAME

    #elif defined(VK_USE_PLATFORM_XLIB_KHR)

        #ifdef SPLIT_GUI_USE_GLFW
            #define GLFW_EXPOSE_NATIVE_X11
        #endif

        #include <X11/Xlib.h>
        #include <vulkan/vulkan_xlib.h>
        #define VK_KHR_WM_SURFACE_EXTENSION_NAME VK_KHR_XLIB_SURFACE_EXTENSION_NAME

    #else
        #error "unknown linux windowing library specified"
    #endif
#else
    #error "Unsupported platform"
#endif

#ifdef SPLIT_GUI_USE_GLFW
    #include <glfwpp/native.h>
#endif

#endif