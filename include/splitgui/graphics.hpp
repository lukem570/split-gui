#ifndef SPLITGUI_GRAPHICS_HPP
#define SPLITGUI_GRAPHICS_HPP

#include "lib.h"

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