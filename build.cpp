#include <cbuild/cbuild.hpp>
#include <string>

#if defined(__APPLE__) && defined(__MACH__)
    #error "Mac not supported."
#endif

int build(CBuild::Context context) {

    CBuild::Shared splitgui (
        context,
        "./src/splitgui.cpp",
        "splitgui"
    );

#if defined(_WIN32) || defined(_WIN64)
    splitgui.define("VK_USE_PLATFORM_WIN32_KHR");
    splitgui.linkLibrary("kernel32");
#elif defined(__linux__)

    std::string windowManager = getenv("XDG_SESSION_TYPE");

    if (windowManager == "x11") {
        splitgui.define("VK_USE_PLATFORM_XLIB_KHR");
        splitgui.linkLibrary("X11");
    } else if (windowManager == "wayland") {
        splitgui.define("VK_USE_PLATFORM_WAYLAND_KHR");
        splitgui.linkLibrary("wayland-client");
        splitgui.linkLibrary("wayland-egl");
        splitgui.linkLibrary("wayland-cursor");
    } else {
        printf("Unsupported Linux windowing system\n");
        return -1;
    }

#endif

    splitgui.define("BUILD_SPLITGUI");
    splitgui.includeDirectory("include");
    splitgui.compile();

    CBuild::Executable cube (
        context,
        "./test/cubeTest/main.cpp",
        "cube"
    );

    cube.includeDirectory("include");
    cube.linkLibrary("splitgui");
    cube.compile();
    
    return 0;
}