#include "vulkan.hpp"

namespace SplitGui {
    void VulkanInterface::resizeEvent() {
        recreateSwapchain();

        Result TODO = createVertexUniformBuffer();
    }
}