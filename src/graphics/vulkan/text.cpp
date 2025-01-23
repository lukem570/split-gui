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

    template <typename T>
    inline T clamp(T n) {
        return n >= T(0) && n <= T(1) ? n : T(n > T(0));
    }

    inline unsigned char pixelFloatToByte(float x) {
        return (unsigned char)(~int(255.5f-255.f*clamp(x)));
    }

    void VulkanInterface::drawText(Vec2 x1, std::string& text) {
        if (!ft_fontInUse) {
            printf("WARN: no font in use\n");
            return;
        }

        int pos = 0;

        for (int i = 0; i < text.size(); i++) {
            if (charImageMappings.find(text[i]) != charImageMappings.end()) {
                continue;
            }

            charImageMappings[text[i]] = {};

            msdfgen::Shape shape;

            // recreation of the 'loadGlyph' function in msdfgen/msdfgen-ext.h to reduce bloat

            int glyphId = ft::FT_Get_Char_Index(ft_face, text[i]);

            ft::FT_Error loadError = ft::FT_Load_Glyph(ft_face, glyphId, (ft::FT_Int32) msdfgen::FontCoordinateScaling::eFontScalingEmNormalized);

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

            msdfgen::SDFTransformation t(msdfgen::Projection(vk_msdfExtent.width, msdfgen::Vector2(0.125, 0.125)), msdfgen::Range(0.125));
            msdfgen::generateMTSDF(msdf, shape, t);

            msdfgen::BitmapConstRef<float, 4> ref = msdf;

            int subPixels = 4 * vk_msdfExtent.width * vk_msdfExtent.height;

            // ------ vulkan code ------

            vk::DeviceSize   stagingBufferSize = subPixels * sizeof(unsigned char);
            vk::Buffer       stagingBuffer;
            vk::DeviceMemory stagingBufferMemory;

            createBuffer(
                stagingBufferSize, 
                vk::BufferUsageFlagBits::eTransferSrc, 
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                stagingBuffer,
                stagingBufferMemory
            );

            unsigned char* memory = (unsigned char*)vk_device.mapMemory(stagingBufferMemory, 0, stagingBufferSize);

            int index = 0;
            for (int x = vk_msdfExtent.width - 1; x >= 0; x--) {
                for (int y = 0; y < vk_msdfExtent.height; y++) {
                    for (int j = 0; j < 4; j++) {
                        memory[index] = pixelFloatToByte(msdf(y,x)[j]);
                        index++;
                    }
                }
            }
            
            vk_device.unmapMemory(stagingBufferMemory);

            vk::CommandBuffer commandBuffer = startCopyBuffer();

            vk::ImageMemoryBarrier barrier1;
            barrier1.srcAccessMask                   = (vk::AccessFlagBits) 0;
            barrier1.dstAccessMask                   = vk::AccessFlagBits::eTransferWrite;
            barrier1.oldLayout                       = vk::ImageLayout::eUndefined;
            barrier1.newLayout                       = vk::ImageLayout::eTransferDstOptimal;
            barrier1.srcQueueFamilyIndex             = vk::QueueFamilyIgnored;
            barrier1.dstQueueFamilyIndex             = vk::QueueFamilyIgnored;
            barrier1.image                           = vk_textGlyphImages;
            barrier1.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
            barrier1.subresourceRange.baseArrayLayer = 0;
            barrier1.subresourceRange.layerCount     = 256;
            barrier1.subresourceRange.levelCount     = 1;

            commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTransfer, (vk::DependencyFlagBits)0, nullptr, nullptr, barrier1);

            vk::BufferImageCopy copyRegion = {};
            copyRegion.bufferOffset                    = 0;
            copyRegion.bufferRowLength                 = 0;
            copyRegion.bufferImageHeight               = 0;
            copyRegion.imageSubresource.aspectMask     = vk::ImageAspectFlagBits::eColor;
            copyRegion.imageSubresource.mipLevel       = 0;
            copyRegion.imageSubresource.baseArrayLayer = text[i];  // layer set to char encoding
            copyRegion.imageSubresource.layerCount     = 1;
            copyRegion.imageOffset.x                   = 0;
            copyRegion.imageOffset.y                   = 0;
            copyRegion.imageOffset.z                   = 0;
            copyRegion.imageExtent.width               = vk_msdfExtent.width;
            copyRegion.imageExtent.height              = vk_msdfExtent.height;
            copyRegion.imageExtent.depth               = 1;

            commandBuffer.copyBufferToImage(stagingBuffer, vk_textGlyphImages, vk::ImageLayout::eTransferDstOptimal, 1, &copyRegion);

            vk::ImageMemoryBarrier barrier2;
            barrier2.srcAccessMask                   = vk::AccessFlagBits::eTransferWrite;
            barrier2.dstAccessMask                   = vk::AccessFlagBits::eShaderRead;
            barrier2.oldLayout                       = vk::ImageLayout::eTransferDstOptimal;
            barrier2.newLayout                       = vk::ImageLayout::eShaderReadOnlyOptimal;
            barrier2.srcQueueFamilyIndex             = vk::QueueFamilyIgnored;
            barrier2.dstQueueFamilyIndex             = vk::QueueFamilyIgnored;
            barrier2.image                           = vk_textGlyphImages;
            barrier2.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
            barrier2.subresourceRange.baseArrayLayer = 0;
            barrier2.subresourceRange.layerCount     = 256;
            barrier2.subresourceRange.levelCount     = 1;

            commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, vk::DependencyFlagBits::eByRegion, nullptr, nullptr, barrier2);

            vk_device.waitIdle();
            endCopyBuffer(commandBuffer);

            vk_device.freeMemory(stagingBufferMemory);
            vk_device.destroyBuffer(stagingBuffer);
        }

        ft::FT_Set_Pixel_Sizes(ft_face, 0, 100);

        for (int i = 0; i < text.size(); i++) {

            ft::FT_Error loadCharError = ft::FT_Load_Char(ft_face, text[i], 0);

            if (loadCharError) {
                printf("WARN: could not load char: %c\n", text[i]);
                return;
            }

            IVec2 windowSize = pWindow->getSize();

            VulkanInterface::drawRect(
                x1 + Vec2{(float)pos / (float)windowSize.x, 0}, 
                x1 + Vec2{(float)(ft_face->glyph->bitmap.width + pos) / (float)windowSize.x, (float)ft_face->glyph->bitmap.rows / (float)windowSize.y}, 
                {0.0, 1.0, 0.0}, 
                VertexFlagsBits::eTextureMsdf, 
                text[i]);

            pos += ft_face->glyph->advance.x / 64;
        }
    }
}