#include "vulkan.hpp"

namespace SplitGui {
    inline void VulkanInterface::createVertexUniformBuffer() {
        
        vk::Buffer       stagingBuffer;
        vk::DeviceMemory stagingBufferMemory;

        createBuffer(
            sizeof(VertexUniformObject), 
            vk::BufferUsageFlagBits::eTransferSrc, 
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
            stagingBuffer,
            stagingBufferMemory
        );

        VertexUniformObject vertexUniformObject;
        vertexUniformObject.screenSize = pWindow->getSize();


        void* memory = vk_device.mapMemory(stagingBufferMemory, 0, sizeof(VertexUniformObject));

        memcpy(memory, &vertexUniformObject, sizeof(VertexUniformObject));

        vk_device.unmapMemory(stagingBufferMemory);

        createBuffer(
            sizeof(VertexUniformObject), 
            vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eUniformBuffer, 
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            vk_vertexUniformBuffer,
            vk_vertexUniformBufferMemory
        );

        vk::CommandBuffer commandBuffer = startCopyBuffer();

        vk::BufferCopy copyRegion;

        copyBuffer(stagingBuffer,  vk_vertexUniformBuffer, sizeof(VertexUniformObject),  commandBuffer, copyRegion);

        endCopyBuffer(commandBuffer);

        vk_device.freeMemory(stagingBufferMemory);
        vk_device.destroyBuffer(stagingBuffer);
    }
}