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

    SplitGui::Default::Rect rect3;
    rect3.setColor(0x373d50);

    SplitGui::Default::Split split1;
    split1.addChild(&rect2);
    split1.addChild(&rect3);
    split1.setVertical(true);

    SplitGui::Default::Split split2;
    split2.addChild(&split1);
    split2.addChild(&rect1);

    SplitGui::InterfaceElement root;
    root.addChild(&split2);
    root.setGraphics(&graphics);
    root.setSize(window.getSize());
    root.setPosition({0, 0});

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