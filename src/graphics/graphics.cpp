#include <splitgui/graphics.hpp>
#include <splitgui/window.hpp>
#include "vulkan/vulkan.cpp"

namespace SplitGui {

    Graphics::Graphics() {
        
    }

    Graphics::~Graphics() {
        if (pInterface) {
            delete pInterface;
        }
    }
    
    void Graphics::instanceVulkan(bool validation = false) {
        if (!glfw::vulkanSupported()) {
            printf("ERROR: vulkan is not supported on this device\n");
            throw;
        }
        pInterface = (GraphicsLibInterface*) new VulkanInterface(validation);
        pInterface->instance();
    }

    void Graphics::submitWindow(Window& window) {
        pWindow = &window;
        pInterface->submitWindow(window);
    }

    void Graphics::drawFrame() {
        pInterface->drawFrame();
    }

    void Graphics::drawRect(IVec2 x1, IVec2 x2, HexColor color) {

        IVec2 windowSize = pWindow->getSize();

        Vec2 newX1;
        newX1.x = x1.x / windowSize.x * 2 - 1.0f;
        newX1.y = x1.y / windowSize.y * 2 - 1.0f;

        Vec2 newX2;
        newX2.x = x2.x / windowSize.x * 2 - 1.0f;
        newX2.y = x2.y / windowSize.y * 2 - 1.0f;

        pInterface->drawRect(newX1, newX2, color.normalize());
    }

    void Graphics::submitBuffers() {
        pInterface->submitBuffers();
    }

    void Graphics::instanceScene(Vec2 x1, Vec2 x2) {
        pInterface->instanceScene(x1, x2);
    }
}
