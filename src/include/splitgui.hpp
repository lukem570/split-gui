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

    struct SPLITGUI_EXPORT Ui {
        std::string name;
        std::vector<Ui*> children;
        std::unordered_map<std::string, std::string> props;
    };

    class SPLITGUI_EXPORT Window {
        public:

            Window();

            ~Window() {
                if(uiData) {
                    cleanupUi(uiData);
                }
            }

            void createWindow();
            void instanceVulkan();
            void pushUi(Ui* ui);
            Ui* parseXml(std::string path);

        private:
            Ui* uiData = nullptr;

            void cleanupUi(Ui* element);
    };
}
#endif