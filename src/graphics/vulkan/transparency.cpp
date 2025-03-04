#include "vulkan.hpp"

namespace SplitGui {
    Result VulkanInterface::createTransparentResources() {

        vk::ImageCreateInfo colorAccumImageInfo;
        colorAccumImageInfo.imageType     = vk::ImageType::e2D;
        colorAccumImageInfo.format        = vk::Format::eR16G16B16A16Sfloat;
        colorAccumImageInfo.extent.width  = vk_swapchainExtent.width;
        colorAccumImageInfo.extent.height = vk_swapchainExtent.height;
        colorAccumImageInfo.extent.depth  = 1;
        colorAccumImageInfo.mipLevels     = 1;
        colorAccumImageInfo.arrayLayers   = 1;
        colorAccumImageInfo.samples       = vk::SampleCountFlagBits::e1;
        colorAccumImageInfo.tiling        = vk::ImageTiling::eOptimal;
        colorAccumImageInfo.usage         = vk::ImageUsageFlagBits::eColorAttachment;
        colorAccumImageInfo.usage        |= vk::ImageUsageFlagBits::eSampled;
        colorAccumImageInfo.usage        |= vk::ImageUsageFlagBits::eTransferDst;
        colorAccumImageInfo.sharingMode   = vk::SharingMode::eExclusive;
        colorAccumImageInfo.initialLayout = vk::ImageLayout::eUndefined;

        vk_colorAccumImage = vk_device.createImage(colorAccumImageInfo);

        vk::MemoryRequirements memoryRequirements1 = vk_device.getImageMemoryRequirements(vk_colorAccumImage);

        ResultValue<uint32_t> memType1 = findMemoryType(memoryRequirements1.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);

        TRYD(memType1);

        vk::MemoryAllocateInfo allocInfo1;
        allocInfo1.allocationSize  = memoryRequirements1.size;
        allocInfo1.memoryTypeIndex = memType1.value;

        vk_colorAccumImageMemory = vk_device.allocateMemory(allocInfo1);

        vk_device.bindImageMemory(vk_colorAccumImage, vk_colorAccumImageMemory, 0);

        vk::ImageViewCreateInfo colorAccumImageViewInfo;
        colorAccumImageViewInfo.image                           = vk_colorAccumImage;
        colorAccumImageViewInfo.viewType                        = vk::ImageViewType::e2D;
        colorAccumImageViewInfo.format                          = vk::Format::eR16G16B16A16Sfloat;
        colorAccumImageViewInfo.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
        colorAccumImageViewInfo.subresourceRange.baseMipLevel   = 0;
        colorAccumImageViewInfo.subresourceRange.levelCount     = 1;
        colorAccumImageViewInfo.subresourceRange.baseArrayLayer = 0;
        colorAccumImageViewInfo.subresourceRange.layerCount     = 1;

        vk_colorAccumImageView = vk_device.createImageView(colorAccumImageViewInfo);

        vk::SamplerCreateInfo colorAccumSamplerInfo;
        colorAccumSamplerInfo.magFilter               = vk::Filter::eLinear;
        colorAccumSamplerInfo.minFilter               = vk::Filter::eLinear;
        colorAccumSamplerInfo.mipmapMode              = vk::SamplerMipmapMode::eNearest;
        colorAccumSamplerInfo.addressModeU            = vk::SamplerAddressMode::eClampToBorder;
        colorAccumSamplerInfo.addressModeV            = vk::SamplerAddressMode::eClampToBorder;
        colorAccumSamplerInfo.addressModeW            = vk::SamplerAddressMode::eClampToBorder;
        colorAccumSamplerInfo.mipLodBias              = 0.0f;
        colorAccumSamplerInfo.anisotropyEnable        = vk::False;
        colorAccumSamplerInfo.maxAnisotropy           = 1.0f;
        colorAccumSamplerInfo.compareEnable           = vk::False;
        colorAccumSamplerInfo.compareOp               = vk::CompareOp::eNever;
        colorAccumSamplerInfo.borderColor             = vk::BorderColor::eIntOpaqueBlack;
        colorAccumSamplerInfo.unnormalizedCoordinates = vk::False;
        colorAccumSamplerInfo.minLod                  = 0.0f;
        colorAccumSamplerInfo.maxLod                  = 1.0f;

        vk_colorAccumSampler = vk_device.createSampler(colorAccumSamplerInfo);

        vk::ImageCreateInfo alphaAccumImageInfo;
        alphaAccumImageInfo.imageType     = vk::ImageType::e2D;
        alphaAccumImageInfo.format        = vk::Format::eR16Sfloat;
        alphaAccumImageInfo.extent.width  = vk_swapchainExtent.width;
        alphaAccumImageInfo.extent.height = vk_swapchainExtent.height;
        alphaAccumImageInfo.extent.depth  = 1;
        alphaAccumImageInfo.mipLevels     = 1;
        alphaAccumImageInfo.arrayLayers   = 1;
        alphaAccumImageInfo.samples       = vk::SampleCountFlagBits::e1;
        alphaAccumImageInfo.tiling        = vk::ImageTiling::eOptimal;
        alphaAccumImageInfo.usage         = vk::ImageUsageFlagBits::eColorAttachment;
        alphaAccumImageInfo.usage        |= vk::ImageUsageFlagBits::eSampled;
        alphaAccumImageInfo.usage        |= vk::ImageUsageFlagBits::eTransferDst;
        alphaAccumImageInfo.sharingMode   = vk::SharingMode::eExclusive;
        alphaAccumImageInfo.initialLayout = vk::ImageLayout::eUndefined;

        vk_alphaAccumImage = vk_device.createImage(alphaAccumImageInfo);

        vk::MemoryRequirements memoryRequirements2 = vk_device.getImageMemoryRequirements(vk_alphaAccumImage);

        ResultValue<uint32_t> memType2 = findMemoryType(memoryRequirements2.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);

        TRYD(memType2);

        vk::MemoryAllocateInfo allocInfo2;
        allocInfo2.allocationSize  = memoryRequirements2.size;
        allocInfo2.memoryTypeIndex = memType2.value;

        vk_alphaAccumImageMemory = vk_device.allocateMemory(allocInfo2);

        vk_device.bindImageMemory(vk_alphaAccumImage, vk_alphaAccumImageMemory, 0);

        vk::ImageViewCreateInfo alphaAccumImageViewInfo;
        alphaAccumImageViewInfo.image                           = vk_alphaAccumImage;
        alphaAccumImageViewInfo.viewType                        = vk::ImageViewType::e2D;
        alphaAccumImageViewInfo.format                          = vk::Format::eR16Sfloat;
        alphaAccumImageViewInfo.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
        alphaAccumImageViewInfo.subresourceRange.baseMipLevel   = 0;
        alphaAccumImageViewInfo.subresourceRange.levelCount     = 1;
        alphaAccumImageViewInfo.subresourceRange.baseArrayLayer = 0;
        alphaAccumImageViewInfo.subresourceRange.layerCount     = 1;

        vk_alphaAccumImageView = vk_device.createImageView(alphaAccumImageViewInfo);

        vk::SamplerCreateInfo alphaAccumSamplerInfo;
        alphaAccumSamplerInfo.magFilter               = vk::Filter::eLinear;
        alphaAccumSamplerInfo.minFilter               = vk::Filter::eLinear;
        alphaAccumSamplerInfo.mipmapMode              = vk::SamplerMipmapMode::eNearest;
        alphaAccumSamplerInfo.addressModeU            = vk::SamplerAddressMode::eClampToBorder;
        alphaAccumSamplerInfo.addressModeV            = vk::SamplerAddressMode::eClampToBorder;
        alphaAccumSamplerInfo.addressModeW            = vk::SamplerAddressMode::eClampToBorder;
        alphaAccumSamplerInfo.mipLodBias              = 0.0f;
        alphaAccumSamplerInfo.anisotropyEnable        = vk::False;
        alphaAccumSamplerInfo.maxAnisotropy           = 1.0f;
        alphaAccumSamplerInfo.compareEnable           = vk::False;
        alphaAccumSamplerInfo.compareOp               = vk::CompareOp::eNever;
        alphaAccumSamplerInfo.borderColor             = vk::BorderColor::eIntOpaqueBlack;
        alphaAccumSamplerInfo.unnormalizedCoordinates = vk::False;
        alphaAccumSamplerInfo.minLod                  = 0.0f;
        alphaAccumSamplerInfo.maxLod                  = 1.0f;

        vk_alphaAccumSampler = vk_device.createSampler(alphaAccumSamplerInfo);

        vk::ImageCreateInfo outputImageInfo;
        outputImageInfo.imageType     = vk::ImageType::e2D;
        outputImageInfo.format        = vk_surfaceFormat.format;
        outputImageInfo.extent.width  = vk_swapchainExtent.width;
        outputImageInfo.extent.height = vk_swapchainExtent.height;
        outputImageInfo.extent.depth  = 1;
        outputImageInfo.mipLevels     = 1;
        outputImageInfo.arrayLayers   = 1;
        outputImageInfo.samples       = vk::SampleCountFlagBits::e1;
        outputImageInfo.tiling        = vk::ImageTiling::eOptimal;
        outputImageInfo.usage         = vk::ImageUsageFlagBits::eStorage;
        outputImageInfo.usage        |= vk::ImageUsageFlagBits::eTransferSrc;
        outputImageInfo.usage        |= vk::ImageUsageFlagBits::eTransferDst;
        outputImageInfo.sharingMode   = vk::SharingMode::eExclusive;
        outputImageInfo.initialLayout = vk::ImageLayout::eUndefined;

        vk_outputImage = vk_device.createImage(outputImageInfo);

        vk::MemoryRequirements memoryRequirements3 = vk_device.getImageMemoryRequirements(vk_outputImage);

        ResultValue<uint32_t> memType3 = findMemoryType(memoryRequirements3.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);

        TRYD(memType3);

        vk::MemoryAllocateInfo allocInfo3;
        allocInfo3.allocationSize  = memoryRequirements3.size;
        allocInfo3.memoryTypeIndex = memType3.value;

        vk_outputImageMemory = vk_device.allocateMemory(allocInfo3);

        vk_device.bindImageMemory(vk_outputImage, vk_outputImageMemory, 0);

        vk::ImageViewCreateInfo outputImageViewInfo;
        outputImageViewInfo.image                           = vk_outputImage;
        outputImageViewInfo.viewType                        = vk::ImageViewType::e2D;
        outputImageViewInfo.format                          = vk_surfaceFormat.format;
        outputImageViewInfo.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
        outputImageViewInfo.subresourceRange.baseMipLevel   = 0;
        outputImageViewInfo.subresourceRange.levelCount     = 1;
        outputImageViewInfo.subresourceRange.baseArrayLayer = 0;
        outputImageViewInfo.subresourceRange.layerCount     = 1;

        vk_outputImageView = vk_device.createImageView(outputImageViewInfo);

        vk::CommandBuffer commandBuffer = startCopyBuffer();

        vk::ImageMemoryBarrier colorAccumBarrier;
        colorAccumBarrier.oldLayout                       = vk::ImageLayout::eUndefined;
        colorAccumBarrier.newLayout                       = vk::ImageLayout::eShaderReadOnlyOptimal;
        colorAccumBarrier.srcQueueFamilyIndex             = vk::QueueFamilyIgnored;
        colorAccumBarrier.dstQueueFamilyIndex             = vk::QueueFamilyIgnored;
        colorAccumBarrier.image                           = vk_colorAccumImage;
        colorAccumBarrier.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
        colorAccumBarrier.subresourceRange.baseMipLevel   = 0;
        colorAccumBarrier.subresourceRange.levelCount     = 1;
        colorAccumBarrier.subresourceRange.baseArrayLayer = 0;
        colorAccumBarrier.subresourceRange.layerCount     = 1;

        commandBuffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTopOfPipe,
            vk::PipelineStageFlagBits::eFragmentShader,
            vk::DependencyFlags(0),
            0, nullptr, 0, nullptr, 1, &colorAccumBarrier
        );

        vk::ImageMemoryBarrier weightAccumBarrier;
        weightAccumBarrier.oldLayout                       = vk::ImageLayout::eUndefined;
        weightAccumBarrier.newLayout                       = vk::ImageLayout::eShaderReadOnlyOptimal;
        weightAccumBarrier.srcQueueFamilyIndex             = vk::QueueFamilyIgnored;
        weightAccumBarrier.dstQueueFamilyIndex             = vk::QueueFamilyIgnored;
        weightAccumBarrier.image                           = vk_alphaAccumImage;
        weightAccumBarrier.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
        weightAccumBarrier.subresourceRange.baseMipLevel   = 0;
        weightAccumBarrier.subresourceRange.levelCount     = 1;
        weightAccumBarrier.subresourceRange.baseArrayLayer = 0;
        weightAccumBarrier.subresourceRange.layerCount     = 1;

        commandBuffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTopOfPipe,
            vk::PipelineStageFlagBits::eFragmentShader,
            vk::DependencyFlags(0),
            0, nullptr, 0, nullptr, 1, &weightAccumBarrier
        );

        TRYR(endRes, endSingleTimeCommands(commandBuffer));

        SPLITGUI_LOG("Created Transparent Resources");

        return Result::eSuccess;
    }
}