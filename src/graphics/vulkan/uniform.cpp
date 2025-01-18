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

        void* vk_vertexUniformBufferMap = vk_device.mapMemory(vk_vertexUniformBufferMemory, 0, sizeof(VertexUniformObject));

        vertexUniformObject.screenSize = pWindow->getSize();
        
        memcpy(vk_vertexUniformBufferMap, &vertexUniformObject.screenSize, sizeof(IVec2));

        vk_device.unmapMemory(vk_vertexUniformBufferMemory);

        vk::DescriptorBufferInfo bufferInfo;
        bufferInfo.buffer = vk_vertexUniformBuffer;
        bufferInfo.offset = 0;
        bufferInfo.range  = sizeof(VertexUniformObject);

        vk::WriteDescriptorSet descriptorWrite;
        descriptorWrite.dstSet           = vk_descriptorSet;
        descriptorWrite.dstBinding       = DescriporBindings::eVertexUniform;
        descriptorWrite.dstArrayElement  = 0;
        descriptorWrite.descriptorType   = vk::DescriptorType::eUniformBuffer;
        descriptorWrite.descriptorCount  = 1;
        descriptorWrite.pBufferInfo      = &bufferInfo;
        descriptorWrite.pImageInfo       = nullptr;
        descriptorWrite.pTexelBufferView = nullptr;

        vk_device.updateDescriptorSets(1, &descriptorWrite, 0, nullptr);
    }

    inline void VulkanInterface::createScenesUniforms() { // memory leaks!!!!
        
        vk::DeviceSize scenesSize = scenes.size() * sizeof(Scene);

        createBuffer(
            scenesSize, 
            vk::BufferUsageFlagBits::eUniformBuffer, 
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, 
            vk_sceneBuffer, 
            vk_sceneBufferMemory
        );

        void* vk_sceneUniformBufferMap = vk_device.mapMemory(vk_sceneBufferMemory, 0, scenesSize);
        
        memcpy(vk_sceneUniformBufferMap, scenes.data(), scenesSize);

        vk_device.unmapMemory(vk_sceneBufferMemory);

        vk::DescriptorBufferInfo bufferInfo;
        bufferInfo.buffer = vk_sceneBuffer;
        bufferInfo.offset = 0;
        bufferInfo.range  = sizeof(VertexUniformObject);

        vk::WriteDescriptorSet descriptorWrite;
        descriptorWrite.dstSet           = vk_descriptorSet;
        descriptorWrite.dstBinding       = DescriporBindings::eSceneData;
        descriptorWrite.dstArrayElement  = 0;
        descriptorWrite.descriptorType   = vk::DescriptorType::eUniformBuffer;
        descriptorWrite.descriptorCount  = 1;
        descriptorWrite.pBufferInfo      = &bufferInfo;
        descriptorWrite.pImageInfo       = nullptr;
        descriptorWrite.pTexelBufferView = nullptr;

        vk_device.updateDescriptorSets(1, &descriptorWrite, 0, nullptr);
    }
}