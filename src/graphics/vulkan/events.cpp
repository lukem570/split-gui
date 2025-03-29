#include "vulkan.hpp"

namespace SplitGui {
    Result VulkanInterface::resizeEvent() {
        SPLITGUI_PROFILE;

        TRYR(recreateRes, recreateSwapchain());

        return Result::eSuccess;
    }
}