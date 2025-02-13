#include "vulkan.hpp"

namespace SplitGui {
    Result VulkanInterface::resizeEvent() {
        TRYR(recreateRes, recreateSwapchain());

        TRYR(vertexUniformRes, createVertexUniformBuffer());
        updateDescriptorSets();

        return Result::eSuccess;
    }
}