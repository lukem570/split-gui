#include "vulkan.hpp"

namespace SplitGui {
    inline Result VulkanInterface::vertexBufferSubmit() {
        SPLITGUI_PROFILE;

        vk::DeviceSize indexBufferSize  = indices.size() * sizeof(uint16_t);
        vk::DeviceSize vertexBufferSize = vertices.size() * sizeof(VertexBufferObject);

        TRYR(stagingRes1, InstanceStagingBuffer(vk_indexStagingBuffer,  indexBufferSize ));
        TRYR(stagingRes2, InstanceStagingBuffer(vk_vertexStagingBuffer, vertexBufferSize));

        void* vertexMemory = vk_device.mapMemory(vk_vertexStagingBuffer.memory, 0, vertexBufferSize);

        std::memcpy(vertexMemory, vertices.data(), vertexBufferSize);

        vk_device.unmapMemory(vk_vertexStagingBuffer.memory);

        void* indexMemory = vk_device.mapMemory(vk_indexStagingBuffer.memory, 0, indexBufferSize);

        std::memcpy(indexMemory, indices.data(), indexBufferSize);
        
        vk_device.unmapMemory(vk_indexStagingBuffer.memory);

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

        vk::CommandBuffer commandBuffer = startCopyBuffer(vk_indexStagingBuffer);
        vk::BufferCopy copyRegionIndex;
        vk::BufferCopy copyRegionVertex;

        copyBuffer(vk_indexStagingBuffer.buffer,  tempIndexBuffer, indexBufferSize,  commandBuffer, copyRegionIndex);
        copyBuffer(vk_vertexStagingBuffer.buffer, tempVertexBuffer, vertexBufferSize, commandBuffer, copyRegionVertex);

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