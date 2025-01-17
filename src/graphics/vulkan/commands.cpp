#include "vulkan.hpp"

namespace SplitGui {
    inline void VulkanInterface::createCommandPool() {
        vk::CommandPoolCreateInfo commandPoolInfo;
        commandPoolInfo.flags            = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
        commandPoolInfo.queueFamilyIndex = graphicsQueueFamilyIndex;

        vk_commandPool = vk_device.createCommandPool(commandPoolInfo);
    }

    inline void VulkanInterface::createCommandBuffers() {
        vk::CommandBufferAllocateInfo allocInfo;
        allocInfo.commandPool        = vk_commandPool;
        allocInfo.level              = vk::CommandBufferLevel::ePrimary;
        allocInfo.commandBufferCount = MAX_FRAMES_IN_FLIGHT;

        vk_commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        
        vk_commandBuffers = vk_device.allocateCommandBuffers(allocInfo);
    }
}