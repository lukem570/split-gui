#include "vulkan.hpp"

namespace SplitGui {

    void VulkanInterface::transitionImageToComputeWriteState(vk::CommandBuffer commandBuffer, vk::Image image) {
        vk::ImageMemoryBarrier barrier;
        barrier.oldLayout                       = vk::ImageLayout::eUndefined;
        barrier.newLayout                       = vk::ImageLayout::eGeneral;
        barrier.srcQueueFamilyIndex             = vk::QueueFamilyIgnored;
        barrier.dstQueueFamilyIndex             = vk::QueueFamilyIgnored;
        barrier.image                           = image;
        barrier.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
        barrier.subresourceRange.baseMipLevel   = 0;
        barrier.subresourceRange.levelCount     = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount     = 1;

        commandBuffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTopOfPipe,
            vk::PipelineStageFlagBits::eAllCommands,
            vk::DependencyFlags(0),
            0, nullptr, 0, nullptr, 1, &barrier
        );
    }

    void VulkanInterface::transitionImageToComputeReadState(vk::CommandBuffer commandBuffer, vk::Image image, vk::ClearColorValue& clearColor) {
        vk::ImageMemoryBarrier barrier;
        barrier.srcAccessMask                   = vk::AccessFlags(0);
        barrier.dstAccessMask                   = vk::AccessFlagBits::eTransferWrite;
        barrier.oldLayout                       = vk::ImageLayout::eUndefined;
        barrier.newLayout                       = vk::ImageLayout::eGeneral;
        barrier.srcQueueFamilyIndex             = vk::QueueFamilyIgnored;
        barrier.dstQueueFamilyIndex             = vk::QueueFamilyIgnored;
        barrier.image                           = image;
        barrier.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
        barrier.subresourceRange.baseMipLevel   = 0;
        barrier.subresourceRange.levelCount     = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount     = 1;

        commandBuffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTopOfPipe,
            vk::PipelineStageFlagBits::eTransfer,
            vk::DependencyFlags(0),
            0, nullptr, 0, nullptr, 1, &barrier
        );

        vk::ImageSubresourceRange subresourceRange;
        subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        subresourceRange.baseMipLevel = 0;
        subresourceRange.levelCount = 1;
        subresourceRange.baseArrayLayer = 0;
        subresourceRange.layerCount = 1;

        vk_commandBuffers[currentFrame].clearColorImage(vk_colorAccumImage, vk::ImageLayout::eGeneral, &clearColor, 1, &subresourceRange); 

        barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
        barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
        barrier.oldLayout     = vk::ImageLayout::eGeneral;
        barrier.newLayout     = vk::ImageLayout::eColorAttachmentOptimal;

        commandBuffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTransfer,
            vk::PipelineStageFlagBits::eFragmentShader,
            vk::DependencyFlags(0),
            0, nullptr, 0, nullptr, 1, &barrier
        );
    }

    void VulkanInterface::transitionImageToTransferState(vk::CommandBuffer commandBuffer, vk::Image image) {
        vk::ImageMemoryBarrier barrier;
        barrier.oldLayout                       = vk::ImageLayout::eUndefined;
        barrier.newLayout                       = vk::ImageLayout::eTransferDstOptimal;
        barrier.srcQueueFamilyIndex             = vk::QueueFamilyIgnored;
        barrier.dstQueueFamilyIndex             = vk::QueueFamilyIgnored;
        barrier.image                           = image;
        barrier.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
        barrier.subresourceRange.baseMipLevel   = 0;
        barrier.subresourceRange.levelCount     = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount     = 1;
    
        commandBuffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTopOfPipe,
            vk::PipelineStageFlagBits::eTransfer,
            vk::DependencyFlags(0),
            0, nullptr, 0, nullptr, 1, &barrier
        );
    }
    
    void VulkanInterface::transitionImageToPresentState(vk::CommandBuffer commandBuffer, vk::Image image) {
        vk::ImageMemoryBarrier barrier;
        barrier.srcAccessMask                   = vk::AccessFlagBits::eTransferWrite;
        barrier.dstAccessMask                   = vk::AccessFlagBits::eMemoryRead;
        barrier.oldLayout                       = vk::ImageLayout::eTransferDstOptimal;
        barrier.newLayout                       = vk::ImageLayout::ePresentSrcKHR;
        barrier.srcQueueFamilyIndex             = vk::QueueFamilyIgnored;
        barrier.dstQueueFamilyIndex             = vk::QueueFamilyIgnored;
        barrier.image                           = image;
        barrier.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
        barrier.subresourceRange.baseMipLevel   = 0;
        barrier.subresourceRange.levelCount     = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount     = 1;
    
        commandBuffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTransfer,
            vk::PipelineStageFlagBits::eBottomOfPipe,
            vk::DependencyFlags(0),
            0, nullptr, 0, nullptr, 1, &barrier
        );
    }

    Result VulkanInterface::drawFrame() {
        
        if (!vk_vertexBuffer) {
            return Result::eSuccess;
        }

        vk_runtimeResult = vk_device.waitForFences(1, &vk_inFlightFences[currentFrame], vk::True, UINT64_MAX);

        if (vk_runtimeResult != vk::Result::eSuccess) {
            return Result::eFailedToWaitForFences;
        }

        vk::ResultValue<uint32_t> result = vk_device.acquireNextImageKHR(vk_swapchain, UINT64_MAX, vk_imageAvailableSemaphores[currentFrame], nullptr);

        if (result.result != vk::Result::eSuccess) {
            if (result.result == vk::Result::eErrorOutOfDateKHR || vk_runtimeResult == vk::Result::eSuboptimalKHR) {
                return Result::eSuccess; // might be a better way to do this...
            }
        
            return Result::eFailedToGetNextSwapchainImage;
        }

        vk_runtimeResult = vk_device.resetFences(1, &vk_inFlightFences[currentFrame]);

        if (vk_runtimeResult != vk::Result::eSuccess) {
            return Result::eFailedToResetFences;
        }

        imageIndex = result.value;

        vk_commandBuffers[currentFrame].reset();
        vk_commandBuffers[currentFrame].begin(vk_beginInfo);

        vk_renderpassBeginInfo.framebuffer = vk_swapchainFramebuffers[imageIndex];
        
        transitionImageToComputeReadState(vk_commandBuffers[currentFrame], vk_colorAccumImage, vk_clearValues[0].color);
        transitionImageToComputeReadState(vk_commandBuffers[currentFrame], vk_alphaAccumImage, vk_clearValues[1].color);
        
        vk_commandBuffers[currentFrame].beginRenderPass(vk_renderpassBeginInfo, vk::SubpassContents::eInline);
        
        vk_commandBuffers[currentFrame].bindPipeline(vk::PipelineBindPoint::eGraphics, vk_graphicsPipeline);
        vk_commandBuffers[currentFrame].bindVertexBuffers(0, 1, &vk_vertexBuffer, &vk_vertexBufferOffsets);
        vk_commandBuffers[currentFrame].bindIndexBuffer(vk_indexBuffer, 0, vk::IndexType::eUint16);
        vk_commandBuffers[currentFrame].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, vk_graphicsPipelineLayout, 0, 1, &vk_descriptorSet, 0, nullptr);
        
        vk_commandBuffers[currentFrame].setViewport(0, 1, &vk_viewport);
        vk_commandBuffers[currentFrame].setScissor(0, 1, &vk_scissor);
        
        vk_commandBuffers[currentFrame].drawIndexed(knownIndicesSize, 1, 0, 0, 0);
        
        vk_commandBuffers[currentFrame].endRenderPass();
        
        transitionImageToComputeWriteState(vk_commandBuffers[currentFrame], vk_outputImage);

        vk::ClearColorValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};

        vk::ImageSubresourceRange subresourceRange;
        subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        subresourceRange.baseMipLevel = 0;
        subresourceRange.levelCount = 1;
        subresourceRange.baseArrayLayer = 0;
        subresourceRange.layerCount = 1;
        
        vk_commandBuffers[currentFrame].clearColorImage(vk_outputImage, vk::ImageLayout::eGeneral, &clearColor, 1, &subresourceRange); 
        
        vk_commandBuffers[currentFrame].bindPipeline(vk::PipelineBindPoint::eCompute, vk_computePipeline);
        vk_commandBuffers[currentFrame].bindDescriptorSets(vk::PipelineBindPoint::eCompute, vk_graphicsPipelineLayout, 0, 1, &vk_descriptorSet, 0, nullptr);
        vk_commandBuffers[currentFrame].dispatch(vk_swapchainExtent.width, vk_swapchainExtent.height, 1);

        transitionImageToTransferState(vk_commandBuffers[currentFrame], vk_swapchainImages[imageIndex]);
        
        vk::ImageCopy presentCopyRegion;
        presentCopyRegion.srcSubresource.aspectMask     = vk::ImageAspectFlagBits::eColor;
        presentCopyRegion.srcSubresource.mipLevel       = 0;
        presentCopyRegion.srcSubresource.baseArrayLayer = 0;
        presentCopyRegion.srcSubresource.layerCount     = 1;
        presentCopyRegion.dstSubresource.aspectMask     = vk::ImageAspectFlagBits::eColor;
        presentCopyRegion.dstSubresource.mipLevel       = 0;
        presentCopyRegion.dstSubresource.baseArrayLayer = 0;
        presentCopyRegion.dstSubresource.layerCount     = 1;
        presentCopyRegion.srcOffset                     = vk::Offset3D{0, 0, 0};
        presentCopyRegion.dstOffset                     = vk::Offset3D{0, 0, 0};
        presentCopyRegion.extent.width                  = vk_swapchainExtent.width;
        presentCopyRegion.extent.height                 = vk_swapchainExtent.height;
        presentCopyRegion.extent.depth                  = 1;

        vk_commandBuffers[currentFrame].copyImage(
            vk_outputImage, 
            vk::ImageLayout::eGeneral, 
            vk_swapchainImages[imageIndex],
            vk::ImageLayout::eTransferDstOptimal,
            1, &presentCopyRegion
        );

        transitionImageToPresentState(vk_commandBuffers[currentFrame], vk_swapchainImages[imageIndex]);

        vk_commandBuffers[currentFrame].end();

        vk_submitInfo.pWaitSemaphores   = &vk_imageAvailableSemaphores[currentFrame];
        vk_submitInfo.pCommandBuffers   = &vk_commandBuffers[currentFrame];
        vk_submitInfo.pSignalSemaphores = &vk_renderFinishedSemaphores[currentFrame];

        vk_runtimeResult = vk_graphicsQueue.submit(1, &vk_submitInfo, vk_inFlightFences[currentFrame]);

        if (vk_runtimeResult != vk::Result::eSuccess) {
            return Result::eFailedToSubmitQueue;
        }

        vk_presentInfo.pWaitSemaphores = &vk_renderFinishedSemaphores[currentFrame];
        vk_presentInfo.pImageIndices   = &imageIndex;

        vk_runtimeResult = vk_presentQueue.presentKHR(&vk_presentInfo);

        if (vk_runtimeResult != vk::Result::eSuccess) {
            if (vk_runtimeResult == vk::Result::eErrorOutOfDateKHR || vk_runtimeResult == vk::Result::eSuboptimalKHR) {
                return Result::eSuccess;
            }
        
            return Result::eFailedToGetNextSwapchainImage;
        }

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

        return Result::eSuccess;
    }
}