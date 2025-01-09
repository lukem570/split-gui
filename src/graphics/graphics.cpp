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
        newX1.x = (float)x1.x / windowSize.x * 2.0 - 1.0f;
        newX1.y = (float)x1.y / windowSize.y * 2.0 - 1.0f;

        Vec2 newX2;
        newX2.x = (float)x2.x / windowSize.x * 2.0 - 1.0f;
        newX2.y = (float)x2.y / windowSize.y * 2.0 - 1.0f;

        printf("drawrect: (%.6f, %.6f), (%.6f, %.6f), color: (%.6f, %.6f, %.6f)\n", newX1.x, newX1.y, newX2.x, newX2.y, color.normalize().x, color.normalize().y, color.normalize().z);

        pInterface->drawRect(newX1, newX2, color.normalize());
    }

    void Graphics::submitBuffers() {
        pInterface->submitBuffers();
    }

    void Graphics::instanceScene(Vec2 x1, Vec2 x2) {
        pInterface->instanceScene(x1, x2);
    }
}
