#include "vulkan.hpp"

namespace SplitGui {
    ResultValue<CropRegionRef> VulkanInterface::createCropRegion(IVec2 x1, IVec2 x2) {
        SPLITGUI_PROFILE;

        CropRegionObject object;
        object.x1 = x1;
        object.x2 = x2;

        CropRegionRef ref;
        ref.cropRegion = cropRegions.push(object);

        vk::DeviceSize bufferSize = sizeof(CropRegionObject) * cropRegions.size();

        TRYR(stagingBufferRes, InstanceStagingBuffer(vk_cropRegionArrayStagingBuffer, bufferSize));

        void* memory = vk_device.mapMemory(vk_cropRegionArrayStagingBuffer.memory, 0, bufferSize);

        std::memcpy(memory, cropRegions.array(), bufferSize);

        vk_device.unmapMemory(vk_cropRegionArrayStagingBuffer.memory);

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

        copyBuffer(vk_cropRegionArrayStagingBuffer.buffer, tempBuffer, bufferSize,  commandBuffer, copyRegion);

        TRYR(endRes, endSingleTimeCommands(commandBuffer));

        frameMutex.lock();
        vk_device.waitIdle();

        cleanupCropRegionResources();

        vk_cropRegionArrayBuffer = tempBuffer;
        vk_cropRegionArrayBufferMemory = tempMem;

        frameMutex.unlock();

        vk::DescriptorBufferInfo cropRegionsBufferInfo;
        cropRegionsBufferInfo.buffer = vk_cropRegionArrayBuffer;
        cropRegionsBufferInfo.offset = 0;
        cropRegionsBufferInfo.range  = bufferSize;

        std::array<vk::WriteDescriptorSet, 1> descriptorWrites;

        descriptorWrites[0].dstSet          = vk_descriptorSet;
        descriptorWrites[0].dstBinding      = DescriporBindings::eCropRegions;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType  = vk::DescriptorType::eUniformBuffer;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo     = &cropRegionsBufferInfo;

        vk_device.updateDescriptorSets(descriptorWrites, nullptr);

        Logger::info("Created crop region");

        return ref;
    }

    Result VulkanInterface::updateCropRegionDescriptor() {

        vk::DeviceSize bufferSize = sizeof(CropRegionObject);

        TRYR(stagingBufferRes, InstanceStagingBuffer(vk_cropRegionArrayStagingBuffer, bufferSize));

        void* memory = vk_device.mapMemory(vk_cropRegionArrayStagingBuffer.memory, 0, bufferSize);

        CropRegionObject b = {};
        std::memcpy(memory, &b, bufferSize);

        vk_device.unmapMemory(vk_cropRegionArrayStagingBuffer.memory);

        TRYR(bufferRes, createBuffer(
            bufferSize,
            vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eUniformBuffer, 
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            vk_cropRegionArrayBuffer,
            vk_cropRegionArrayBufferMemory
        ));

        vk::CommandBuffer commandBuffer = startCopyBuffer();

        vk::BufferCopy copyRegion;

        copyBuffer(vk_cropRegionArrayStagingBuffer.buffer, vk_cropRegionArrayBuffer, bufferSize,  commandBuffer, copyRegion);

        TRYR(endRes, endSingleTimeCommands(commandBuffer));

        vk::DescriptorBufferInfo cropRegionsBufferInfo;
        cropRegionsBufferInfo.buffer = vk_cropRegionArrayBuffer;
        cropRegionsBufferInfo.offset = 0;
        cropRegionsBufferInfo.range  = bufferSize;

        std::array<vk::WriteDescriptorSet, 1> descriptorWrites;

        descriptorWrites[0].dstSet          = vk_descriptorSet;
        descriptorWrites[0].dstBinding      = DescriporBindings::eCropRegions;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType  = vk::DescriptorType::eUniformBuffer;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo     = &cropRegionsBufferInfo;

        vk_device.updateDescriptorSets(descriptorWrites, nullptr);

        return Result::eSuccess;
    }

    Result VulkanInterface::updateCropRegion(CropRegionRef& ref, IVec2 x1, IVec2 x2) {
        SPLITGUI_PROFILE;

        if (ref.cropRegion->data.x1 == x1 && ref.cropRegion->data.x2 == x2) {
            return Result::eSuccess;
        }

        ref.cropRegion->data.x1 = x1;
        ref.cropRegion->data.x2 = x2;

        vk::DeviceSize bufferSize = sizeof(CropRegionObject) * cropRegions.size();

        TRYR(stagingBufferRes, InstanceStagingBuffer(vk_cropRegionArrayStagingBuffer, bufferSize));

        std::optional<unsigned int> cropOff = cropRegions.offset(ref.cropRegion);

        if (!cropOff.has_value()) {
            return Result::eInvalidCropRegionRefUsed;
        }
        
        void* memory = vk_device.mapMemory(vk_cropRegionArrayStagingBuffer.memory, cropOff.value(), sizeof(CropRegionObject));

        std::memcpy(memory, &ref.cropRegion->data, sizeof(CropRegionObject));

        vk_device.unmapMemory(vk_cropRegionArrayStagingBuffer.memory);

        vk::CommandBuffer commandBuffer = startCopyBuffer();

        vk::BufferCopy copyRegion;
        copyRegion.size      = sizeof(CropRegionObject);
        copyRegion.srcOffset = sizeof(CropRegionObject) * cropOff.value();
        copyRegion.dstOffset = sizeof(CropRegionObject) * cropOff.value();

        commandBuffer.copyBuffer(vk_cropRegionArrayStagingBuffer.buffer, vk_cropRegionArrayBuffer, 1, &copyRegion);

        TRYR(commandRes, endSingleTimeCommands(commandBuffer));

        return Result::eSuccess;
    }
}