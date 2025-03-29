#include "vulkan.hpp"

namespace SplitGui {

    inline ResultValue<vk::Format> VulkanInterface::findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features) {
        SPLITGUI_PROFILE;
        
        for (unsigned int i = 0; i < candidates.size(); i++) {

            vk::FormatProperties props = vk_physicalDevice.getFormatProperties(candidates[i]);

            if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features) {
                return candidates[i];
            } else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features) {
                return candidates[i];
            }
        }

        return Result::eFailedToFindSuitableFormat;
    }

    inline Result VulkanInterface::createDepthResources() {
        SPLITGUI_PROFILE;

        ResultValue<vk::Format> depthFormat = findSupportedFormat(
            {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
            vk::ImageTiling::eOptimal,
            vk::FormatFeatureFlagBits::eDepthStencilAttachment
        );

        TRYD(depthFormat);

        vk::ImageCreateInfo depthImageInfo;
        depthImageInfo.imageType     = vk::ImageType::e2D;
        depthImageInfo.format        = depthFormat.value;
        depthImageInfo.extent.width  = vk_swapchainExtent.width;
        depthImageInfo.extent.height = vk_swapchainExtent.height;
        depthImageInfo.extent.depth  = 1;
        depthImageInfo.mipLevels     = 1;
        depthImageInfo.arrayLayers   = 1;
        depthImageInfo.samples       = vk::SampleCountFlagBits::e1;
        depthImageInfo.tiling        = vk::ImageTiling::eOptimal;
        depthImageInfo.usage         = vk::ImageUsageFlagBits::eDepthStencilAttachment;
        depthImageInfo.sharingMode   = vk::SharingMode::eExclusive;
        depthImageInfo.initialLayout = vk::ImageLayout::eUndefined;

        vk_depthImage = vk_device.createImage(depthImageInfo);

        vk::MemoryRequirements memoryRequirements = vk_device.getImageMemoryRequirements(vk_depthImage);

        ResultValue<uint32_t> memType = findMemoryType(memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);

        TRYD(memType);

        vk::MemoryAllocateInfo allocInfo;
        allocInfo.allocationSize  = memoryRequirements.size;
        allocInfo.memoryTypeIndex = memType.value;

        vk_depthImageMemory = vk_device.allocateMemory(allocInfo);

        vk_device.bindImageMemory(vk_depthImage, vk_depthImageMemory, 0);

        vk::ImageViewCreateInfo depthImageViewInfo;
        depthImageViewInfo.image                           = vk_depthImage;
        depthImageViewInfo.viewType                        = vk::ImageViewType::e2D;
        depthImageViewInfo.format                          = depthFormat.value;
        depthImageViewInfo.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eDepth;
        depthImageViewInfo.subresourceRange.baseMipLevel   = 0;
        depthImageViewInfo.subresourceRange.levelCount     = 1;
        depthImageViewInfo.subresourceRange.baseArrayLayer = 0;
        depthImageViewInfo.subresourceRange.layerCount     = 1;

        vk_depthImageView = vk_device.createImageView(depthImageViewInfo);

        Logger::info("Created Depth Resources");

        return Result::eSuccess;
    }
}