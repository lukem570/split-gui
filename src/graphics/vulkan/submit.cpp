#include "vulkan.hpp"

namespace SplitGui {
    inline Result VulkanInterface::vertexBufferSubmit() {
        SPLITGUI_PROFILE;

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

        frameMutex.lock();

        vk::Result fenceRes = vk_device.waitForFences(vk_inFlightFences.size(), vk_inFlightFences.data(), vk::True, UINT64_MAX);

        if (fenceRes != vk::Result::eSuccess) {
            return Result::eFailedToWaitForFences;
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
        
        frameMutex.unlock();
        
        Logger::info("Submitted Vertex Buffer");

        return Result::eSuccess;
    }

    Result VulkanInterface::submitBuffers() {
        SPLITGUI_PROFILE;

        if (indices.size() != knownIndicesSize || markVerticesForUpdate) {
            markVerticesForUpdate = false;
            TRYR(vertexRes, vertexBufferSubmit());
        }

        Logger::info("Submited Buffers");

        return Result::eSuccess;
    }
}