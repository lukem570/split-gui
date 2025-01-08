#include <splitgui/window.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/structs.hpp>

int main() {
    SplitGui::Window window;
    window.instanceGlfw();
    window.createWindow("split test");

    SplitGui::Graphics graphics;
    graphics.instanceVulkan(true);
    graphics.submitWindow(window);

    SplitGui::Default::Rect rect1;
    rect1.setColor(0x444f70);

    SplitGui::Default::Rect rect2;
    rect2.setColor(0x272d40);

    SplitGui::Default::Split split;
    split.addChild(&rect1);
    split.addChild(&rect2);

    SplitGui::InterfaceElement root;
    root.addChild(&split);
    root.setGraphics(&graphics);

    //SplitGui::Interface interface;
    //interface.setInterfaceElement(root);

    root.instance();


    graphics.submitBuffers();

    while (!window.shouldClose()) {
        graphics.drawFrame();
        window.update();
    }

    return 0;
}