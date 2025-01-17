#include "vulkan.hpp"

namespace SplitGui {
    void VulkanInterface::resizeEvent() {
        recreateSwapchain();
    }
}