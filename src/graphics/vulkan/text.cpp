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

    Result VulkanInterface::updateText(TextRef& ref, Vec2 x1, Vec3 color, int fontSize, float depth) {
        IVec2 windowSize = pWindow->getSize();

        Vec2 pos = {0.0f, 0.0f};
        ft::FT_Set_Char_Size(ft_face, 0, fontSize * 64, 1000, 500);

        pos.y += ft_face->ascender >> 3;

        int rectIdx = 0;

        for (unsigned int i = 0; i < ref.text.size(); i++) {

            if (ref.text[i] == '\n') {
                pos.y += ft_face->ascender >> 3;
                pos.x = 0;
                continue;
            }

            if (std::isspace(ref.text[i])) {
                pos.x += fontSize * 2;
                continue;
            }

            int glyphId = ft::FT_Get_Char_Index(ft_face, ref.text[i]);

            ft::FT_Error loadCharError = ft::FT_Load_Glyph(ft_face, glyphId, 0);

            if (loadCharError) {
                return Result::eFailedToLoadGlyph;
            }

            ft::FT_GlyphSlot slot    = ft_face->glyph;
            unsigned int     width   = slot->bitmap.width;
            unsigned int     height  = slot->bitmap.rows;
            unsigned int     maxHeight = ft_face->ascender >> 6;
            unsigned int     offsetX = slot->bitmap_left;
            unsigned int     offsetY = slot->bitmap_top;

            float outX1 = (pos.x + offsetX) / windowSize.x / 2.0f;
            
            float outY1 = (pos.y + maxHeight - offsetY - height) / windowSize.y / 2.0f;
            float outX2 = (pos.x + offsetX + width) / windowSize.x / 2.0f;
            float outY2 = (pos.y + maxHeight) / windowSize.y / 2.0f;

            updateRect(
                ref.rects[rectIdx],
                x1 + Vec2{outX1, outY1},
                x1 + Vec2{outX2, outY2},
                color,
                depth
            );

            pos.x += slot->advance.x >> 6;

            rectIdx++;
        }

        return Result::eSuccess;
    }

    ResultValue<TextRef> VulkanInterface::drawText(Vec2 x1, std::string& text, Vec3 color, int fontSize, float depth) {
        if (!ft_fontInUse) {
            return Result::eFailedToUseFont;
        }

        TextRef ret;
        ret.text = text;

        IVec2 windowSize = pWindow->getSize();

        Vec2 pos = {0.0f, 0.0f};
        ft::FT_Set_Char_Size(ft_face, 0, fontSize * 64, 1000, 500);

        double fontScale = msdfgen::getFontCoordinateScale(ft_face, msdfgen::FontCoordinateScaling::eFontScalingEmNormalized);

        for (unsigned int i = 0; i < text.size(); i++) {
            if (charSet.find(text[i]) != charSet.end() || std::isspace(text[i])) {
                continue;
            }

            msdfgen::Shape shape;

            // recreation of the 'loadGlyph' function in msdfgen/msdfgen-ext.h

            int glyphId = ft::FT_Get_Char_Index(ft_face, text[i]);

            ft::FT_Error loadError = ft::FT_Load_Glyph(ft_face, glyphId, (ft::FT_Int32) msdfgen::FontCoordinateScaling::eFontScalingEmNormalized);

            if (loadError) {
                return Result::eFailedToLoadGlyph;
            }

            ft::FT_Error outlineError = msdfgen::readFreetypeOutline(shape, &ft_face->glyph->outline, fontScale);

            if (outlineError) {
                return Result::eFailedToLoadGlyph;
            }

            shape.normalize();

            msdfgen::edgeColoringSimple(shape, 3.0);
            
            msdfgen::Vector2 scale;
            msdfgen::Range pxRange = msdfgen::Range(0.04);
            
            msdfgen::Shape::Bounds bounds = shape.getBounds();

            double width  = (bounds.r - bounds.l);
            double height = (bounds.t - bounds.b);

            charSet.insert(text[i]);
            
            scale.set((double)vk_msdfExtent.width / width, (double)vk_msdfExtent.height / height);

            msdfgen::SDFTransformation transformation(
                msdfgen::Projection(
                    scale,
                    msdfgen::Vector2(-bounds.l, -bounds.b)
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

        pos.y += ft_face->ascender >> 3;

        for (unsigned int i = 0; i < text.size(); i++) {

            if (text[i] == '\n') {
                pos.y += ft_face->ascender >> 3;
                pos.x = 0;
                continue;
            }

            if (std::isspace(text[i])) {
                pos.x += fontSize * 2;
                continue;
            }

            int glyphId = ft::FT_Get_Char_Index(ft_face, text[i]);

            ft::FT_Error loadCharError = ft::FT_Load_Glyph(ft_face, glyphId, 0);

            if (loadCharError) {
                return Result::eFailedToLoadGlyph;
            }

            ft::FT_GlyphSlot slot     = ft_face->glyph;
            unsigned int     width    = slot->bitmap.width;
            unsigned int     height   = slot->bitmap.rows;
            unsigned int     maxHeight = ft_face->ascender >> 6;
            unsigned int     offsetX  = slot->bitmap_left;
            unsigned int     offsetY  = slot->bitmap_top;
            int              bearingY = slot->metrics.horiBearingY;

            float yOff = (float)height - (float)bearingY * (1.0f/64.0f);

            float outX1 = (pos.x + offsetX) / windowSize.x / 2.0f;
            
            float outY1 = (pos.y + maxHeight - offsetY - height + yOff) / windowSize.y / 2.0f;
            float outX2 = (pos.x + offsetX + width) / windowSize.x / 2.0f;
            float outY2 = (pos.y + maxHeight + yOff) / windowSize.y / 2.0f;

            ret.rects.push_back(drawRect(
                x1 + Vec2{outX1, outY1}, 
                x1 + Vec2{outX2, outY2}, 
                color, 
                depth,
                VertexFlagsBits::eTextureMsdf, 
                text[i]
            ));

            pos.x += slot->advance.x >> 6;
        }

        SPLITGUI_LOG("Drew Text");

        return ret;
    }
}