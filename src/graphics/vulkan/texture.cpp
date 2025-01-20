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
    }
}