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

            vk_renderpassBeginInfo.framebuffer = scenes[i].framebuffers[imageIndex];

            vk_commandBuffers[currentFrame].beginRenderPass(vk_renderpassBeginInfo, vk::SubpassContents::eInline);

            vk_commandBuffers[currentFrame].bindPipeline(vk::PipelineBindPoint::eGraphics, scenes[i].pipeline);
            vk_commandBuffers[currentFrame].bindVertexBuffers(0, 1, &scenes[i].vertexBuffer, &vk_vertexBufferOffsets);
            vk_commandBuffers[currentFrame].bindIndexBuffer(scenes[i].indexBuffer, 0, vk::IndexType::eUint16);
            vk_commandBuffers[currentFrame].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, vk_scenePipelineLayout, 0, 1, &scenes[i].descriptorSet, 0, nullptr);

            vk_commandBuffers[currentFrame].setViewport(0, 1, &vk_viewport);
            vk_commandBuffers[currentFrame].setScissor(0, 1, &vk_scissor);
            
            vk_commandBuffers[currentFrame].drawIndexed(scenes[i].knownIndicesSize, 1, 0, 0, 0);

            vk_commandBuffers[currentFrame].endRenderPass();
        }

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