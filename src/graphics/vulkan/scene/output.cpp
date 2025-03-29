#include "../vulkan.hpp"

namespace SplitGui {
    inline Result VulkanInterface::createSceneOutputResources(SceneObject& scene) {
        SPLITGUI_PROFILE;

        vk::ImageCreateInfo outputImageInfo;
        outputImageInfo.imageType     = vk::ImageType::e2D;
        outputImageInfo.format        = vk_surfaceFormat.format;
        outputImageInfo.extent.width  = scene.sceneSize.x;
        outputImageInfo.extent.height = scene.sceneSize.y;
        outputImageInfo.extent.depth  = 1;
        outputImageInfo.mipLevels     = 1;
        outputImageInfo.arrayLayers   = 1;
        outputImageInfo.samples       = vk::SampleCountFlagBits::e1;
        outputImageInfo.tiling        = vk::ImageTiling::eOptimal;
        outputImageInfo.usage         = vk::ImageUsageFlagBits::eColorAttachment;
        outputImageInfo.usage        |= vk::ImageUsageFlagBits::eTransferSrc;
        outputImageInfo.sharingMode   = vk::SharingMode::eExclusive;
        outputImageInfo.initialLayout = vk::ImageLayout::eUndefined;

        vk::Image outputImage = vk_device.createImage(outputImageInfo);

        vk::MemoryRequirements memoryRequirements = vk_device.getImageMemoryRequirements(outputImage);

        ResultValue<uint32_t> memType = findMemoryType(memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);

        TRYD(memType);

        vk::MemoryAllocateInfo allocInfo;
        allocInfo.allocationSize  = memoryRequirements.size;
        allocInfo.memoryTypeIndex = memType.value;

        vk::DeviceMemory outputImageMemory = vk_device.allocateMemory(allocInfo);

        vk_device.bindImageMemory(outputImage, outputImageMemory, 0);

        vk::ImageViewCreateInfo outputImageViewInfo;
        outputImageViewInfo.image                           = outputImage;
        outputImageViewInfo.viewType                        = vk::ImageViewType::e2D;
        outputImageViewInfo.format                          = vk_surfaceFormat.format;
        outputImageViewInfo.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
        outputImageViewInfo.subresourceRange.baseMipLevel   = 0;
        outputImageViewInfo.subresourceRange.levelCount     = 1;
        outputImageViewInfo.subresourceRange.baseArrayLayer = 0;
        outputImageViewInfo.subresourceRange.layerCount     = 1;

        vk::ImageView outputImageView = vk_device.createImageView(outputImageViewInfo);

        scene.outputImage       = outputImage;
        scene.outputImageMemory = outputImageMemory;
        scene.outputImageView   = outputImageView;

        Logger::info("Created Scene Output Resources");

        return Result::eSuccess;
    }
}