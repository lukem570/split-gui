#include "vulkan.hpp"

namespace SplitGui {
    Result VulkanInterface::loadFont(const char* path) {
        SPLITGUI_PROFILE;

        if (ft_fontInUse) {
            ft::FT_Done_Face(ft_face);
        }

        ft::FT_Error error = ft::FT_New_Face(ft_lib, path, 0, &ft_face);
        if (error) {
            return Result::eFailedToLoadFont;
        }
        ft_fontInUse = true;

        Logger::info("Loaded Font");

        return Result::eSuccess;
    }

    void VulkanInterface::deleteText(TextRef& ref) {
        for (unsigned int i = 0; i < ref.rects.size(); i++) {
            deleteRect(ref.rects[i]);
        }
    }

    ResultValue<Vec2> VulkanInterface::getTextSize(const std::string& text, float fontSize) {

        IVec2 windowSize = pWindow->getSize();

        float emScale = fontSize / (float)ft_face->units_per_EM;
        float lineHeight = ft_face->size->metrics.height * emScale;
        
        if (lineHeight == 0) {
            lineHeight = fontSize * 1.2f;
        }

        Vec2 pos = {0.0f, ft_face->ascender * emScale };
        Vec2 size = {0.0f, 0.0f};

        for (unsigned int i = 0; i < text.size(); i++) {

            if (text[i] == '\n') {
                pos.y += lineHeight;
                pos.x = 0;
                continue;
            }
            
            int glyphId = ft::FT_Get_Char_Index(ft_face, text[i]);
            
            ft::FT_Error loadCharError = ft::FT_Load_Glyph(ft_face, glyphId, 0);
            
            if (loadCharError) {
                return Result::eFailedToLoadGlyph;
            }

            ft::FT_GlyphSlot slot = ft_face->glyph;

            if (std::isspace(text[i])) {
                
                pos.x += slot->advance.x * emScale;
                continue;
            }

            float width  = slot->metrics.width * emScale;
            float height = slot->metrics.height * emScale;
            
            float xBorder = ((float)width  * 0.1f * (float)width / (float)height) / 2.0f;
            float yBorder = ((float)height * 0.1f) / 2.0f;
            
            float bearingY = slot->metrics.horiBearingY * emScale;
            
            float outX2 = (pos.x + slot->bitmap_left * emScale + width + xBorder) / windowSize.x * 2.0f;
            float outY2 = (pos.y - bearingY + height + yBorder) / windowSize.y * 2.0f;
            
            if (outX2 > size.x) {
                size.x = outX2;
            }

            if (outY2 > size.y) {
                size.y = outY2;
            }

            pos.x += slot->advance.x * emScale;
        }

        return size + Vec2{-1, -1};
    }

    Result VulkanInterface::prepareTextForRendering(const std::string& text) {
        double fontScale = msdfgen::getFontCoordinateScale(ft_face, msdfgen::FontCoordinateScaling::eFontScalingEmNormalized);

        bool allPrepped = true;

        for (unsigned int i = 0; i < text.size(); i++) {
            if (charSet.find(text[i]) != charSet.end() || std::isspace(text[i])) {
                continue;
            }
            allPrepped = false;
            break;
        }

        if (allPrepped) {
            return Result::eSuccess;
        }

        vk::CommandBuffer commandBuffer = startCopyBuffer();
        std::vector<StagingBuffer> stagingBuffers;

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
            msdfgen::Range pxRange = msdfgen::Range(0.07);
            
            msdfgen::Shape::Bounds bounds = shape.getBounds();

            double width  = (bounds.r - bounds.l);
            double height = (bounds.t - bounds.b);

            bounds.l -= 0.05 * width / height;
            bounds.r += 0.05 * width / height;
            bounds.t += 0.05;
            bounds.b -= 0.05;

            width  = (bounds.r - bounds.l);
            height = (bounds.t - bounds.b);

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

            vk::DeviceSize stagingBufferSize = subPixels * sizeof(unsigned char);
            stagingBuffers.push_back({});

            TRYR(bufferRes, InstanceStagingBuffer(stagingBuffers.back(), stagingBufferSize));

            unsigned char* memory = (unsigned char*)vk_device.mapMemory(stagingBuffers.back().memory, 0, stagingBufferSize);

            int index = 0;
            for (int x = (int)vk_msdfExtent.width - 1; x >= 0; x--) {
                for (int y = 0; y < (int)vk_msdfExtent.height; y++) {
                    for (int j = 0; j < 4; j++) {
                        memory[index] = msdfgen::pixelFloatToByte(msdf(y,x)[j]);
                        index++;
                    }
                }
            }
            
            vk_device.unmapMemory(stagingBuffers.back().memory);

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

            commandBuffer.copyBufferToImage(stagingBuffers.back().buffer, vk_textGlyphImages, vk::ImageLayout::eTransferDstOptimal, 1, &copyRegion);

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
        }

        TRYR(commandRes, endSingleTimeCommands(commandBuffer));
        
        for (unsigned int i = 0; i < stagingBuffers.size(); i++) {

            cleanupStagingBuffer(stagingBuffers[i]);
        }

        return Result::eSuccess;
    }

    Result VulkanInterface::submitText(TextRef& ref) {

        for (auto& rect : ref.rects) {
            TRYR(submitRes, submitRect(rect));
        }

        return Result::eSuccess;
    }

    Result VulkanInterface::updateText(TextRef& ref, const std::string& text, Vec2 x1, Vec3 color, float fontSize, float depth) {
        SPLITGUI_PROFILE;

        IVec2 windowSize = pWindow->getSize();

        TRYR(prepRes, prepareTextForRendering(text));

        if (text.size() < ref.text.size()) {
            for (unsigned int i = 0; i < ref.text.size() - text.size(); i++) {
                deleteRect(ref.rects.back());
                ref.rects.pop_back();
            }
        }

        if (text.size() > ref.text.size()) {

            auto refRes = drawText(x1, text.substr(ref.text.size()), color, fontSize, depth, ref.crop);
            TRYD(refRes);
            ref.rects.insert(ref.rects.end(), refRes.value.rects.begin(), refRes.value.rects.end());
        }

        float emScale = fontSize / (float)ft_face->units_per_EM;
        float lineHeight = ft_face->size->metrics.height * emScale;
        
        if (lineHeight == 0) {
            lineHeight = fontSize * 1.2f;
        }

        Vec2 pos = {0.0f, ft_face->ascender * emScale };

        int rectIdx = 0;

        for (unsigned int i = 0; i < text.size(); i++) {

            if (text[i] == '\n') {
                pos.y += lineHeight;
                pos.x = 0;
                continue;
            }
            
            int glyphId = ft::FT_Get_Char_Index(ft_face, text[i]);
            
            ft::FT_Error loadCharError = ft::FT_Load_Glyph(ft_face, glyphId, 0);
            
            if (loadCharError) {
                return Result::eFailedToLoadGlyph;
            }

            ft::FT_GlyphSlot slot = ft_face->glyph;

            if (std::isspace(text[i])) {
                
                pos.x += slot->advance.x * emScale;
                continue;
            }

            float width  = slot->metrics.width * emScale;
            float height = slot->metrics.height * emScale;
            
            float xBorder = ((float)width  * 0.1f * (float)width / (float)height) / 2.0f;
            float yBorder = ((float)height * 0.1f) / 2.0f;

            float bearingY = slot->metrics.horiBearingY * emScale;
            
            float outX1 = (pos.x + slot->bitmap_left * emScale - xBorder) / windowSize.x * 2.0f;
            float outY1 = (pos.y - bearingY - yBorder) / windowSize.y * 2.0f;
            float outX2 = (pos.x + slot->bitmap_left * emScale + width + xBorder) / windowSize.x * 2.0f;
            float outY2 = (pos.y - bearingY + height + yBorder) / windowSize.y * 2.0f;

            updateRect(
                ref.rects[rectIdx],
                x1 + Vec2{outX1, outY1},
                x1 + Vec2{outX2, outY2},
                color,
                depth,
                text[i]
            );

            pos.x += slot->advance.x * emScale;

            rectIdx++;
        }

        ref.text = text;

        return Result::eSuccess;
    }

    ResultValue<TextRef> VulkanInterface::drawText(Vec2 x1, const std::string& text, Vec3 color, float fontSize, float depth, std::optional<CropRegionRef> crop) {
        SPLITGUI_PROFILE;

        if (!ft_fontInUse) {
            return Result::eFailedToUseFont;
        }

        TRYR(prepRes, prepareTextForRendering(text));

        TextRef ret;
        ret.crop = crop;
        ret.text = text;

        IVec2 windowSize = pWindow->getSize();

        float emScale = fontSize / (float)ft_face->units_per_EM;
        float lineHeight = ft_face->size->metrics.height * emScale;
        
        if (lineHeight == 0) {
            lineHeight = fontSize * 1.2f;
        }

        Vec2 pos = {0.0f, ft_face->ascender * emScale };

        for (unsigned int i = 0; i < text.size(); i++) {

            if (text[i] == '\n') {
                pos.y += lineHeight;
                pos.x = 0;
                continue;
            }
            
            int glyphId = ft::FT_Get_Char_Index(ft_face, text[i]);
            
            ft::FT_Error loadCharError = ft::FT_Load_Glyph(ft_face, glyphId, 0);
            
            if (loadCharError) {
                return Result::eFailedToLoadGlyph;
            }

            ft::FT_GlyphSlot slot = ft_face->glyph;

            if (std::isspace(text[i])) {
                
                pos.x += slot->advance.x * emScale;
                continue;
            }

            float width  = slot->metrics.width * emScale;
            float height = slot->metrics.height * emScale;
            
            float xBorder = ((float)width  * 0.1f * (float)width / (float)height) / 2.0f;
            float yBorder = ((float)height * 0.1f) / 2.0f;

            float bearingY = slot->metrics.horiBearingY * emScale;
            
            float outX1 = (pos.x + slot->bitmap_left * emScale - xBorder) / windowSize.x * 2.0f;
            float outY1 = (pos.y - bearingY - yBorder) / windowSize.y * 2.0f;
            float outX2 = (pos.x + slot->bitmap_left * emScale + width + xBorder) / windowSize.x * 2.0f;
            float outY2 = (pos.y - bearingY + height + yBorder) / windowSize.y * 2.0f;
            
            ret.rects.push_back(drawRect(
                x1 + Vec2{outX1, outY1}, 
                x1 + Vec2{outX2, outY2}, 
                color, 
                depth,
                VertexFlagsBits::eTextureMsdf, 
                text[i],
                crop
            ));

            pos.x += slot->advance.x * emScale;
        }

        Logger::info("Drew Text");

        return ret;
    }
}