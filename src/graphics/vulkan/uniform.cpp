#include "vulkan.hpp"

namespace SplitGui {
    inline void VulkanInterface::createVertexUniforms() { // might remove due to dead code and obsolete

        createBuffer(
            sizeof(VertexUniformObject),
            vk::BufferUsageFlagBits::eUniformBuffer,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
            vk_vertexUniformBuffer,
            vk_vertexUniformBufferMemory
        );

        void* vk_vertexUniformBufferMap = vk_device.mapMemory(vk_vertexUniformBufferMemory, 0, sizeof(VertexUniformObject));

        vertexUniformObject.screenSize = pWindow->getSize();
        
        memcpy(vk_vertexUniformBufferMap, &vertexUniformObject.screenSize, sizeof(IVec2));

        vk_device.unmapMemory(vk_vertexUniformBufferMemory);
    }
}