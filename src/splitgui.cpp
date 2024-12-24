#include "include/splitgui.hpp"
#include <cstdio>

namespace SplitGui {

    Window::Window() {

    }

    void Window::createWindow() {

    }

    void Window::instanceVulkan() {

    }

    Ui* Window::parseXml(std::string path) {

    }

    void Window::pushUi(Ui* ui) {
        if (Window::uiData) {
            printf("WARN: ui already exists this will lead to a memory leak if unhandled");
        }

        Window::uiData = ui;
    }

    void Window::cleanupUi(Ui* element) {
        for (int i = 0; i < element->children.size(); i++) {
            cleanupUi(element->children[i]);
            delete element->children[i];
        }
    }

}