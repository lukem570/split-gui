#include "vulkan.hpp"

namespace SplitGui {
    inline void VulkanInterface::createTextGlyphImage() {
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

        vk::MemoryAllocateInfo allocateInfo;
        allocateInfo.allocationSize = memoryRequirements.size;
        allocateInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);

        vk_textGlyphImageMemory = vk_device.allocateMemory(allocateInfo);
        vk_device.bindImageMemory(vk_textGlyphImages, vk_textGlyphImageMemory, 0);

        vk::SamplerCreateInfo samplerInfo;
        samplerInfo.magFilter               = vk::Filter::eLinear;
        samplerInfo.minFilter               = vk::Filter::eLinear;
        samplerInfo.mipmapMode              = vk::SamplerMipmapMode::eNearest;
        samplerInfo.addressModeU            = vk::SamplerAddressMode::eRepeat;
        samplerInfo.addressModeV            = vk::SamplerAddressMode::eRepeat;
        samplerInfo.addressModeW            = vk::SamplerAddressMode::eRepeat;
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

        vk::DescriptorImageInfo descriptorImageInfo;
        descriptorImageInfo.sampler     = vk_textGlyphSampler;
        descriptorImageInfo.imageView   = vk_textGlyphImageView;
        descriptorImageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

        vk::WriteDescriptorSet writeDescriptor;
        writeDescriptor.dstSet          = vk_descriptorSet;
        writeDescriptor.dstBinding      = DescriporBindings::eGlyphs;
        writeDescriptor.dstArrayElement = 0;
        writeDescriptor.descriptorType  = vk::DescriptorType::eCombinedImageSampler;
        writeDescriptor.descriptorCount = 1;
        writeDescriptor.pImageInfo      = &descriptorImageInfo;

        vk_device.updateDescriptorSets(1, &writeDescriptor, 0, nullptr);
    }
}