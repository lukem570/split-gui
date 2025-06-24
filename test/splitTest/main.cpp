#include <splitgui/window.hpp>
#include <splitgui/result.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/structs.hpp>

int main() {
    ma::Instance instance;

    SPLITGUI_PROFILE;

    SplitGui::EventHandler eventHandler;

    SplitGui::WindowFlags flags;
    flags.title = "split test";

    SplitGui::Window window;
    TRYRC(glfwRes, window.instanceGlfw());
    TRYRC(windowRes, window.createWindow(flags));
    window.attachEventHandler(eventHandler);

    SplitGui::VulkanFlags vulkanFlags;
    vulkanFlags.enableValidationLayers = true;

    SplitGui::Graphics graphics;
    TRYRC(instanceRes, graphics.instanceVulkan(vulkanFlags));
    TRYRC(winSubRes, graphics.submitWindow(window));
    graphics.attachEventHandler(eventHandler);

    SplitGui::Default::Rect rect1;
    rect1.setColor(0x9aaedb);

    SplitGui::Default::Rect rect2;
    rect2.setColor(0x636e87);

    SplitGui::Default::Rect rect3;
    rect3.setColor(0x929bb0);

    SplitGui::Default::Split split1;
    split1.addChild(&rect2);
    split1.addChild(&rect3);
    split1.setVertical(true);
    TRYRC(split1Res, split1.setPosition("50%"));

    SplitGui::Default::Split split2;
    split2.addChild(&split1);
    split2.addChild(&rect1);
    TRYRC(split2Res, split2.setPosition("50%"));

    SplitGui::InterfaceElement root;
    root.addChild(&split2);
    root.setGraphics(&graphics);
    root.setSize(window.getSize());
    root.setPosition({0, 0});

    TRYRC(interfaceRes, root.instance());
    TRYRC(submitRes, graphics.submitBuffers());

    while (!window.shouldClose()) {
        while (eventHandler.popEvent()) {
            
        }  
        
        TRYRC(frameRes, graphics.drawFrame());
        window.update();
    }

    return 0;
}