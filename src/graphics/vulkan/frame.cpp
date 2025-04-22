#include "vulkan.hpp"

namespace SplitGui {

    inline void VulkanInterface::imageBarrier(
        vk::Image& image, 
        vk::ImageLayout oldLayout, 
        vk::ImageLayout newLayout, 
        vk::PipelineStageFlags src, 
        vk::PipelineStageFlagBits dst, 
        uint32_t layerCount,
        vk::AccessFlags srcAccess,
        vk::AccessFlags dstAccess
    ) {
        SPLITGUI_PROFILE;

        vk::ImageMemoryBarrier barrier;
        barrier.oldLayout                       = oldLayout;
        barrier.newLayout                       = newLayout;
        barrier.srcQueueFamilyIndex             = vk::QueueFamilyIgnored;
        barrier.dstQueueFamilyIndex             = vk::QueueFamilyIgnored;
        barrier.image                           = image;
        barrier.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
        barrier.subresourceRange.baseMipLevel   = 0;
        barrier.subresourceRange.levelCount     = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount     = layerCount;

        vk_commandBuffers[currentFrame].pipelineBarrier(
            src,
            dst,
            vk::DependencyFlagBits(0),
            0, nullptr, 0, nullptr, 1,
            &barrier
        );
    }

    inline void VulkanInterface::renderVectorEngineInstance(VectorEngineObject& vEngine, SceneObject& scene) {
        SPLITGUI_PROFILE;

        vk_commandBuffers[currentFrame].bindPipeline(vk::PipelineBindPoint::eCompute, vEngine.transformPipeline);
        vk_commandBuffers[currentFrame].bindDescriptorSets(vk::PipelineBindPoint::eCompute, vk_vectorEnginePipelineLayout, 0, 1, &vEngine.descriptorSet, 0, nullptr);
        vk_commandBuffers[currentFrame].dispatch(vEngine.edges.size() / LOCAL_COMPUTE_SIZE, 1, 1);
        
        vk_commandBuffers[currentFrame].bindPipeline(vk::PipelineBindPoint::eCompute, vEngine.renderPipeline);
        vk_commandBuffers[currentFrame].bindDescriptorSets(vk::PipelineBindPoint::eCompute, vk_vectorEnginePipelineLayout, 0, 1, &vEngine.descriptorSet, 0, nullptr);
        vk_commandBuffers[currentFrame].dispatch(scene.sceneSize.x / LOCAL_COMPUTE_SIZE, scene.sceneSize.y / LOCAL_COMPUTE_SIZE, 1);
    }

    inline void VulkanInterface::renderScene(SceneObject& scene) {
        SPLITGUI_PROFILE;

        vk::RenderPassBeginInfo renderpassBeginInfo;

        renderpassBeginInfo.framebuffer = scene.framebuffers[imageIndex];

        renderpassBeginInfo.renderPass          = vk_sceneRenderpass;
        renderpassBeginInfo.renderArea.offset.x = 0;
        renderpassBeginInfo.renderArea.offset.y = 0;
        renderpassBeginInfo.renderArea.extent   = vk::Extent2D{ (unsigned int)scene.sceneSize.x, (unsigned int)scene.sceneSize.y };
        renderpassBeginInfo.clearValueCount     = vk_sceneClearValues.size();
        renderpassBeginInfo.pClearValues        = vk_sceneClearValues.data();

        vk_commandBuffers[currentFrame].beginRenderPass(renderpassBeginInfo, vk::SubpassContents::eInline);

        vk_commandBuffers[currentFrame].bindPipeline(vk::PipelineBindPoint::eGraphics, scene.pipeline);
        vk_commandBuffers[currentFrame].bindVertexBuffers(0, 1, &scene.vertexBuffer, &vk_vertexBufferOffsets);
        vk_commandBuffers[currentFrame].bindIndexBuffer(scene.indexBuffer, 0, vk::IndexType::eUint16);
        vk_commandBuffers[currentFrame].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, vk_scenePipelineLayout, 0, 1, &scene.descriptorSet, 0, nullptr);

        vk::Viewport viewport;
        viewport.x        = 0.0f;
        viewport.y        = 0.0f;
        viewport.width    = scene.sceneSize.x;
        viewport.height   = scene.sceneSize.y;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        vk::Rect2D scissor;
        scissor.offset.x = 0;
        scissor.offset.y = 0;
        scissor.extent   = renderpassBeginInfo.renderArea.extent;

        vk_commandBuffers[currentFrame].setViewport(0, 1, &viewport);
        vk_commandBuffers[currentFrame].setScissor(0, 1, &scissor);
        
        vk_commandBuffers[currentFrame].drawIndexed(scene.knownIndicesSize, 1, 0, 0, 0);

        vk_commandBuffers[currentFrame].endRenderPass();
    }

    inline void VulkanInterface::drawScene(SceneObject& scene, unsigned int index) {
        SPLITGUI_PROFILE;

        if (!scene.vertexBuffer) {
            return;
        }

        renderScene(scene);

        if (scene.vEngineRef.has_value()) {

            VectorEngineObject& vEngine = vectorEngineInstances[scene.vEngineRef.value().instanceNumber];
            renderVectorEngineInstance(vEngine, scene);
        }

        imageBarrier(
            scene.outputImage,
            vk::ImageLayout::ePresentSrcKHR, vk::ImageLayout::eTransferSrcOptimal,
            vk::PipelineStageFlagBits::eBottomOfPipe, vk::PipelineStageFlagBits::eFragmentShader
        );

        vk::ImageBlit blitRegion;
        blitRegion.srcSubresource.aspectMask     = vk::ImageAspectFlagBits::eColor;
        blitRegion.srcSubresource.mipLevel       = 0;
        blitRegion.srcSubresource.baseArrayLayer = 0;
        blitRegion.srcSubresource.layerCount     = 1;

        blitRegion.srcOffsets[0] = vk::Offset3D{ 0, 0, 0 };
        blitRegion.srcOffsets[1] = vk::Offset3D{ scene.sceneSize.x, scene.sceneSize.y, 1};

        blitRegion.dstSubresource.aspectMask     = vk::ImageAspectFlagBits::eColor;
        blitRegion.dstSubresource.mipLevel       = 0;
        blitRegion.dstSubresource.baseArrayLayer = index;
        blitRegion.dstSubresource.layerCount     = 1;

        blitRegion.dstOffsets[0] = vk::Offset3D{ 0, 0, 0 };
        blitRegion.dstOffsets[1] = vk::Offset3D{ (int)vk_swapchainExtent.width, (int)vk_swapchainExtent.height, 1};

        vk_commandBuffers[currentFrame].blitImage(
            scene.outputImage, vk::ImageLayout::eTransferSrcOptimal,
            vk_scenesImageArrayImages, vk::ImageLayout::eTransferDstOptimal,
            1, &blitRegion, vk::Filter::eNearest
        );
    }

    void VulkanInterface::renderInterface() {
        SPLITGUI_PROFILE;

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
    }

    inline Result VulkanInterface::presentFrame() {
        SPLITGUI_PROFILE;

        vk_submitInfo.pWaitSemaphores   = &vk_imageAvailableSemaphores[currentFrame];
        vk_submitInfo.pCommandBuffers   = &vk_commandBuffers[currentFrame];
        vk_submitInfo.pSignalSemaphores = &vk_renderFinishedSemaphores[currentFrame];

        queueMutex.lock();

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

        queueMutex.unlock();

        return Result::eSuccess;
    }

    Result VulkanInterface::waitFences() {
        SPLITGUI_PROFILE;

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

        return Result::eSuccess;
    }

    Result VulkanInterface::drawFrame() {
        SPLITGUI_PROFILE;

        frameMutex.lock();
        
        if (!vk_vertexBuffer) {
            return Result::eSuccess;
        }

        TRYR(fenceRes, waitFences());

        commandPoolMutex.lock();

        vk_commandBuffers[currentFrame].reset();
        vk_commandBuffers[currentFrame].begin(vk_beginInfo);

        for (unsigned int i = 0; i < scenes.size(); i++) {
            drawScene(scenes[i], i);
        }

        imageBarrier(
            vk_scenesImageArrayImages,
            vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal,
            vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, 
            MAX_SCENES,
            vk::AccessFlagBits::eMemoryWrite, vk::AccessFlagBits::eMemoryRead
        );
        
        renderInterface();

        imageBarrier(
            vk_scenesImageArrayImages,
            vk::ImageLayout::eShaderReadOnlyOptimal, vk::ImageLayout::eTransferDstOptimal,
            vk::PipelineStageFlagBits::eFragmentShader, vk::PipelineStageFlagBits::eBottomOfPipe, 
            MAX_SCENES,
            vk::AccessFlagBits::eMemoryRead, vk::AccessFlagBits::eMemoryWrite
        );

        vk_commandBuffers[currentFrame].end();

        commandPoolMutex.unlock();

        TRYR(presRes, presentFrame());

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

        frameMutex.unlock();

        return Result::eSuccess;
    }
}