#include <splitgui/graphics.hpp>
#include <splitgui/window.hpp>
#include "vulkan/vulkan.cpp"

namespace SplitGui {

    Graphics::Graphics() {
        
    }
    
    void Graphics::instanceVulkan(Window& window) {
        VulkanInterface interface;
        interface.instance();
        interface.submitWindow(window.handle);
    }
}
