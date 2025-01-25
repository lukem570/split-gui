#include "vulkan.hpp"

namespace SplitGui {
    VulkanInterface::VulkanInterface(bool validation = false) {
        vk_validation = validation;
        vk_clearColor.color = vk::ClearColorValue{0.0f, 0.0f, 0.0f, 1.0f};

        ft::FT_Error error = ft::FT_Init_FreeType(&ft_lib);

        if (error) {
            printf("could not init freetype\n");
            throw;
        }

        vk_msdfExtent.width  = 64;
        vk_msdfExtent.height = 64;
    }

    void VulkanInterface::instance() {
        instanceVulkan();
        createPhysicalDevice();
    }

    void VulkanInterface::submitWindow(SplitGui::Window& window) { // need to add physical device surface support
        pWindow = &window;
        createSurface(window);
        getQueueFamilies();
        createDevice();
        getQueues();
        createSwapchain();
        createImageViews();
        createRenderpass();
        createDescriptorSetLayout();
        createGraphicsPipelineLayout();
        createGraphicsPipeline();
        createFramebuffers();
        createCommandPool();
        createCommandBuffers();
        createSyncObj();
        createDescriptorPool();
        createDescriptorSet();
        createTextGlyphImage();
        updateDescriptorSets();

        setupRenderpassBeginInfo();
        setupViewport();
        setupScissor();
        setupSubmitInfo();
        setupPresentInfo();
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