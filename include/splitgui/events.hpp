#ifndef SPLITGUI_EVENTS_HPP
#define SPLITGUI_EVENTS_HPP

#include "lib.hpp"

namespace SplitGui {

    class EventHandler {
        public:
            
            // for binding functions to be used in parsing / interface
            template <typename T>
            void bindFunction();
    };
}

#endif