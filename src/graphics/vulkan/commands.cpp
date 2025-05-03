#include "vulkan.hpp"

namespace SplitGui {
    inline void VulkanInterface::createCommandPool() {
        SPLITGUI_PROFILE;
        
        vk::CommandPoolCreateInfo commandPoolInfo;
        commandPoolInfo.flags            = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
        commandPoolInfo.queueFamilyIndex = graphicsQueueFamilyIndex;

        vk_frameCommandPool = vk_device.createCommandPool(commandPoolInfo);

        vk_interactionCommandPools.resize(graphicsQueueCount);

        for (unsigned int i = 0; i < graphicsQueueCount; i++) {
            vk_interactionCommandPools[i] = vk_device.createCommandPool(commandPoolInfo);
        }

        Logger::info("Created Command Pool");
    }

    inline void VulkanInterface::createCommandBuffers() {
        SPLITGUI_PROFILE;
        
        vk::CommandBufferAllocateInfo allocInfo;
        allocInfo.commandPool        = vk_frameCommandPool;
        allocInfo.level              = vk::CommandBufferLevel::ePrimary;
        allocInfo.commandBufferCount = MAX_FRAMES_IN_FLIGHT;

        vk_commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        
        vk_commandBuffers = vk_device.allocateCommandBuffers(allocInfo);

        Logger::info("Created Command Buffer");
    }
}