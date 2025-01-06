#ifndef SPLITGUI_INTERFACE_HPP
#define SPLITGUI_INTERFACE_HPP

#include "lib.h"

#include <vector>
#include <string>
#include <unordered_map>

namespace SplitGui {

    class Window;

    enum class InterfaceElementType {

        // Concepts
        eSplit,
        eList,
        eBox,
        eOverlay,

        // Modifiers
        eMask,
        eTransform,

        // Objects
        eRect,
        eScene,
        eText,
        eMedia,

        // Misc
        eBinding,
        eMeta,
    };

    struct SPLITGUI_EXPORT InterfaceElement {
        std::vector<InterfaceElement*> children;
        InterfaceElementType type;
        std::string name;
    };

    class SPLITGUI_EXPORT Interface {
        public:

            friend class Window;

            Interface();
            ~Interface();

            void parseXml(std::string& data);

            void setInterfaceElement(InterfaceElement& data);
            InterfaceElement* getInterfaceElement();

        private:
            InterfaceElement* interfaceElement = nullptr;

            void cleanupInterfaceElement(InterfaceElement* element);
    };
}
#endif