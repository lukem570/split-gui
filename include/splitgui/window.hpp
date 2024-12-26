#ifndef SPLITGUI_WINDOW_HPP
#define SPLITGUI_WINDOW_HPP

#include "lib.h"

#include <splitgui/interface.hpp>
#include <splitgui/graphics.hpp>

namespace SplitGui {

    class SPLITGUI_EXPORT Window {
        public:

            friend class Graphics;
            friend class Interface;

            Window();

            void createWindow(const char* title);
            void attachGraphics(Graphics& graphics);
            void attachInterface(Interface& interface);
            void updateInterface();

        private:
            Interface* interface = nullptr;
            Graphics* graphics = nullptr;

    };
}
#endif