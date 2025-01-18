#include "vulkan.hpp"

namespace SplitGui {
    inline void VulkanInterface::createVertexUniforms() {
        createBuffer(
            sizeof(VertexUniformObject),
            vk::BufferUsageFlagBits::eUniformBuffer,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
            vk_vertexUniformBuffer,
            vk_vertexUniformBufferMemory
        );

        vk_vertexUniformBufferMap = vk_device.mapMemory(vk_vertexUniformBufferMemory, 0, sizeof(VertexUniformObject));
    }

    inline void VulkanInterface::createScenesUniforms() {
        
        vk::DeviceSize scenesSize = scenes.size() * sizeof(Scene);

        vk::Buffer sceneBuffer;
        vk::DeviceMemory sceneBufferMemory;

        createBuffer(
            scenesSize, 
            vk::BufferUsageFlagBits::eUniformBuffer, 
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, 
            sceneBuffer, 
            sceneBufferMemory
        );
    }
}