#include "vulkan.hpp"

namespace SplitGui {
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

        for (unsigned int i = 0; i < scenes.size(); i++) {

            if (!scenes[i].vertexBuffer) {
                continue;
            }

            vk::RenderPassBeginInfo renderpassBeginInfo;

            renderpassBeginInfo.framebuffer = scenes[i].framebuffers[imageIndex];

            renderpassBeginInfo.renderPass          = vk_renderpass;
            renderpassBeginInfo.renderArea.offset.x = 0;
            renderpassBeginInfo.renderArea.offset.y = 0;
            renderpassBeginInfo.renderArea.extent   = vk::Extent2D{ (unsigned int)scenes[i].sceneSize.x, (unsigned int)scenes[i].sceneSize.y };
            renderpassBeginInfo.clearValueCount     = vk_sceneClearValues.size();
            renderpassBeginInfo.pClearValues        = vk_sceneClearValues.data();

            vk_commandBuffers[currentFrame].beginRenderPass(renderpassBeginInfo, vk::SubpassContents::eInline);

            vk_commandBuffers[currentFrame].bindPipeline(vk::PipelineBindPoint::eGraphics, scenes[i].pipeline);
            vk_commandBuffers[currentFrame].bindVertexBuffers(0, 1, &scenes[i].vertexBuffer, &vk_vertexBufferOffsets);
            vk_commandBuffers[currentFrame].bindIndexBuffer(scenes[i].indexBuffer, 0, vk::IndexType::eUint16);
            vk_commandBuffers[currentFrame].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, vk_scenePipelineLayout, 0, 1, &scenes[i].descriptorSet, 0, nullptr);

            vk::Viewport viewport;
            viewport.x        = 0.0f;
            viewport.y        = 0.0f;
            viewport.width    = scenes[i].sceneSize.x;
            viewport.height   = scenes[i].sceneSize.y;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;

            vk::Rect2D scissor;
            scissor.offset.x = 0;
            scissor.offset.y = 0;
            scissor.extent   = renderpassBeginInfo.renderArea.extent;

            vk_commandBuffers[currentFrame].setViewport(0, 1, &viewport);
            vk_commandBuffers[currentFrame].setScissor(0, 1, &scissor);
            
            vk_commandBuffers[currentFrame].drawIndexed(scenes[i].knownIndicesSize, 1, 0, 0, 0);

            vk_commandBuffers[currentFrame].endRenderPass();

            vk::ImageMemoryBarrier barrier;
            barrier.oldLayout                       = vk::ImageLayout::ePresentSrcKHR;
            barrier.newLayout                       = vk::ImageLayout::eTransferSrcOptimal;
            barrier.srcQueueFamilyIndex             = vk::QueueFamilyIgnored;
            barrier.dstQueueFamilyIndex             = vk::QueueFamilyIgnored;
            barrier.image                           = scenes[i].outputImage;
            barrier.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
            barrier.subresourceRange.baseMipLevel   = 0;
            barrier.subresourceRange.levelCount     = 1;
            barrier.subresourceRange.baseArrayLayer = 0;
            barrier.subresourceRange.layerCount     = 1;

            vk_commandBuffers[currentFrame].pipelineBarrier(
                vk::PipelineStageFlagBits::eBottomOfPipe,
                vk::PipelineStageFlagBits::eFragmentShader,
                vk::DependencyFlagBits(0),
                0, nullptr, 0, nullptr, 1,
                &barrier
            );

            vk::ImageBlit blitRegion;
            blitRegion.srcSubresource.aspectMask     = vk::ImageAspectFlagBits::eColor;
            blitRegion.srcSubresource.mipLevel       = 0;
            blitRegion.srcSubresource.baseArrayLayer = 0;
            blitRegion.srcSubresource.layerCount     = 1;

            blitRegion.srcOffsets[0] = vk::Offset3D{ 0, 0, 0 };
            blitRegion.srcOffsets[1] = vk::Offset3D{ scenes[i].sceneSize.x, scenes[i].sceneSize.y, 1};

            blitRegion.dstSubresource.aspectMask     = vk::ImageAspectFlagBits::eColor;
            blitRegion.dstSubresource.mipLevel       = 0;
            blitRegion.dstSubresource.baseArrayLayer = i;
            blitRegion.dstSubresource.layerCount     = 1;

            blitRegion.dstOffsets[0] = vk::Offset3D{ 0, 0, 0 };
            blitRegion.dstOffsets[1] = vk::Offset3D{ (int)vk_swapchainExtent.width, (int)vk_swapchainExtent.height, 1};

            vk_commandBuffers[currentFrame].blitImage(
                scenes[i].outputImage, vk::ImageLayout::eTransferSrcOptimal,
                vk_scenesImageArrayImages, vk::ImageLayout::eTransferDstOptimal,
                1, &blitRegion, vk::Filter::eNearest
            );
        }

        vk::ImageMemoryBarrier barrier;
        barrier.oldLayout                       = vk::ImageLayout::eTransferDstOptimal;
        barrier.newLayout                       = vk::ImageLayout::eShaderReadOnlyOptimal;
        barrier.srcQueueFamilyIndex             = vk::QueueFamilyIgnored;
        barrier.dstQueueFamilyIndex             = vk::QueueFamilyIgnored;
        barrier.image                           = vk_scenesImageArrayImages;
        barrier.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
        barrier.subresourceRange.baseMipLevel   = 0;
        barrier.subresourceRange.levelCount     = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount     = MAX_SCENES;
        barrier.srcAccessMask                   = vk::AccessFlagBits::eMemoryWrite;
        barrier.dstAccessMask                   = vk::AccessFlagBits::eMemoryRead;

        vk_commandBuffers[currentFrame].pipelineBarrier(
            vk::PipelineStageFlagBits::eTransfer, 
            vk::PipelineStageFlagBits::eFragmentShader, 
            vk::DependencyFlagBits(0), 
            0, nullptr, 0, nullptr, 1, 
            &barrier
        );

        vk_renderpassBeginInfo.framebuffer = vk_swapchainFramebuffers[imageIndex];
        
        vk_commandBuffers[currentFrame].beginRenderPass(vk_renderpassBeginInfo, vk::SubpassContents::eInline);

        vk_commandBuffers[currentFrame].bindPipeline(vk::PipelineBindPoint::eGraphics, vk_graphicsPipeline);
        vk_commandBuffers[currentFrame].bindVertexBuffers(0, 1, &vk_vertexBuffer, &vk_vertexBufferOffsets);
        vk_commandBuffers[currentFrame].bindIndexBuffer(vk_indexBuffer, 0, vk::IndexType::eUint16);
        vk_commandBuffers[currentFrame].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, vk_graphicsPipelineLayout, 0, 1, &vk_descriptorSet, 0, nullptr);

        vk_commandBuffers[currentFrame].setViewport(0, 1, &vk_viewport);
        vk_commandBuffers[currentFrame].setScissor(0, 1, &vk_scissor);

        vk_commandBuffers[currentFrame].drawIndexed(knownIndicesSize, 1, 0, 0, 0);

        vk_commandBuffers[currentFrame].endRenderPass();

        vk::ImageMemoryBarrier revertBarrier;
        revertBarrier.oldLayout                       = vk::ImageLayout::eShaderReadOnlyOptimal;
        revertBarrier.newLayout                       = vk::ImageLayout::eTransferDstOptimal;
        revertBarrier.srcQueueFamilyIndex             = vk::QueueFamilyIgnored;
        revertBarrier.dstQueueFamilyIndex             = vk::QueueFamilyIgnored;
        revertBarrier.image                           = vk_scenesImageArrayImages;
        revertBarrier.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
        revertBarrier.subresourceRange.baseMipLevel   = 0;
        revertBarrier.subresourceRange.levelCount     = 1;
        revertBarrier.subresourceRange.baseArrayLayer = 0;
        revertBarrier.subresourceRange.layerCount     = MAX_SCENES;
        revertBarrier.srcAccessMask                   = vk::AccessFlagBits::eMemoryRead;
        revertBarrier.dstAccessMask                   = vk::AccessFlagBits::eMemoryWrite;

        vk_commandBuffers[currentFrame].pipelineBarrier(
            vk::PipelineStageFlagBits::eFragmentShader, 
            vk::PipelineStageFlagBits::eBottomOfPipe, 
            vk::DependencyFlagBits(0), 
            0, nullptr, 0, nullptr, 1, 
            &revertBarrier
        );

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