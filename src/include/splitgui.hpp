#ifndef SPLITGUI_HPP
#define SPLITGUI_HPP

#ifdef BUILD_SPLITGUI
    #define SPLITGUI_EXPORT __declspec(dllexport)
#else
    #define SPLITGUI_EXPORT __declspec(dllimport)
#endif

#include <vector>
#include <string>
#include <unordered_map>

namespace SplitGui {

    union propType {
        std::string string;
        int integer;
        float floatingPoint;
        UiData* reference;
    };

    struct UiData {
        std::string name;
        std::vector<UiData*> children;
        std::unordered_map<std::string, propType> props;
    };

    class Ui {
        public:
            UiData* getData() {
                return data;
            }

            ~Ui() {
                cleanupUi(data);
            }

        private:
            void cleanupUi(UiData* element) {
                for (int i = 0; i < element->children.size(); i++) {
                    cleanupUi(element->children[i]);
                    delete element->children[i];
                }
            }

            UiData* data;
    };

    class SPLITGUI_EXPORT Window {
        public:
            Window();
            void createWindow();
            void instanceVulkan();
            Ui parseXml(std::string path);
    };
}
#endif