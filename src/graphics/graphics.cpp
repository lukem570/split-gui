#include <splitgui/graphics.hpp>
#include <splitgui/window.hpp>
#include "vulkan/vulkan.cpp"

namespace SplitGui {

    Graphics::Graphics() {
        
    }
    
    void Graphics::instanceVulkan() {

        if (!glfw::vulkanSupported()) {
            printf("ERROR: vulkan is not supported on this device\n");
            throw;
        }

        VulkanInterface interface;
        interface.instance();
        Graphics::pInterface = (GraphicsLibInterface*) &interface;
    }

    void Graphics::submitWindow(Window& window) {
        pInterface->submitWindow(window.handle);
    }
}
