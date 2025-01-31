#include "vulkan.hpp"

namespace SplitGui {
    inline Result VulkanInterface::vertexBufferSubmit() {

        vk::DeviceSize   indexBufferSize;
        vk::Buffer       stagingIndexBuffer;
        vk::DeviceMemory stagingIndexBufferMemory;

        vk::DeviceSize   vertexBufferSize;
        vk::Buffer       stagingVertexBuffer;
        vk::DeviceMemory stagingVertexBufferMemory;

        TRYR(stagingRes1, InstanceStagingBuffer(indices,  stagingIndexBuffer,  stagingIndexBufferMemory,  indexBufferSize ));
        TRYR(stagingRes2, InstanceStagingBuffer(vertices, stagingVertexBuffer, stagingVertexBufferMemory, vertexBufferSize));

        vk::Buffer       tempIndexBuffer;
        vk::DeviceMemory tempIndexBufferMemory;

        vk::Buffer       tempVertexBuffer;
        vk::DeviceMemory tempVertexBufferMemory;

        TRYR(bufferRes1, createBuffer(
            indexBufferSize, 
            vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, 
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            tempIndexBuffer,
            tempIndexBufferMemory
        ));

        TRYR(bufferRes2, createBuffer(
            vertexBufferSize, 
            vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, 
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            tempVertexBuffer,
            tempVertexBufferMemory
        ));

        vk::CommandBuffer commandBuffer = startCopyBuffer();
        vk::BufferCopy copyRegionIndex;
        vk::BufferCopy copyRegionVertex;

        copyBuffer(stagingIndexBuffer,  tempIndexBuffer, indexBufferSize,  commandBuffer, copyRegionIndex);
        copyBuffer(stagingVertexBuffer, tempVertexBuffer, vertexBufferSize, commandBuffer, copyRegionVertex);

        TRYR(commandRes, endSingleTimeCommands(commandBuffer));

        vk_device.waitIdle();
        
        cleanupVertexAndIndexBuffers();

        vk_vertexBuffer       = tempVertexBuffer;
        vk_vertexBufferMemory = tempVertexBufferMemory;

        vk_indexBuffer        = tempIndexBuffer;
        vk_indexBufferMemory  = tempIndexBufferMemory;

        vk_device.destroyBuffer(stagingVertexBuffer);
        vk_device.freeMemory(stagingVertexBufferMemory);

        vk_device.destroyBuffer(stagingIndexBuffer);
        vk_device.freeMemory(stagingIndexBufferMemory);
        
        knownIndicesSize = indices.size();

        return Result::eSuccess;
    }

    inline void VulkanInterface::updateScenes() {
        vk::DescriptorBufferInfo bufferInfo;
        bufferInfo.buffer = vk_sceneBuffer;
        bufferInfo.offset = 0;
        bufferInfo.range  = vk_sceneBufferSize;

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

    inline Result VulkanInterface::scenesSubmit() {
    
        vk::Buffer       stagingBuffer;
        vk::DeviceMemory stagingBufferMemory;

        TRYR(stagingRes, InstanceStagingBuffer<SceneObj>(scenes, stagingBuffer, stagingBufferMemory, vk_sceneBufferSize));

        vk::Buffer       tempBuffer;
        vk::DeviceMemory tempBufferMemory;

        TRYR(bufferRes, createBuffer(
            vk_sceneBufferSize, 
            vk::BufferUsageFlagBits::eUniformBuffer | vk::BufferUsageFlagBits::eTransferDst, 
            vk::MemoryPropertyFlagBits::eDeviceLocal, 
            tempBuffer, 
            tempBufferMemory
        ));

        vk::CommandBuffer commandBuffer = startCopyBuffer();

        vk::BufferCopy copyRegion;

        copyBuffer(stagingBuffer,  tempBuffer, vk_sceneBufferSize,  commandBuffer, copyRegion);

        TRYR(commandRes, endSingleTimeCommands(commandBuffer));

        vk_device.waitIdle();

        cleanupSceneBuffer();

        vk_sceneBuffer       = tempBuffer;
        vk_sceneBufferMemory = tempBufferMemory;

        vk_device.destroyBuffer(stagingBuffer);
        vk_device.freeMemory(stagingBufferMemory);

        updateScenes();

        knownScenesSize = scenes.size();

        return Result::eSuccess;
    }

    Result VulkanInterface::submitBuffers() {

        if (indices.size() != knownIndicesSize || markVerticesForUpdate) {
            markVerticesForUpdate = false;
            TRYR(vertexRes, vertexBufferSubmit());
        }
        
        if (scenes.size() != knownScenesSize || markScenesForUpdate) {
            markScenesForUpdate = false;
            TRYR(sceneRes, scenesSubmit());
        }

        return Result::eSuccess;
    }
}