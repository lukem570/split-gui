#include "vulkan.hpp"

namespace SplitGui {
    Result VulkanInterface::resizeEvent() {
        TRYR(recreateRes, recreateSwapchain());

        return Result::eSuccess;
    }
}