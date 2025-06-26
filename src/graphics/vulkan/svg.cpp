#include "vulkan.hpp"

namespace SplitGui {
    ResultValue<TextureRef> VulkanInterface::rasterizeSvg(const std::string& svg) {

        char* string = new char[svg.size() + 1];
        ma::track(string, "SVG data");
        std::memcpy(string, svg.c_str(), svg.size());
        string[svg.size()] = '\0';

        NSVGimage* image = nsvgParse(string, "px", 96);
        ma::untrack(string);
        delete[] string;

        if (!image) {
            return Result::eFailedToParseSvg;
        }

        float scale = std::min(
            vk_msdfExtent.width / image->width,
            vk_msdfExtent.height / image->height
        );

        int numPixels = 4 * vk_msdfExtent.width * vk_msdfExtent.height;
        TRYR(bufferRes, InstanceStagingBuffer(vk_textureArrayStagingBuffer, numPixels * sizeof(uint8_t)));

        uint8_t* memory = (uint8_t*)vk_device.mapMemory(vk_textureArrayStagingBuffer.memory, 0, vk_textureArrayStagingBuffer.size);

        nsvgRasterize(ns_rasterizer, image, 0, 0, scale, memory, vk_msdfExtent.width, vk_msdfExtent.height, vk_msdfExtent.width * 4);

        vk_device.unmapMemory(vk_textureArrayStagingBuffer.memory);

        vk::CommandBuffer commandBuffer = startCopyBuffer();

        vk::ImageMemoryBarrier barrier1 = {};
        barrier1.srcAccessMask                   = (vk::AccessFlagBits) 0;
        barrier1.dstAccessMask                   = vk::AccessFlagBits::eTransferWrite;
        barrier1.oldLayout                       = vk::ImageLayout::eUndefined;
        barrier1.newLayout                       = vk::ImageLayout::eTransferDstOptimal;
        barrier1.srcQueueFamilyIndex             = vk::QueueFamilyIgnored;
        barrier1.dstQueueFamilyIndex             = vk::QueueFamilyIgnored;
        barrier1.image                           = vk_textureArrayImages;
        barrier1.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
        barrier1.subresourceRange.baseArrayLayer = textures;
        barrier1.subresourceRange.layerCount     = 1;
        barrier1.subresourceRange.levelCount     = 1;

        commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTransfer, (vk::DependencyFlagBits)0, nullptr, nullptr, barrier1);

        vk::BufferImageCopy copyRegion = {};
        copyRegion.bufferOffset                    = 0;
        copyRegion.bufferRowLength                 = 0;
        copyRegion.bufferImageHeight               = 0;
        copyRegion.imageSubresource.aspectMask     = vk::ImageAspectFlagBits::eColor;
        copyRegion.imageSubresource.mipLevel       = 0;
        copyRegion.imageSubresource.baseArrayLayer = textures; // layer set to index
        copyRegion.imageSubresource.layerCount     = 1;
        copyRegion.imageOffset.x                   = 0;
        copyRegion.imageOffset.y                   = 0;
        copyRegion.imageOffset.z                   = 0;
        copyRegion.imageExtent.width               = vk_msdfExtent.width;
        copyRegion.imageExtent.height              = vk_msdfExtent.height;
        copyRegion.imageExtent.depth               = 1;

        commandBuffer.copyBufferToImage(vk_textureArrayStagingBuffer.buffer, vk_textureArrayImages, vk::ImageLayout::eTransferDstOptimal, 1, &copyRegion);

        vk::ImageMemoryBarrier barrier2 = {};
        barrier2.srcAccessMask                   = vk::AccessFlagBits::eTransferWrite;
        barrier2.dstAccessMask                   = vk::AccessFlagBits::eShaderRead;
        barrier2.oldLayout                       = vk::ImageLayout::eTransferDstOptimal;
        barrier2.newLayout                       = vk::ImageLayout::eShaderReadOnlyOptimal;
        barrier2.srcQueueFamilyIndex             = vk::QueueFamilyIgnored;
        barrier2.dstQueueFamilyIndex             = vk::QueueFamilyIgnored;
        barrier2.image                           = vk_textureArrayImages;
        barrier2.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
        barrier2.subresourceRange.baseArrayLayer = textures;
        barrier2.subresourceRange.layerCount     = 1;
        barrier2.subresourceRange.levelCount     = 1;

        commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, vk::DependencyFlagBits::eByRegion, nullptr, nullptr, barrier2);

        TRYR(commandRes, endSingleTimeCommands(commandBuffer));

        Logger::info("Rasterized SVG");

        nsvgDelete(image);

        TextureRef ref;
        ref.textureNumber = textures++;

        return ref;
    }
}