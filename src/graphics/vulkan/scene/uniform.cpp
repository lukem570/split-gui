#include "../vulkan.hpp"

namespace SplitGui {
    inline Result VulkanInterface::createSceneDataUniform(SceneObject& scene) {
        SPLITGUI_PROFILE;

        vk::DeviceSize bufferSize = sizeof(SceneObj);

        TRYR(stagingBufferRes, InstanceStagingBuffer(scene.sceneDataStagingBuffer, bufferSize));

        void* memory = vk_device.mapMemory(scene.sceneDataStagingBuffer.memory, 0, bufferSize);

        std::memcpy(memory, &scene.sceneData, bufferSize);

        vk_device.unmapMemory(scene.sceneDataStagingBuffer.memory);

        vk::Buffer       tempBuffer;
        vk::DeviceMemory tempBufferMemory;
        
        TRYR(bufferRes, createBuffer(
            bufferSize,
            vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eUniformBuffer, 
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            tempBuffer,
            tempBufferMemory
        ));
        
        vk::CommandBuffer commandBuffer = startCopyBuffer();
        
        vk::BufferCopy copyRegion;
        
        copyBuffer(scene.sceneDataStagingBuffer.buffer,  tempBuffer, bufferSize,  commandBuffer, copyRegion);
        
        TRYR(endRes, endSingleTimeCommands(commandBuffer));

        if (scene.dataUniformBufferMemory) {
            vk_device.freeMemory(scene.dataUniformBufferMemory);
            vk_device.destroyBuffer(scene.dataUniformBuffer);
        }

        vk_device.waitIdle();
        
        scene.dataUniformBufferMemory = tempBufferMemory;
        scene.dataUniformBuffer       = tempBuffer;

        Logger::info("Created Scene Data Uniform");

        return Result::eSuccess;
    }

    inline Result VulkanInterface::createSceneModelUniform(SceneObject& scene) {
        SPLITGUI_PROFILE;

        vk::DeviceSize bufferSize = sizeof(Mat4) * scene.models.size();

        TRYR(stagingBufferRes, InstanceStagingBuffer(scene.sceneModelStagingBuffer, sizeof(Mat4) * scene.models.size()));

        void* memory = vk_device.mapMemory(scene.sceneModelStagingBuffer.memory, 0, bufferSize);

        std::memcpy(memory, scene.models.data(), bufferSize);

        vk_device.unmapMemory(scene.sceneModelStagingBuffer.memory);

        TRYR(bufferRes, createBuffer(
            bufferSize,
            vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eUniformBuffer, 
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            scene.modelUniformBuffer,
            scene.modelUniformBufferMemory
        ));

        vk::CommandBuffer commandBuffer = startCopyBuffer();

        vk::BufferCopy copyRegion;

        copyBuffer(scene.sceneModelStagingBuffer.buffer, scene.modelUniformBuffer, bufferSize,  commandBuffer, copyRegion);

        TRYR(endRes, endSingleTimeCommands(commandBuffer));

        Logger::info("Created Scene Data Uniform");

        return Result::eSuccess;
    }

}