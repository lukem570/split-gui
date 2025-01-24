#include "vulkan.hpp"

namespace SplitGui {
    inline void VulkanInterface::vertexBufferSubmit() {

        vk::DeviceSize   indexBufferSize;
        vk::Buffer       stagingIndexBuffer;
        vk::DeviceMemory stagingIndexBufferMemory;

        vk::DeviceSize   vertexBufferSize;
        vk::Buffer       stagingVertexBuffer;
        vk::DeviceMemory stagingVertexBufferMemory;

        InstanceStagingBuffer(indices,  stagingIndexBuffer,  stagingIndexBufferMemory,  indexBufferSize );
        InstanceStagingBuffer(vertices, stagingVertexBuffer, stagingVertexBufferMemory, vertexBufferSize);

        vk::Buffer       tempIndexBuffer;
        vk::DeviceMemory tempIndexBufferMemory;

        vk::Buffer       tempVertexBuffer;
        vk::DeviceMemory tempVertexBufferMemory;

        createBuffer(
            indexBufferSize, 
            vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, 
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            tempIndexBuffer,
            tempIndexBufferMemory
        );

        createBuffer(
            vertexBufferSize, 
            vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, 
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            tempVertexBuffer,
            tempVertexBufferMemory
        );

        vk::CommandBuffer commandBuffer = startCopyBuffer();
        vk::BufferCopy copyRegionIndex;
        vk::BufferCopy copyRegionVertex;

        copyBuffer(stagingIndexBuffer,  tempIndexBuffer, indexBufferSize,  commandBuffer, copyRegionIndex);
        copyBuffer(stagingVertexBuffer, tempVertexBuffer, vertexBufferSize, commandBuffer, copyRegionVertex);

        endCopyBuffer(commandBuffer);

        vk_runtimeResult = vk_device.waitForFences(vk_inFlightFences.size(), vk_inFlightFences.data(), true, UINT64_MAX);

        if (vk_runtimeResult != vk::Result::eSuccess) {
            printf("Error Waiting for fences\n");
            throw;
        }
        
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
    }

    inline void VulkanInterface::scenesSubmit() {
        
        vk::DeviceSize   sceneBufferSize;
        vk::Buffer       stagingBuffer;
        vk::DeviceMemory stagingBufferMemory;

        InstanceStagingBuffer(scenes, stagingBuffer, stagingBufferMemory, sceneBufferSize);

        vk::Buffer       tempBuffer;
        vk::DeviceMemory tempBufferMemory;

        createBuffer(
            sceneBufferSize, 
            vk::BufferUsageFlagBits::eUniformBuffer | vk::BufferUsageFlagBits::eTransferDst, 
            vk::MemoryPropertyFlagBits::eDeviceLocal, 
            tempBuffer, 
            tempBufferMemory
        );

        vk::CommandBuffer commandBuffer = startCopyBuffer();

        vk::BufferCopy copyRegion;

        copyBuffer(stagingBuffer,  tempBuffer, sceneBufferSize,  commandBuffer, copyRegion);

        endCopyBuffer(commandBuffer);

        vk_runtimeResult = vk_device.waitForFences(vk_inFlightFences.size(), vk_inFlightFences.data(), true, UINT64_MAX);

        if (vk_runtimeResult != vk::Result::eSuccess) {
            printf("Error Waiting for fences\n");
            throw;
        }

        cleanupSceneBuffer();

        vk_sceneBuffer       = tempBuffer;
        vk_sceneBufferMemory = tempBufferMemory;

        vk_device.destroyBuffer(stagingBuffer);
        vk_device.freeMemory(stagingBufferMemory);

        vk::DescriptorBufferInfo bufferInfo;
        bufferInfo.buffer = vk_sceneBuffer;
        bufferInfo.offset = 0;
        bufferInfo.range  = sizeof(SceneObj);

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

        knownScenesSize = scenes.size();
    }

    void VulkanInterface::submitBuffers() {

        if (indices.size() != knownIndicesSize) {
            vertexBufferSubmit();
        }
        
        if (scenes.size() != knownScenesSize) {
            scenesSubmit();
        }
    }
}