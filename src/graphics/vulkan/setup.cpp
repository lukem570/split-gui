#include "vulkan.hpp"

namespace SplitGui {
    VulkanInterface::VulkanInterface(bool validation = false) {
        vk_validation = validation;
        vk_clearColor.color = vk::ClearColorValue{0.0f, 0.0f, 0.0f, 1.0f};
        vk_msdfExtent.width  = 64;
        vk_msdfExtent.height = 64;
    }

    Result VulkanInterface::instance() {
        
        ft::FT_Error error = ft::FT_Init_FreeType(&ft_lib);

        if (error) {
            return Result::eFailedToInstanceFreeType;
        }

        TRYR(instanceVulkan());
        TRYR(createPhysicalDevice());

        return Result::eSuccess;
    }

    Result VulkanInterface::submitWindow(SplitGui::Window& window) {
        pWindow = &window;
        TRYR(createSurface(window));
        TRYR(getQueueFamilies());
        TRYR(createDevice());
        getQueues();
        createSwapchain();
        createImageViews();
        createRenderpass();
        createDescriptorSetLayout();
        createGraphicsPipelineLayout();
        TRYR(createGraphicsPipeline());
        createFramebuffers();
        createCommandPool();
        createCommandBuffers();
        createSyncObj();
        createDescriptorPool();
        createDescriptorSet();
        TRYR(createTextGlyphImage());
        TRYR(createVertexUniformBuffer());
        updateDescriptorSets();

        setupRenderpassBeginInfo();
        setupViewport();
        setupScissor();
        setupSubmitInfo();
        setupPresentInfo();

        return Result::eSuccess;
    }

    void VulkanInterface::setupRenderpassBeginInfo() {
        vk_renderpassBeginInfo.renderPass          = vk_renderpass;
        vk_renderpassBeginInfo.renderArea.offset.x = 0;
        vk_renderpassBeginInfo.renderArea.offset.y = 0;
        vk_renderpassBeginInfo.renderArea.extent   = vk_swapchainExtent;
        vk_renderpassBeginInfo.clearValueCount     = 1;
        vk_renderpassBeginInfo.pClearValues        = &vk_clearColor;
    }

    void VulkanInterface::setupViewport() {
        vk_viewport.x        = 0.0f;
        vk_viewport.y        = 0.0f;
        vk_viewport.width    = vk_swapchainExtent.width;
        vk_viewport.height   = vk_swapchainExtent.height;
        vk_viewport.minDepth = 0.0f;
        vk_viewport.maxDepth = 1.0f;
    }

    void VulkanInterface::setupScissor() {
        vk_scissor.offset.x = 0;
        vk_scissor.offset.y = 0;
        vk_scissor.extent   = vk_swapchainExtent;
    }

    void VulkanInterface::setupSubmitInfo() {
        vk_waitStages = {vk::PipelineStageFlagBits::eColorAttachmentOutput};

        vk_submitInfo.waitSemaphoreCount   = 1;
        vk_submitInfo.pWaitDstStageMask    = &vk_waitStages;
        vk_submitInfo.commandBufferCount   = 1;
        vk_submitInfo.signalSemaphoreCount = 1;
    }

    void VulkanInterface::setupPresentInfo() {
        vk_presentInfo.waitSemaphoreCount = 1;
        vk_presentInfo.swapchainCount     = 1;
        vk_presentInfo.pSwapchains        = &vk_swapchain;
    }
}