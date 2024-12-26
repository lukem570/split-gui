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