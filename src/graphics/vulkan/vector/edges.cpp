#include "../vulkan.hpp"

namespace SplitGui {
    inline Result VulkanInterface::submitVectorEngineEdgeResources(VectorEngineObject& vEngine) {
        SPLITGUI_PROFILE;

        vk::DeviceSize edgeBufferSize = vEngine.edges.size();

        TRYR(stagingRes, InstanceStagingBuffer(vEngine.edgeStagingBuffer, edgeBufferSize));

        void* edgeMemory = vk_device.mapMemory(vEngine.edgeStagingBuffer.memory, 0, edgeBufferSize);

        std::memcpy(edgeMemory, vEngine.edges.array(), edgeBufferSize);

        vk_device.unmapMemory(vEngine.edgeStagingBuffer.memory);

        vk::Buffer       tempEdgeBuffer;
        vk::DeviceMemory tempEdgeBufferMemory;

        vk::Buffer       tempTransformedEdgeBuffer;
        vk::DeviceMemory tempTransformedEdgeBufferMemory;

        TRYR(bufferRes1, createBuffer(
            edgeBufferSize, 
            vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, 
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            tempEdgeBuffer,
            tempEdgeBufferMemory
        ));

        TRYR(bufferRes2, createBuffer(
            edgeBufferSize, 
            vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, 
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            tempTransformedEdgeBuffer,
            tempTransformedEdgeBufferMemory
        ));

        vk::CommandBuffer commandBuffer = startCopyBuffer();
        vk::BufferCopy copyRegion;

        copyBuffer(vEngine.edgeStagingBuffer.buffer, tempEdgeBuffer, edgeBufferSize, commandBuffer, copyRegion);

        TRYR(commandRes, endSingleTimeCommands(commandBuffer));

        frameMutex.lock();

        vk::Result fenceRes = vk_device.waitForFences(vk_inFlightFences.size(), vk_inFlightFences.data(), vk::True, UINT64_MAX);

        if (fenceRes != vk::Result::eSuccess) {
            return Result::eFailedToWaitForFences;
        }

        cleanupVectorEngineEdgeResources(vEngine);

        vEngine.edgeBuffer       = tempEdgeBuffer;
        vEngine.edgeBufferMemory = tempEdgeBufferMemory;

        vEngine.transformedEdgeBuffer       = tempTransformedEdgeBuffer;
        vEngine.transformedEdgeBufferMemory = tempTransformedEdgeBufferMemory;

        frameMutex.unlock();

        Logger::info("Submitted Vector Engine Edge Buffer");

        return Result::eSuccess;
    }
}