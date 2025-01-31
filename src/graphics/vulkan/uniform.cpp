#include "vulkan.hpp"

namespace SplitGui {
    inline Result VulkanInterface::createVertexUniformBuffer() {
        
        if (vk_vertexUniformBufferMemory) {
            vk_device.freeMemory(vk_vertexUniformBufferMemory);
            vk_device.destroyBuffer(vk_vertexUniformBuffer);
        }

        vk::Buffer       stagingBuffer;
        vk::DeviceMemory stagingBufferMemory;

        TRYR(bufferRes1, createBuffer(
            sizeof(VertexUniformObject), 
            vk::BufferUsageFlagBits::eTransferSrc, 
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
            stagingBuffer,
            stagingBufferMemory
        ));

        VertexUniformObject vertexUniformObject;
        vertexUniformObject.screenSize = pWindow->getSize();

        void* memory = vk_device.mapMemory(stagingBufferMemory, 0, sizeof(VertexUniformObject));

        memcpy(memory, &vertexUniformObject, sizeof(VertexUniformObject));

        vk_device.unmapMemory(stagingBufferMemory);

        TRYR(bufferRes2, createBuffer(
            sizeof(VertexUniformObject), 
            vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eUniformBuffer, 
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            vk_vertexUniformBuffer,
            vk_vertexUniformBufferMemory
        ));

        vk::CommandBuffer commandBuffer = startCopyBuffer();

        vk::BufferCopy copyRegion;

        copyBuffer(stagingBuffer,  vk_vertexUniformBuffer, sizeof(VertexUniformObject),  commandBuffer, copyRegion);

        TRYR(commandRes, endSingleTimeCommands(commandBuffer));

        vk_device.freeMemory(stagingBufferMemory);
        vk_device.destroyBuffer(stagingBuffer);

        return Result::eSuccess;
    }
}