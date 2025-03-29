#include "vulkan.hpp"

namespace SplitGui {

    inline msdfgen::Point2 splitguiPoint2(const Vec2& vector) {
        return msdfgen::Point2(vector.x, vector.y);
    }

    ResultValue<unsigned int> VulkanInterface::createContourImage(std::vector<Contour>& contours) {
        SPLITGUI_PROFILE;

        msdfgen::Shape shape;

        msdfgen::Contour* contour;

        msdfgen::Point2 position;

        for (unsigned int i = 0; i < contours.size(); i++) {

            if (std::holds_alternative<MoveTo>(contours[i])) {
                
                msdfgen::Point2 from = splitguiPoint2(std::get<MoveTo>(contours[i]).from);
                
                contour = &shape.addContour();
                position = from;
            }

            if (std::holds_alternative<LinearContour>(contours[i])) {

                msdfgen::Point2 to = splitguiPoint2(std::get<LinearContour>(contours[i]).to);
                
                contour->addEdge(msdfgen::EdgeHolder(position, to));
                position = to;
            }
            
            if (std::holds_alternative<QuadraticBezierContour>(contours[i])) {
                
                msdfgen::Point2 to      = splitguiPoint2(std::get<QuadraticBezierContour>(contours[i]).to);
                msdfgen::Point2 control = splitguiPoint2(std::get<QuadraticBezierContour>(contours[i]).control);
                
                msdfgen::EdgeHolder quadraticHolder(position, control, to);
                
                contour->addEdge(quadraticHolder);
                position = to;
            }
            
            if (std::holds_alternative<CubicBezierContour>(contours[i])) {
                
                msdfgen::Point2 to       = splitguiPoint2(std::get<CubicBezierContour>(contours[i]).to);
                msdfgen::Point2 controlA = splitguiPoint2(std::get<CubicBezierContour>(contours[i]).controlA);
                msdfgen::Point2 controlB = splitguiPoint2(std::get<CubicBezierContour>(contours[i]).controlB);
                
                msdfgen::EdgeHolder cubicHolder(position, controlA, controlB, to);
                
                contour->addEdge(cubicHolder);
                position = to;
            }
        }

        
        shape.normalize();
        
        msdfgen::edgeColoringSimple(shape, M_PI);
        
        msdfgen::Range pxRange = msdfgen::Range(0.04);
        
        msdfgen::Vector2 scale;

        scale.set(vk_msdfExtent.width, vk_msdfExtent.height);

        msdfgen::SDFTransformation transformation(
            msdfgen::Projection(
                scale,
                msdfgen::Vector2(0, 0)
            ), 
            pxRange
        );

        msdfgen::Bitmap<float, 4> msdf(vk_msdfExtent.width, vk_msdfExtent.height);
        msdfgen::generateMTSDF(msdf, shape, transformation);

        int numPixels = 4 * vk_msdfExtent.width * vk_msdfExtent.height;

        vk::DeviceSize   stagingBufferSize = numPixels * sizeof(unsigned char);
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
        barrier1.image                           = vk_textureArrayImages;
        barrier1.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
        barrier1.subresourceRange.baseArrayLayer = 0;
        barrier1.subresourceRange.layerCount     = MAX_VECTOR_IMAGES;
        barrier1.subresourceRange.levelCount     = 1;

        commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTransfer, (vk::DependencyFlagBits)0, nullptr, nullptr, barrier1);

        vk::BufferImageCopy copyRegion = {};
        copyRegion.bufferOffset                    = 0;
        copyRegion.bufferRowLength                 = 0;
        copyRegion.bufferImageHeight               = 0;
        copyRegion.imageSubresource.aspectMask     = vk::ImageAspectFlagBits::eColor;
        copyRegion.imageSubresource.mipLevel       = 0;
        copyRegion.imageSubresource.baseArrayLayer = vectorImages.size(); // layer set to index
        copyRegion.imageSubresource.layerCount     = 1;
        copyRegion.imageOffset.x                   = 0;
        copyRegion.imageOffset.y                   = 0;
        copyRegion.imageOffset.z                   = 0;
        copyRegion.imageExtent.width               = vk_msdfExtent.width;
        copyRegion.imageExtent.height              = vk_msdfExtent.height;
        copyRegion.imageExtent.depth               = 1;

        commandBuffer.copyBufferToImage(stagingBuffer, vk_textureArrayImages, vk::ImageLayout::eTransferDstOptimal, 1, &copyRegion);

        vk::ImageMemoryBarrier barrier2;
        barrier2.srcAccessMask                   = vk::AccessFlagBits::eTransferWrite;
        barrier2.dstAccessMask                   = vk::AccessFlagBits::eShaderRead;
        barrier2.oldLayout                       = vk::ImageLayout::eTransferDstOptimal;
        barrier2.newLayout                       = vk::ImageLayout::eShaderReadOnlyOptimal;
        barrier2.srcQueueFamilyIndex             = vk::QueueFamilyIgnored;
        barrier2.dstQueueFamilyIndex             = vk::QueueFamilyIgnored;
        barrier2.image                           = vk_textureArrayImages;
        barrier2.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
        barrier2.subresourceRange.baseArrayLayer = 0;
        barrier2.subresourceRange.layerCount     = MAX_VECTOR_IMAGES;
        barrier2.subresourceRange.levelCount     = 1;

        commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, vk::DependencyFlagBits::eByRegion, nullptr, nullptr, barrier2);

        vk_device.waitIdle();

        TRYR(commandRes, endSingleTimeCommands(commandBuffer));

        vectorImages.push_back(contours);

        vk_device.freeMemory(stagingBufferMemory);
        vk_device.destroyBuffer(stagingBuffer);

        Logger::info("Created Contour Image");

        return vectorImages.size() - 1;
    }
}