#include "vulkan.hpp"

namespace SplitGui {
    Result VulkanInterface::resizeEvent() {
        SPLITGUI_PROFILE;

        if (!resizeUpdate) {
            lastResizeUpdate = clk::high_resolution_clock::now();
            resizeUpdate = true;
        }

        return Result::eSuccess;
    }
}