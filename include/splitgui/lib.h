#ifndef SPLITGUI_EXPORT

// linked library defines

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

// Graphics library (default Vulkan)

#if !defined(SPLIT_GUI_USE_VULKAN)

    #define SPLIT_GUI_USE_VULKAN

#elif defined(SPLIT_GUI_USE_VULKAN)
    
#else 
    #error "invalid graphics library"
#endif

// Windowing library (default GLFW)

#if !defined(SPLIT_GUI_USE_GLFW)    && \
    !defined(SPLIT_GUI_USE_XCB)     && \
    !defined(SPLIT_GUI_USE_XLIB)    && \
    !defined(SPLIT_GUI_USE_WAYLAND) && \
    !defined(SPLIT_GUI_USE_WIN32)

    #define SPLIT_GUI_USE_GLFW

#elif defined(SPLIT_GUI_USE_GLFW)
    
#elif defined(SPLIT_GUI_USE_XCB)
    
#elif defined(SPLIT_GUI_USE_XLIB)

#elif defined(SPLIT_GUI_USE_WAYLAND)
    
#elif defined(SPLIT_GUI_USE_WIN32)
    
#else
    #error "invalid windowing library"
#endif


// Vulkan library implementation

#ifdef SPLIT_GUI_USE_VULKAN

    // Choose which desktop window manager to use for vulkan surface
    //
    // defaults:
    // - linux   : VK_USE_PLATFORM_XLIB_KHR
    // - windows : VK_USE_PLATFORM_WIN32_KHR

    #if defined(__linux__) 

        #if !defined(VK_USE_PLATFORM_WAYLAND_KHR) && \
            !defined(VK_USE_PLATFORM_XCB_KHR) && \
            !defined(VK_USE_PLATFORM_XLIB_KHR)

            #define VK_USE_PLATFORM_XLIB_KHR

        #endif

    #elif defined(_WIN32)

        #ifndef VK_USE_PLATFORM_WIN32_KHR

            #define VK_USE_PLATFORM_WIN32_KHR

        #endif
    
    #endif

    // Vulkan settings 

    #define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
    #define VULKAN_HPP_TYPESAFE_CONVERSION 1
    #define VK_NO_PROTOTYPES

    // Vulkan already defined check

    #ifdef VULKAN_H_
        #error "Vulkan previously defined"
    #endif

    // includes

    #include <volk/volk.h>

    // This is wonderful 
    #include <vulkan/vulkan_core.h>
    #undef VK_HEADER_VERSION
    #define VK_HEADER_VERSION 305
    #include <vulkan/vulkan.hpp>

    // dispatch loader

    VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

#endif

// platform specific includes

// TODO: use correct platform definitions instead of the vk_use_platform...

#if defined(__linux__)

    #if defined(VK_USE_PLATFORM_WAYLAND_KHR)

        // Vulkan specific definition for the surface extention

        #ifdef SPLIT_GUI_USE_VULKAN

            #define VK_KHR_WM_SURFACE_EXTENSION_NAME VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME

        #endif 

        // GLFW specific expose for "glfwpp/native.h"

        #ifdef SPLIT_GUI_USE_GLFW

            #define GLFW_EXPOSE_NATIVE_WAYLAND

        #endif

        // wayland includes

        #include <wayland-client.h>
        #include <vulkan/vulkan_wayland.h>

    #elif defined(VK_USE_PLATFORM_XCB_KHR)

        // Vulkan specific definition for the surface extention

        #ifdef SPLIT_GUI_USE_VULKAN

            #define VK_KHR_WM_SURFACE_EXTENSION_NAME VK_KHR_XCB_SURFACE_EXTENSION_NAME

        #endif 

        // GLFW specific expose for "glfwpp/native.h"

        #ifdef SPLIT_GUI_USE_GLFW

            #define GLFW_EXPOSE_NATIVE_X11

        #endif

        // xcb includes

        #include <xcb/xcb.h>
        #include <vulkan/vulkan_xcb.h>
        

    #elif defined(VK_USE_PLATFORM_XLIB_KHR)

        // Vulkan specific definition for the surface extention

        #ifdef SPLIT_GUI_USE_VULKAN

            #define VK_KHR_WM_SURFACE_EXTENSION_NAME VK_KHR_XLIB_SURFACE_EXTENSION_NAME

        #endif 

        // GLFW specific expose for "glfwpp/native.h"

        #ifdef SPLIT_GUI_USE_GLFW

            #define GLFW_EXPOSE_NATIVE_X11

        #endif

        // Xlib includes 

        #include <X11/Xlib.h>
        #include <vulkan/vulkan_xlib.h>

    #endif

#elif defined(_WIN32)

    // Windows settings

    #define WIN32_LEAN_AND_MEAN

    // Vulkan specific definition for the surface extention

    #ifdef SPLIT_GUI_USE_VULKAN

        #define VK_KHR_WM_SURFACE_EXTENSION_NAME VK_KHR_WIN32_SURFACE_EXTENSION_NAME

    #endif

    // GLFW specific expose for "glfwpp/native.h"

    #ifdef SPLIT_GUI_USE_GLFW

        #define GLFW_EXPOSE_NATIVE_WIN32

    #endif

    // Windows includes

    #include <windows.h>
    #include <vulkan/vulkan_win32.h>

#endif

// windowing library implementation

#if defined(SPLIT_GUI_USE_GLFW)

    #include <glfwpp/glfwpp.h>
    #include <glfwpp/native.h>

#elif defined(SPLIT_GUI_USE_XCB)

    #include <X11/Xlib.h>

#elif defined(SPLIT_GUI_USE_XLIB)

    #include <X11/Xlib.h>

#elif defined(SPLIT_GUI_USE_WIN32)

    #include <windows.h>
#endif

// msdfgen 

#define MSDFGEN_PUBLIC
#include <msdfgen/msdfgen.h>

// freetype
#include <setjmp.h>

namespace ft {
    #include <ft2build.h>

    #include FT_FREETYPE_H
    #include FT_OUTLINE_H
}

#endif // !SPLITGUI_EXPORT