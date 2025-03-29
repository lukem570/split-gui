#include "glfw.hpp"

namespace SplitGui {

    void GlfwInterface::update() { 
        SPLITGUI_PROFILE;
        
        window.handle->swapBuffers();
        glfw::pollEvents();
    }

    void GlfwInterface::close() { 
        SPLITGUI_PROFILE;
        
        window.handle->setShouldClose(true);
    }

    void GlfwInterface::minimize() { 
        SPLITGUI_PROFILE;
        
        window.handle->iconify();
    }

    void GlfwInterface::maximize() { 
        SPLITGUI_PROFILE;
        
        window.handle->maximize();
    }

    void GlfwInterface::restore() { 
        SPLITGUI_PROFILE;
        
        window.handle->restore();
    }

    void GlfwInterface::setWindowPosition(IVec2 position) { 
        SPLITGUI_PROFILE;
        
        window.handle->setPos(position.x, position.y);
    }

    void GlfwInterface::setSize(IVec2 size) { 
        SPLITGUI_PROFILE;
        
        window.handle->setSize(size.x, size.y);
    }

#if defined(VK_USE_PLATFORM_WAYLAND_KHR)

extern "C" {
    wl_shm* shm;

    static void registry_global(void* data, wl_registry *registry,
                                uint32_t id, const char *interface,
                                uint32_t version) {
        if (strcmp(interface, "wl_shm") == 0) {
            shm = (wl_shm*)wl_registry_bind(registry, id, &wl_shm_interface, 1);
        }
    }

    static void registry_global_remove(void* data, wl_registry *registry, uint32_t id) {
        if (shm) {
            shm = NULL;
        }
    }

    static const struct wl_registry_listener registry_listener = {
        .global = registry_global,
        .global_remove = registry_global_remove
    };
}

#endif

    void GlfwInterface::setCursorShape(CursorType type) { 
        SPLITGUI_PROFILE;
        

    #if defined(__linux__)
        #if defined(VK_USE_PLATFORM_WAYLAND_KHR)
    
            wl_display* display = glfwGetWaylandDisplay();
            wl_surface* surface = glfwGetWaylandWindow(*window.handle);

            wl_registry* registry = wl_display_get_registry(display);

            wl_registry_add_listener(registry, &registry_listener, NULL);

            wl_display_dispatch(display);

            wl_cursor_theme* cursor_theme = wl_cursor_theme_load(NULL, 32, shm);

            wl_cursor* cursor;
    
            switch (type) {
                case CursorType::eArrow:           cursor = wl_cursor_theme_get_cursor(cursor_theme, "arrow");           break;
                case CursorType::eCrosshair:       cursor = wl_cursor_theme_get_cursor(cursor_theme, "crosshair");       break;
                case CursorType::eHand:            cursor = wl_cursor_theme_get_cursor(cursor_theme, "grab");            break;
                case CursorType::eIBeam:           cursor = wl_cursor_theme_get_cursor(cursor_theme, "ibeam");           break;
                case CursorType::eHorizontalArrow: cursor = wl_cursor_theme_get_cursor(cursor_theme, "right_arrow");     break;
                case CursorType::eVerticalArrow:   cursor = wl_cursor_theme_get_cursor(cursor_theme, "bottom_arrow");    break;
                case CursorType::eDiagonal:        cursor = wl_cursor_theme_get_cursor(cursor_theme, "ne-resize");             break; 
                case CursorType::eDiagonalInverse: cursor = wl_cursor_theme_get_cursor(cursor_theme, "nw-resize");
                default: break;
            }

            wl_cursor_image* image = cursor->images[0];
            wl_surface_attach(surface, wl_cursor_image_get_buffer(image), 0, 0);

        #elif defined(VK_USE_PLATFORM_XCB_KHR)
    
            // TODO:
    
        #elif defined(VK_USE_PLATFORM_XLIB_KHR)
    
            Display*      display = glfwGetX11Display(); 
            unsigned long xwindow = glfwGetX11Window(*window.handle);  

            // diagonal cursors aren't in cursorfont.h and loading them from kde or gnome themes would be too much effort so fleur will do

            switch (type) {
                case CursorType::eArrow:           cursor = XCreateFontCursor(display, XC_arrow);             break;
                case CursorType::eCrosshair:       cursor = XCreateFontCursor(display, XC_crosshair);         break;
                case CursorType::eHand:            cursor = XCreateFontCursor(display, XC_hand1);             break;
                case CursorType::eIBeam:           cursor = XCreateFontCursor(display, XC_xterm);             break;
                case CursorType::eHorizontalArrow: cursor = XCreateFontCursor(display, XC_sb_h_double_arrow); break;
                case CursorType::eVerticalArrow:   cursor = XCreateFontCursor(display, XC_sb_v_double_arrow); break;
                case CursorType::eDiagonal:        cursor = XCreateFontCursor(display, XC_fleur);             break; 
                case CursorType::eDiagonalInverse: cursor = XCreateFontCursor(display, XC_fleur);             break;
                default: break;
            }

            XDefineCursor(display, xwindow, cursor);
    
        #endif
    #elif defined(_WIN32)
    
        HWND hwnd = glfwGetWin32Window(*window.handle);

        HCURSOR cursor;

        switch (type) {
            case CursorType::eArrow:           cursor = LoadCursor(NULL, IDC_ARROW);  break;
            case CursorType::eCrosshair:       cursor = LoadCursor(NULL, IDC_CROSS);  break;
            case CursorType::eHand:            cursor = LoadCursor(NULL, IDC_HAND);   break;
            case CursorType::eIBeam:           cursor = LoadCursor(NULL, IDC_IBEAM);  break;
            case CursorType::eHorizontalArrow: cursor = LoadCursor(NULL, IDC_SIZEWE); break;
            case CursorType::eVerticalArrow:   cursor = LoadCursor(NULL, IDC_SIZENS); break;
            case CursorType::eDiagonal:        cursor = LoadCursor(NULL, IDC_SIZENESW); break;
            case CursorType::eDiagonalInverse: cursor = LoadCursor(NULL, IDC_SIZENWSE); break;
            default: break;
        }

        SetSystemCursor(cursor, OCR_NORMAL);
    
    #endif
    }
}