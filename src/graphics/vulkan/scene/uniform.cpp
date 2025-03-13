#include "../vulkan.hpp"

namespace SplitGui {
    inline Result VulkanInterface::createSceneDataUniform(SceneObject& scene) {

        vk::DeviceSize   bufferSize = sizeof(SceneObj);
        vk::Buffer       stagingBuffer;
        vk::DeviceMemory stagingBufferMemory;

        TRYR(stagingRes, createBuffer(
            bufferSize,
            vk::BufferUsageFlagBits::eTransferSrc, 
            vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible,
            stagingBuffer,
            stagingBufferMemory
        ));

        void* memory = vk_device.mapMemory(stagingBufferMemory, 0, bufferSize);

        std::memcpy(memory, &scene.sceneData, bufferSize);

        vk_device.unmapMemory(stagingBufferMemory);

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
        
        copyBuffer(stagingBuffer,  tempBuffer, bufferSize,  commandBuffer, copyRegion);
        
        TRYR(endRes, endSingleTimeCommands(commandBuffer));

        if (scene.dataUniformBufferMemory) {
            vk_device.freeMemory(scene.dataUniformBufferMemory);
            vk_device.destroyBuffer(scene.dataUniformBuffer);
        }

        vk_device.waitIdle();
        
        scene.dataUniformBufferMemory = tempBufferMemory;
        scene.dataUniformBuffer       = tempBuffer;

        vk_device.freeMemory(stagingBufferMemory);
        vk_device.destroyBuffer(stagingBuffer);

        Logger::info("Created Scene Data Uniform");

        return Result::eSuccess;
    }

    inline Result VulkanInterface::createSceneModelUniform(SceneObject& scene) {

        vk::DeviceSize   bufferSize = sizeof(Mat4) * scene.models.size();
        vk::Buffer       stagingBuffer;
        vk::DeviceMemory stagingBufferMemory;

        TRYR(stagingRes, createBuffer(
            bufferSize,
            vk::BufferUsageFlagBits::eTransferSrc, 
            vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible,
            stagingBuffer,
            stagingBufferMemory
        ));

        void* memory = vk_device.mapMemory(stagingBufferMemory, 0, bufferSize);

        std::memcpy(memory, scene.models.data(), bufferSize);

        vk_device.unmapMemory(stagingBufferMemory);

        TRYR(bufferRes, createBuffer(
            bufferSize,
            vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eUniformBuffer, 
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            scene.modelUniformBuffer,
            scene.modelUniformBufferMemory
        ));

        vk::CommandBuffer commandBuffer = startCopyBuffer();

        vk::BufferCopy copyRegion;

        copyBuffer(stagingBuffer, scene.modelUniformBuffer, bufferSize,  commandBuffer, copyRegion);

        TRYR(endRes, endSingleTimeCommands(commandBuffer));

        vk_device.freeMemory(stagingBufferMemory);
        vk_device.destroyBuffer(stagingBuffer);

        Logger::info("Created Scene Data Uniform");

        return Result::eSuccess;
    }

}