#include "vulkan.hpp"

namespace SplitGui {
    void VulkanInterface::loadFont(const char* path) {
        if (ft_fontInUse) {
            ft::FT_Done_Face(ft_face);
        }

        ft::FT_Error error = ft::FT_New_Face(ft_lib, path, 0, &ft_face);
        if (error) {
            printf("WARN: could not load font: %s\n", path);
            return;
        }
        ft_fontInUse = true;
    }

    void VulkanInterface::drawText(Vec2 x1, Vec2 x2, std::string& text) {
        if (!ft_fontInUse) {
            printf("WARN: no font in use\n");
            return;
        }

        vk::CommandBuffer commandBuffer = startCopyBuffer();

        for (int i = 0; i < text.size(); i++) {
            if (charImageMappings.find(text[i]) != charImageMappings.end()) {
                continue;
            }

            charImageMappings[text[i]] = {};

            msdfgen::Shape shape;

            // recreation of the 'loadGlyph' function in msdfgen/msdfgen-ext.h to reduce bloat

            ft::FT_Error loadError = ft::FT_Load_Glyph(ft_face, text[i], (ft::FT_Int32) msdfgen::FontCoordinateScaling::eFontScalingEmNormalized);

            if (loadError) {
                printf("WARN: could not load glyph: %c\n", text[i]);
                return;
            }

            double scale = msdfgen::getFontCoordinateScale(ft_face, msdfgen::FontCoordinateScaling::eFontScalingEmNormalized);

            ft::FT_Error outlineError = msdfgen::readFreetypeOutline(shape, &ft_face->glyph->outline, scale);

            if (outlineError) {
                printf("WARN: could not load glyph: %c\n", text[i]);
                return;
            }

            shape.normalize();

            msdfgen::edgeColoringSimple(shape, 3.0);

            msdfgen::Bitmap<float, 4> msdf(vk_msdfExtent.width, vk_msdfExtent.height);

            msdfgen::SDFTransformation t(msdfgen::Projection(32.0, msdfgen::Vector2(0.125, 0.125)), msdfgen::Range(0.125));
            msdfgen::generateMTSDF(msdf, shape, t);

            msdfgen::BitmapConstRef<float, 4> ref = msdf;

            vk::ImageCreateInfo imageInfo;
            imageInfo.imageType     = vk::ImageType::e2D;
            imageInfo.format        = vk::Format::eR8G8B8A8Unorm;
            imageInfo.extent.width  = vk_msdfExtent.width;
            imageInfo.extent.height = vk_msdfExtent.height;
            imageInfo.extent.depth  = 1;
            imageInfo.mipLevels     = 1;
            imageInfo.arrayLayers   = 1;
            imageInfo.samples       = vk::SampleCountFlagBits::e1;
            imageInfo.tiling        = vk::ImageTiling::eOptimal;
            imageInfo.usage         = vk::ImageUsageFlagBits::eTransferDst;
            imageInfo.usage        |= vk::ImageUsageFlagBits::eSampled;
            imageInfo.sharingMode   = vk::SharingMode::eExclusive;
            imageInfo.initialLayout = vk::ImageLayout::eUndefined;

            charImageMappings[text[i]].image = vk_device.createImage(imageInfo);

            vk::MemoryRequirements memReqs = vk_device.getImageMemoryRequirements(charImageMappings[text[i]].image);
            
            vk::MemoryAllocateInfo allocInfo;
            allocInfo.allocationSize  = memReqs.size;
            allocInfo.memoryTypeIndex = findMemoryType(memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);

            charImageMappings[text[i]].imageMemory = vk_device.allocateMemory(allocInfo);

            vk_device.bindImageMemory(charImageMappings[text[i]].image, charImageMappings[text[i]].imageMemory, 0);

            vk::ImageViewCreateInfo imageViewInfo;
            imageViewInfo.image                           = charImageMappings[text[i]].image;
            imageViewInfo.viewType                        = vk::ImageViewType::e2D;
            imageViewInfo.format                          = imageInfo.format;
            imageViewInfo.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
            imageViewInfo.subresourceRange.baseMipLevel   = 0;
            imageViewInfo.subresourceRange.levelCount     = 1;
            imageViewInfo.subresourceRange.baseArrayLayer = 0;
            imageViewInfo.subresourceRange.layerCount     = 1;

            charImageMappings[text[i]].imageView = vk_device.createImageView(imageViewInfo);

            vk::DeviceSize   imageSize = vk_msdfExtent.width * vk_msdfExtent.height * sizeof(float) * 4;
            vk::Buffer       imageStagingBuffer;
            vk::DeviceMemory imageStagingBufferMemory;
            
            // could put this as one big buffer but I am too tired to do so
            createBuffer(
                imageSize, 
                vk::BufferUsageFlagBits::eTransferSrc, 
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                imageStagingBuffer,
                imageStagingBufferMemory
            );

            void* memory = vk_device.mapMemory(imageStagingBufferMemory, 0, imageSize);
            memcpy(memory, ref.pixels, imageSize);
            vk_device.unmapMemory(imageStagingBufferMemory);

            vk::ImageMemoryBarrier barrier;
            barrier.srcAccessMask                   = {};
            barrier.dstAccessMask                   = vk::AccessFlagBits::eTransferWrite;
            barrier.oldLayout                       = vk::ImageLayout::eUndefined;
            barrier.newLayout                       = vk::ImageLayout::eTransferDstOptimal;
            barrier.srcQueueFamilyIndex             = vk::QueueFamilyIgnored;
            barrier.dstQueueFamilyIndex             = vk::QueueFamilyIgnored;
            barrier.image                           = charImageMappings[text[i]].image;
            barrier.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
            barrier.subresourceRange.baseMipLevel   = 0;
            barrier.subresourceRange.levelCount     = 1;
            barrier.subresourceRange.baseArrayLayer = 0;
            barrier.subresourceRange.layerCount     = 1;

            commandBuffer.pipelineBarrier(
                vk::PipelineStageFlagBits::eTopOfPipe,
                vk::PipelineStageFlagBits::eTransfer,
                {},
                nullptr, nullptr,
                barrier
            );

            vk::BufferImageCopy region;
            region.bufferOffset                    = 0;
            region.bufferRowLength                 = 0;
            region.bufferImageHeight               = 0;
            region.imageSubresource.aspectMask     = vk::ImageAspectFlagBits::eColor;
            region.imageSubresource.mipLevel       = 0;
            region.imageSubresource.baseArrayLayer = 0;
            region.imageSubresource.layerCount     = 1;
            region.imageOffset.x                   = 0;
            region.imageOffset.y                   = 0;
            region.imageOffset.z                   = 0;
            region.imageExtent.width               = vk_msdfExtent.width;
            region.imageExtent.height              = vk_msdfExtent.height;
            region.imageExtent.depth               = 1;

            commandBuffer.copyBufferToImage(imageStagingBuffer, charImageMappings[text[i]].image, vk::ImageLayout::eTransferDstOptimal, 1, &region);

            vk::ImageMemoryBarrier barrier2;
            barrier2.srcAccessMask                   = vk::AccessFlagBits::eTransferWrite;
            barrier2.dstAccessMask                   = vk::AccessFlagBits::eShaderRead;
            barrier2.oldLayout                       = vk::ImageLayout::eTransferDstOptimal;
            barrier2.newLayout                       = vk::ImageLayout::eShaderReadOnlyOptimal;
            barrier2.srcQueueFamilyIndex             = vk::QueueFamilyIgnored;
            barrier2.dstQueueFamilyIndex             = vk::QueueFamilyIgnored;
            barrier2.image                           = charImageMappings[text[i]].image;
            barrier2.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
            barrier2.subresourceRange.baseMipLevel   = 0;
            barrier2.subresourceRange.levelCount     = 1;
            barrier2.subresourceRange.baseArrayLayer = 0;
            barrier2.subresourceRange.layerCount     = 1;

            // Insert the barrier to transition the image layout to the final layout for sampling
            commandBuffer.pipelineBarrier(
                vk::PipelineStageFlagBits::eTransfer,
                vk::PipelineStageFlagBits::eFragmentShader,
                {},
                nullptr, nullptr, // No synchronization needed between stages here
                barrier2
            );


            vk::SamplerCreateInfo samplerInfo;
            samplerInfo.magFilter               = vk::Filter::eLinear;
            samplerInfo.minFilter               = vk::Filter::eLinear;;
            samplerInfo.addressModeU            = vk::SamplerAddressMode::eRepeat;
            samplerInfo.addressModeV            = vk::SamplerAddressMode::eRepeat;
            samplerInfo.addressModeW            = vk::SamplerAddressMode::eRepeat;
            samplerInfo.borderColor             = vk::BorderColor::eIntTransparentBlack;
            samplerInfo.unnormalizedCoordinates = vk::False;
            samplerInfo.compareEnable           = vk::False;
            samplerInfo.compareOp               = vk::CompareOp::eAlways;
            samplerInfo.mipmapMode              = vk::SamplerMipmapMode::eLinear;

            charImageMappings[text[i]].sampler = vk_device.createSampler(samplerInfo);
        }

        endCopyBuffer(commandBuffer);
    }
}