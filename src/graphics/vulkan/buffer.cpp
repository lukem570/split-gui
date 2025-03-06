#include "vulkan.hpp"

namespace SplitGui {
    inline void VulkanInterface::createFramebuffers() {
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

        SPLITGUI_LOG("Created Framebuffers");
    }

    void VulkanInterface::clearBuffers() {
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

    inline vk::CommandBuffer VulkanInterface::startCopyBuffer() {
        vk::CommandBufferAllocateInfo allocInfo;
        allocInfo.level              = vk::CommandBufferLevel::ePrimary;
        allocInfo.commandPool        = vk_commandPool;
        allocInfo.commandBufferCount = 1;

        std::vector<vk::CommandBuffer> commandBuffers = vk_device.allocateCommandBuffers(allocInfo);

        vk::CommandBufferBeginInfo beginInfo;
        beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

        commandBuffers.front().begin(beginInfo);

        return commandBuffers.front();
    }

    inline void VulkanInterface::copyBuffer(vk::Buffer src, vk::Buffer dst, vk::DeviceSize size, vk::CommandBuffer commandBuffer, vk::BufferCopy& copyRegion) {
        copyRegion.size = size;
        copyRegion.srcOffset = 0; 
        copyRegion.dstOffset = 0; 

        commandBuffer.copyBuffer(src, dst, 1, &copyRegion);
    }

    inline Result VulkanInterface::endSingleTimeCommands(vk::CommandBuffer commandBuffer) {
        commandBuffer.end();

        vk::SubmitInfo submitInfo;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers    = &commandBuffer;

        vk::Result result_submit = vk_graphicsQueue.submit(1, &submitInfo, nullptr);

        if (result_submit != vk::Result::eSuccess) {
            return Result::eFailedToSubmitQueue;
        }

        vk_graphicsQueue.waitIdle();

        vk_device.freeCommandBuffers(vk_commandPool, 1, &commandBuffer);

        return Result::eSuccess;
    }

    template <typename T>
    inline Result VulkanInterface::InstanceStagingBuffer(
        std::vector<T>    dataToUpload,
        vk::Buffer&       out_buffer,
        vk::DeviceMemory& out_memory,
        vk::DeviceSize&   out_size
    ) {
        
        out_size = sizeof(dataToUpload[0]) * dataToUpload.size();

        TRYR(bufferRes, createBuffer(
            out_size, 
            vk::BufferUsageFlagBits::eTransferSrc, 
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
            out_buffer,
            out_memory
        ));

        void* memory = vk_device.mapMemory(out_memory, 0, out_size);

        std::memcpy(memory, dataToUpload.data(), out_size);

        vk_device.unmapMemory(out_memory);

        return Result::eSuccess;
    }
}