#include "vulkan.hpp"

namespace SplitGui {
    void VulkanInterface::submitBuffers() {

        if (indices.size() == knownIndicesSize) {
            return;
        }

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
}