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
    
    void Graphics::instanceVulkan() {
        if (!glfw::vulkanSupported()) {
            printf("ERROR: vulkan is not supported on this device\n");
            throw;
        }
        pInterface = (GraphicsLibInterface*) new VulkanInterface();
        pInterface->instance();
    }

    void Graphics::submitWindow(Window& window) {
        pInterface->submitWindow(window);
    }
}
