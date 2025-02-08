#include "vulkan.hpp"

namespace SplitGui {
    Result VulkanInterface::resizeEvent() {
        recreateSwapchain();

        TRYR(vertexUniformRes, createVertexUniformBuffer());
        updateDescriptorSets();

        return Result::eSuccess;
    }
}