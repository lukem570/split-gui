#include "vulkan.hpp"

namespace SplitGui {
    inline void transitionImageLayout(vk::CommandBuffer commandBuffer, vk::Image image, int layerCount) {
        vk::ImageMemoryBarrier barrier;
        barrier.oldLayout                       = vk::ImageLayout::eUndefined;
        barrier.newLayout                       = vk::ImageLayout::eShaderReadOnlyOptimal;
        barrier.srcQueueFamilyIndex             = vk::QueueFamilyIgnored;
        barrier.dstQueueFamilyIndex             = vk::QueueFamilyIgnored;
        barrier.image                           = image;
        barrier.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
        barrier.subresourceRange.baseMipLevel   = 0;
        barrier.subresourceRange.levelCount     = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount     = layerCount;
        barrier.srcAccessMask                   = vk::AccessFlagBits(0);
        barrier.dstAccessMask                   = vk::AccessFlagBits::eShaderRead;

        vk::PipelineStageFlags sourceStage;
        vk::PipelineStageFlags destinationStage;

        sourceStage      = vk::PipelineStageFlagBits::eTopOfPipe;
        destinationStage = vk::PipelineStageFlagBits::eFragmentShader;

        commandBuffer.pipelineBarrier(sourceStage, destinationStage, vk::DependencyFlagBits(0), 0, nullptr, 0, nullptr, 1, &barrier);
    }

    inline Result VulkanInterface::createTextGlyphImage() {
        vk::ImageCreateInfo imageInfo;
        imageInfo.imageType     = vk::ImageType::e2D;
        imageInfo.format        = vk::Format::eR8G8B8A8Unorm;
        imageInfo.extent.width  = vk_msdfExtent.width;
        imageInfo.extent.height = vk_msdfExtent.height;
        imageInfo.extent.depth  = 1;
        imageInfo.mipLevels     = 1;
        imageInfo.arrayLayers   = UINT8_MAX + 1;
        imageInfo.samples       = vk::SampleCountFlagBits::e1;
        imageInfo.tiling        = vk::ImageTiling::eOptimal;
        imageInfo.usage         = vk::ImageUsageFlagBits::eTransferDst;
        imageInfo.usage        |= vk::ImageUsageFlagBits::eSampled;
        imageInfo.sharingMode   = vk::SharingMode::eExclusive;
        imageInfo.initialLayout = vk::ImageLayout::eUndefined;

        vk_textGlyphImages = vk_device.createImage(imageInfo);

        vk::MemoryRequirements memoryRequirements = vk_device.getImageMemoryRequirements(vk_textGlyphImages);

        ResultValue<uint32_t> memType = findMemoryType(memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);

        TRYD(memType);

        vk::MemoryAllocateInfo allocateInfo;
        allocateInfo.allocationSize = memoryRequirements.size;
        allocateInfo.memoryTypeIndex = memType.value;

        vk_textGlyphImageMemory = vk_device.allocateMemory(allocateInfo);
        vk_device.bindImageMemory(vk_textGlyphImages, vk_textGlyphImageMemory, 0);

        vk::SamplerCreateInfo samplerInfo;
        samplerInfo.magFilter               = vk::Filter::eLinear;
        samplerInfo.minFilter               = vk::Filter::eLinear;
        samplerInfo.mipmapMode              = vk::SamplerMipmapMode::eNearest;
        samplerInfo.addressModeU            = vk::SamplerAddressMode::eClampToBorder;
        samplerInfo.addressModeV            = vk::SamplerAddressMode::eClampToBorder;
        samplerInfo.addressModeW            = vk::SamplerAddressMode::eClampToBorder;
        samplerInfo.mipLodBias              = 0.0f;
        samplerInfo.anisotropyEnable        = vk::False;
        samplerInfo.maxAnisotropy           = 1.0f;
        samplerInfo.compareEnable           = vk::False;
        samplerInfo.compareOp               = vk::CompareOp::eNever;
        samplerInfo.borderColor             = vk::BorderColor::eIntOpaqueBlack;
        samplerInfo.unnormalizedCoordinates = vk::False;
        samplerInfo.minLod                  = 0.0f;
        samplerInfo.maxLod                  = 1.0f;

        vk_textGlyphSampler = vk_device.createSampler(samplerInfo);

        vk::ImageViewCreateInfo imageViewInfo;
        imageViewInfo.image                           = vk_textGlyphImages;
        imageViewInfo.viewType                        = vk::ImageViewType::e2DArray;
        imageViewInfo.format                          = imageInfo.format;
        imageViewInfo.components.r                    = vk::ComponentSwizzle::eIdentity;
        imageViewInfo.components.g                    = vk::ComponentSwizzle::eIdentity;
        imageViewInfo.components.b                    = vk::ComponentSwizzle::eIdentity;
        imageViewInfo.components.a                    = vk::ComponentSwizzle::eIdentity;
        imageViewInfo.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
        imageViewInfo.subresourceRange.baseMipLevel   = 0;
        imageViewInfo.subresourceRange.levelCount     = 1;
        imageViewInfo.subresourceRange.baseArrayLayer = 0;
        imageViewInfo.subresourceRange.layerCount     = imageInfo.arrayLayers;

        vk_textGlyphImageView = vk_device.createImageView(imageViewInfo);

        vk::CommandBuffer commandBuffer = startCopyBuffer();

        transitionImageLayout(commandBuffer, vk_textGlyphImages, imageInfo.arrayLayers);

        TRYR(commandRes, endSingleTimeCommands(commandBuffer));

        SPLITGUI_LOG("Created Glyph Texture Image");

        return Result::eSuccess;
    }
}