#ifndef SPLITGUI_INTERFACE_HPP
#define SPLITGUI_INTERFACE_HPP

#ifndef SPLITGUI_EXPORT
#ifdef BUILD_SPLITGUI
    #define SPLITGUI_EXPORT __declspec(dllexport)
#else
    #define SPLITGUI_EXPORT __declspec(dllimport)
#endif
#endif

#include <vector>
#include <string>
#include <unordered_map>

namespace SplitGui {

    class Window;

    struct SPLITGUI_EXPORT InterfaceData {
        std::string name;
        std::vector<InterfaceData*> children;
        std::unordered_map<std::string, std::string> props;
    };

    class SPLITGUI_EXPORT Interface {
        public:

            friend class Window;

            Interface();
            ~Interface();

            void parseXml(std::string& data);

            void setInterfaceData(InterfaceData& data);
            InterfaceData* getInterfaceData();

        private:
            InterfaceData* interfaceData = nullptr;

            void cleanupInterfaceData(InterfaceData* element);
    };
}
#endif