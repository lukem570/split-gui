#include "vulkan.hpp"

namespace SplitGui {
    Result VulkanInterface::loadFont(const char* path) {
        if (ft_fontInUse) {
            ft::FT_Done_Face(ft_face);
        }

        ft::FT_Error error = ft::FT_New_Face(ft_lib, path, 0, &ft_face);
        if (error) {
            return Result::eFailedToLoadFont;
        }
        ft_fontInUse = true;

        SPLITGUI_LOG("Loaded Font");

        return Result::eSuccess;
    }

    ResultValue<float> VulkanInterface::drawText(Vec2 x1, std::string& text, Vec3 color, int fontSize, float depth) {
        if (!ft_fontInUse) {
            return Result::eFailedToUseFont;
        }

        IVec2 windowSize = pWindow->getSize();

        float pos = 0;
        ft::FT_Set_Char_Size(ft_face, 0, fontSize * 64, windowSize.x, windowSize.y);

        for (unsigned int i = 0; i < text.size(); i++) {
            if (charWidthMap.find(text[i]) != charWidthMap.end() || std::isspace(text[i])) {
                continue;
            }

            msdfgen::Shape shape;

            // recreation of the 'loadGlyph' function in msdfgen/msdfgen-ext.h to reduce bloat

            int glyphId = ft::FT_Get_Char_Index(ft_face, text[i]);

            ft::FT_Error loadError = ft::FT_Load_Glyph(ft_face, glyphId, (ft::FT_Int32) msdfgen::FontCoordinateScaling::eFontScalingEmNormalized);

            if (loadError) {
                return Result::eFailedToLoadGlyph;
            }

            double fontScale = msdfgen::getFontCoordinateScale(ft_face, msdfgen::FontCoordinateScaling::eFontScalingEmNormalized);

            ft::FT_Error outlineError = msdfgen::readFreetypeOutline(shape, &ft_face->glyph->outline, fontScale);

            if (outlineError) {
                return Result::eFailedToLoadGlyph;
            }

            shape.normalize();

            msdfgen::edgeColoringSimple(shape, 3.0);
            
            msdfgen::Vector2 scale;
            msdfgen::Range pxRange = msdfgen::Range(0.1);
            
            msdfgen::Shape::Bounds bounds = shape.getBounds();

            bounds.l -= 0.1;
            bounds.r += 0.1;

            double width  = (bounds.r - bounds.l);
            double height = (bounds.t - bounds.b);

            charWidthMap[text[i]] = width / height;
            
            scale.set((double)vk_msdfExtent.width / width, (double)vk_msdfExtent.height);

            msdfgen::SDFTransformation transformation(
                msdfgen::Projection(
                    scale,
                    msdfgen::Vector2(-bounds.l, 0.2)
                ), 
                pxRange
            );

            msdfgen::Bitmap<float, 4> msdf(vk_msdfExtent.width, vk_msdfExtent.height);
            msdfgen::generateMTSDF(msdf, shape, transformation);
            
            int subPixels = 4 * vk_msdfExtent.width * vk_msdfExtent.height;

            // ------ vulkan code ------

            vk::DeviceSize   stagingBufferSize = subPixels * sizeof(unsigned char);
            vk::Buffer       stagingBuffer;
            vk::DeviceMemory stagingBufferMemory;

            TRYR(bufferRes, createBuffer(
                stagingBufferSize, 
                vk::BufferUsageFlagBits::eTransferSrc, 
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                stagingBuffer,
                stagingBufferMemory
            ));

            unsigned char* memory = (unsigned char*)vk_device.mapMemory(stagingBufferMemory, 0, stagingBufferSize);

            int index = 0;
            for (int x = (int)vk_msdfExtent.width - 1; x >= 0; x--) {
                for (int y = 0; y < (int)vk_msdfExtent.height; y++) {
                    for (int j = 0; j < 4; j++) {
                        memory[index] = msdfgen::pixelFloatToByte(msdf(y,x)[j]);
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

            TRYR(commandRes, endSingleTimeCommands(commandBuffer));

            vk_device.freeMemory(stagingBufferMemory);
            vk_device.destroyBuffer(stagingBuffer);
        }

        for (unsigned int i = 0; i < text.size(); i++) {
            if (std::isspace(text[i])) {
                pos += (float)fontSize / (float)windowSize.x;
                continue;
            }

            int glyphId = ft::FT_Get_Char_Index(ft_face, text[i]);

            ft::FT_Error loadCharError = ft::FT_Load_Glyph(ft_face, glyphId, 0);

            if (loadCharError) {
                return Result::eFailedToLoadGlyph;
            }

            ft::FT_GlyphSlot slot = ft_face->glyph;
            ft::FT_Fixed heightInFUnits = ft_face->size->metrics.height;
            ft::FT_Fixed widthInFUnits = slot->metrics.horiAdvance;
            ft::FT_F26Dot6 heightInPixels = (heightInFUnits * 16) / ft_face->units_per_EM;
            ft::FT_F26Dot6 widthInPixels = (widthInFUnits * 16) / ft_face->units_per_EM;

            float height = (float)heightInPixels / (float)windowSize.y;
            float width  = ((float)widthInPixels ) / (float)windowSize.x;

            VulkanInterface::drawRect(
                x1 + Vec2{pos , 0}, 
                x1 + Vec2{pos + width, height}, 
                color, 
                depth,
                VertexFlagsBits::eTextureMsdf, 
                text[i]
            );

            pos += width;
        }

        SPLITGUI_LOG("Drew Text");

        return pos;
    }
}