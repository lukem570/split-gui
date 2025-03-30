#include "vulkan.hpp"

namespace SplitGui {
    inline void VulkanInterface::createFramebuffers() {
        SPLITGUI_PROFILE;

        vk_swapchainFramebuffers.resize(vk_swapchainImageViews.size());

        for (unsigned int i = 0; i < vk_swapchainImageViews.size(); i++) {
            std::array<vk::ImageView, 2> attachments = {
                vk_swapchainImageViews[i],
                vk_depthImageView
            };

            vk::FramebufferCreateInfo framebufferInfo;
            framebufferInfo.renderPass      = vk_renderpass;
            framebufferInfo.attachmentCount = attachments.size();
            framebufferInfo.pAttachments    = attachments.data();
            framebufferInfo.width           = vk_swapchainExtent.width;
            framebufferInfo.height          = vk_swapchainExtent.height;
            framebufferInfo.layers          = 1;

            vk_swapchainFramebuffers[i] = vk_device.createFramebuffer(framebufferInfo);
        }

        Logger::info("Created Framebuffers");
    }

    void VulkanInterface::clearBuffers() {
        SPLITGUI_PROFILE;

        indices.clear();
        vertices.clear();
        scenes.clear();

        knownIndicesSize = 0;
    }

    inline Result VulkanInterface::createBuffer(
        vk::DeviceSize             size,
        vk::BufferUsageFlags       usage,
        vk::MemoryPropertyFlags    properties,
        vk::Buffer&                out_buffer,
        vk::DeviceMemory&          out_bufferMemory
    ) {
        SPLITGUI_PROFILE;

        vk::BufferCreateInfo createInfo;
        createInfo.size        = size;
        createInfo.usage       = usage;
        createInfo.sharingMode = vk::SharingMode::eExclusive;

        out_buffer = vk_device.createBuffer(createInfo);

        vk::MemoryRequirements memoryRequirements = vk_device.getBufferMemoryRequirements(out_buffer);

        ResultValue<uint32_t> memType = findMemoryType(memoryRequirements.memoryTypeBits, properties);

        TRYD(memType);

        vk::MemoryAllocateInfo allocInfo;
        allocInfo.allocationSize  = memoryRequirements.size;
        allocInfo.memoryTypeIndex = memType.value;

        out_bufferMemory = vk_device.allocateMemory(allocInfo);

        vk_device.bindBufferMemory(out_buffer, out_bufferMemory, 0);

        return Result::eSuccess;
    }

    inline vk::CommandBuffer VulkanInterface::startCopyBuffer(StagingBuffer& stagingBuffer) {
        SPLITGUI_PROFILE;
        
        commandPoolMutex.lock();

        vk::CommandBufferBeginInfo beginInfo;
        beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

        if (stagingBuffer.commandBuffer != VK_NULL_HANDLE) {
            stagingBuffer.commandBuffer.reset();
            stagingBuffer.commandBuffer.begin(beginInfo);
            return stagingBuffer.commandBuffer;
        }

        vk::CommandBufferAllocateInfo allocInfo;
        allocInfo.level              = vk::CommandBufferLevel::ePrimary;
        allocInfo.commandPool        = vk_commandPool;
        allocInfo.commandBufferCount = 1;

        std::vector<vk::CommandBuffer> commandBuffers = vk_device.allocateCommandBuffers(allocInfo);

        stagingBuffer.commandBuffer = commandBuffers.front();

        stagingBuffer.commandBuffer.begin(beginInfo);

        return commandBuffers.front();
    }

    inline void VulkanInterface::copyBuffer(vk::Buffer src, vk::Buffer dst, vk::DeviceSize size, vk::CommandBuffer commandBuffer, vk::BufferCopy& copyRegion) {
        SPLITGUI_PROFILE;

        copyRegion.size = size;
        copyRegion.srcOffset = 0; 
        copyRegion.dstOffset = 0; 

        commandBuffer.copyBuffer(src, dst, 1, &copyRegion);
    }

    inline Result VulkanInterface::endSingleTimeCommands(vk::CommandBuffer commandBuffer) {
        SPLITGUI_PROFILE;

        commandBuffer.end();

        vk::SubmitInfo submitInfo;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers    = &commandBuffer;

        queueMutex.lock();

        vk::Result result_submit = vk_graphicsQueue.submit(1, &submitInfo, nullptr);

        if (result_submit != vk::Result::eSuccess) {
            return Result::eFailedToSubmitQueue;
        }

        vk_graphicsQueue.waitIdle();

        queueMutex.unlock();

        commandPoolMutex.unlock();

        return Result::eSuccess;
    }

    inline Result VulkanInterface::InstanceStagingBuffer(StagingBuffer& stagingBuffer, vk::DeviceSize size) {
        SPLITGUI_PROFILE;

        if (size == stagingBuffer.size) {
            return Result::eSuccess;
        }

        cleanupStagingBuffer(stagingBuffer);

        TRYR(bufferRes, createBuffer(
            size, 
            vk::BufferUsageFlagBits::eTransferSrc, 
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
            stagingBuffer.buffer,
            stagingBuffer.memory
        ));

        stagingBuffer.size = size;

        return Result::eSuccess;
    }
}