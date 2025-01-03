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
        pInterface->submitWindow(window);
    }

    void Graphics::drawFrame() {
        pInterface->drawFrame();
    }

    void Graphics::drawQuad(std::array<Vec2, 4> vertices, Vec3 color) {
        pInterface->drawQuad(vertices, color);
    }
}
