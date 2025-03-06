#include "vulkan.hpp"

namespace SplitGui {
    ResultValue<SceneRef> VulkanInterface::instanceScene(Vec2 x1, Vec2 x2) {

        SceneRef reference;
        reference.sceneNumber = scenes.size();

        scenes.push_back({});
        
        TRYR(pipelineRes, createScenePipeline(scenes.back()));
        TRYR(depthRes, createSceneDepthResources(scenes.back()));
        TRYR(outputRes, createSceneOutputResources(scenes.back()));
        createSceneFramebuffers(scenes.back());
        createSceneDescriptorSet(scenes.back());
        updateSceneDescriptorSet(scenes.back());

        reference.rect = drawRect(x1, x2, HexColor(0xFF00FF).normalize());
        
        SPLITGUI_LOG("Created Scene: %ld", scenes.size());

        return reference;
    }

    inline Result VulkanInterface::createScenesImageArray() {

        vk::ImageCreateInfo imageInfo;
        imageInfo.imageType     = vk::ImageType::e2D;
        imageInfo.format        = vk_surfaceFormat.format;
        imageInfo.extent.width  = vk_swapchainExtent.width;
        imageInfo.extent.height = vk_swapchainExtent.height;
        imageInfo.extent.depth  = 1;
        imageInfo.mipLevels     = 1;
        imageInfo.arrayLayers   = MAX_SCENES;
        imageInfo.samples       = vk::SampleCountFlagBits::e1;
        imageInfo.tiling        = vk::ImageTiling::eOptimal;
        imageInfo.usage         = vk::ImageUsageFlagBits::eTransferDst;
        imageInfo.usage        |= vk::ImageUsageFlagBits::eSampled;
        imageInfo.sharingMode   = vk::SharingMode::eExclusive;
        imageInfo.initialLayout = vk::ImageLayout::eUndefined;

        vk_scenesImageArrayImages = vk_device.createImage(imageInfo);

        vk::MemoryRequirements memoryRequirements = vk_device.getImageMemoryRequirements(vk_scenesImageArrayImages);

        ResultValue<uint32_t> memType = findMemoryType(memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);

        TRYD(memType);

        vk::MemoryAllocateInfo allocateInfo;
        allocateInfo.allocationSize = memoryRequirements.size;
        allocateInfo.memoryTypeIndex = memType.value;

        vk_scenesImageArrayImageMemory = vk_device.allocateMemory(allocateInfo);
        vk_device.bindImageMemory(vk_scenesImageArrayImages, vk_scenesImageArrayImageMemory, 0);

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

        vk_scenesImageArraySampler = vk_device.createSampler(samplerInfo);

        vk::ImageViewCreateInfo imageViewInfo;
        imageViewInfo.image                           = vk_scenesImageArrayImages;
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

        vk_scenesImageArrayImageView = vk_device.createImageView(imageViewInfo);

        vk::CommandBuffer commandBuffer = startCopyBuffer();

        vk::ImageMemoryBarrier barrier;
        barrier.oldLayout                       = vk::ImageLayout::eUndefined;
        barrier.newLayout                       = vk::ImageLayout::eShaderReadOnlyOptimal;
        barrier.srcQueueFamilyIndex             = vk::QueueFamilyIgnored;
        barrier.dstQueueFamilyIndex             = vk::QueueFamilyIgnored;
        barrier.image                           = vk_scenesImageArrayImages;
        barrier.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
        barrier.subresourceRange.baseMipLevel   = 0;
        barrier.subresourceRange.levelCount     = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount     = imageInfo.arrayLayers;
        barrier.srcAccessMask                   = vk::AccessFlagBits(0);
        barrier.dstAccessMask                   = vk::AccessFlagBits::eShaderRead;

        commandBuffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTopOfPipe, 
            vk::PipelineStageFlagBits::eFragmentShader, 
            vk::DependencyFlagBits(0), 
            0, nullptr, 0, nullptr, 1, 
            &barrier
        );

        TRYR(commandRes, endSingleTimeCommands(commandBuffer));

        SPLITGUI_LOG("Created Scenes Image Array");

        return Result::eSuccess;
    }

    void VulkanInterface::submitTriangleData(SceneRef& ref, std::vector<Vertex>& newVertices, std::vector<uint16_t>& newIndices, int flags, int textureNumber) {
        

        SPLITGUI_LOG("Submitted Triangles");
    }

    Result VulkanInterface::updateScene(SceneRef& ref, IVec2 x1, IVec2 x2) {
        

        SPLITGUI_LOG("Updated Scene: %d", ref.sceneNumber);

        return Result::eSuccess;
    }

    Result VulkanInterface::updateSceneCameraView(SceneRef& ref, Mat4& view) {

        return Result::eSuccess;
    }

    Result VulkanInterface::updateSceneCameraProjection(SceneRef& ref, Mat4& projection) {

        return Result::eSuccess;
    }

    Result VulkanInterface::updateSceneCameraPosition(SceneRef& ref, Vec3& position) {

        return Result::eSuccess;
    }
}