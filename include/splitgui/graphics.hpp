#ifndef SPLITGUI_GRAPHICS_HPP
#define SPLITGUI_GRAPHICS_HPP

#ifndef SPLITGUI_EXPORT
#ifdef BUILD_SPLITGUI
    #define SPLITGUI_EXPORT __declspec(dllexport)
#else
    #define SPLITGUI_EXPORT __declspec(dllimport)
#endif
#endif

#include <splitgui/window.hpp>

namespace SplitGui {

    class SPLITGUI_EXPORT Window;

    class SPLITGUI_EXPORT Graphics {
        public:

            friend class Window;

            Graphics();

            void instanceVulkan(Window& window);

        private:
    };
}
#endif