#ifndef SPLITGUI_WINDOW_HPP
#define SPLITGUI_WINDOW_HPP

#ifndef SPLITGUI_EXPORT
#ifdef BUILD_SPLITGUI
    #define SPLITGUI_EXPORT __declspec(dllexport)
#else
    #define SPLITGUI_EXPORT __declspec(dllimport)
#endif
#endif

#include <splitgui/interface.hpp>
#include <splitgui/graphics.hpp>

namespace SplitGui {

    class SPLITGUI_EXPORT Window {
        public:

            friend class Graphics;
            friend class Interface;

            Window();

            void createWindow();
            void attachGraphics(Graphics& graphics);
            void attachInterface(Interface& interface);
            void updateInterface();

        private:
            Interface* interface = nullptr;
            Graphics* graphics = nullptr;

    };
}
#endif