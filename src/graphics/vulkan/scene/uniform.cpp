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

    inline Result VulkanInterface::createSceneModelUniform(SceneRef& ref) {
        SPLITGUI_PROFILE;

        SceneObject& scene = scenes[ref.sceneNumber];

        vk::DeviceSize bufferSize = sizeof(Mat4) * scene.models.size();

        TRYR(stagingBufferRes, InstanceStagingBuffer(scene.sceneModelStagingBuffer, sizeof(Mat4) * scenes[ref.sceneNumber].models.size()));

        void* memory = vk_device.mapMemory(scene.sceneModelStagingBuffer.memory, 0, bufferSize);

        std::memcpy(memory, scene.models.array(), bufferSize);

        vk_device.unmapMemory(scene.sceneModelStagingBuffer.memory);

        vk::DeviceMemory tempMem;
        vk::Buffer tempBuffer;

        TRYR(bufferRes, createBuffer(
            bufferSize,
            vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eUniformBuffer, 
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            tempBuffer,
            tempMem
        ));

        vk::CommandBuffer commandBuffer = startCopyBuffer();

        vk::BufferCopy copyRegion;

        copyBuffer(scene.sceneModelStagingBuffer.buffer, tempBuffer, bufferSize,  commandBuffer, copyRegion);

        TRYR(endRes, endSingleTimeCommands(commandBuffer));

        frameMutex.lock();

        vk_device.waitIdle();

        cleanupModelUniform(ref);

        scene.modelUniformBuffer = tempBuffer;
        scene.modelUniformBufferMemory = tempMem;

        frameMutex.unlock();

        vk::DescriptorBufferInfo ModelBufferInfo;
        ModelBufferInfo.buffer = scene.modelUniformBuffer;
        ModelBufferInfo.offset = 0;
        ModelBufferInfo.range  = bufferSize;

        std::array<vk::WriteDescriptorSet, 1> descriptorWrites;

        descriptorWrites[0].dstSet          = scene.descriptorSet;
        descriptorWrites[0].dstBinding      = SceneDescriporBindings::eModels;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType  = vk::DescriptorType::eUniformBuffer;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo     = &ModelBufferInfo;

        vk_device.updateDescriptorSets(descriptorWrites, nullptr);

        Logger::info("Created Scene Data Uniform");

        return Result::eSuccess;
    }

}