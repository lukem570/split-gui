#include "vulkan.hpp"

namespace SplitGui {
    VulkanInterface::VulkanInterface(VulkanFlags flags) {
        SPLITGUI_PROFILE;

        vk_validation = flags.enableValidationLayers;

        Vec3 color = flags.clearColor.normalize();

        vk_clearValues[0].color = vk::ClearColorValue{color.r, color.g, color.b, 1.0f};
        vk_clearValues[1].depthStencil.depth   = 1.0f;
        vk_clearValues[1].depthStencil.stencil = 0;

        vk_sceneClearValues[0].color = vk::ClearColorValue{0.0f, 0.0f, 0.0f, 0.0f};
        vk_sceneClearValues[1].depthStencil.depth   = 1.0f;
        vk_sceneClearValues[1].depthStencil.stencil = 0;

        vk_msdfExtent.width  = flags.mdsfTextureSize;
        vk_msdfExtent.height = flags.mdsfTextureSize;
    }

    Result VulkanInterface::instance() {
        SPLITGUI_PROFILE;
        
        ft::FT_Error error = ft::FT_Init_FreeType(&ft_lib);

        if (error) {
            return Result::eFailedToInstanceFreeType;
        }

        TRYR(instanceRes, instanceVulkan());
        TRYR(deviceRes, createPhysicalDevice());

        return Result::eSuccess;
    }

    Result VulkanInterface::submitWindow(SplitGui::Window& window) {
        SPLITGUI_PROFILE;

        pWindow = &window;
        TRYR(surfaceRes, createSurface(window));
        TRYR(queueRes, getQueueFamilies());
        TRYR(deviceRes, createDevice());
        getQueues();
        createSwapchain();
        createImageViews();
        TRYR(renderpassRes, createRenderpass());
        createDescriptorSetLayout();
        createGraphicsPipelineLayout();
        TRYR(pipelineRes, createGraphicsPipeline());
        createCommandPool();
        TRYR(depthRes, createDepthResources());
        createFramebuffers();
        createCommandBuffers();
        createSyncObj();
        createDescriptorPool();
        createDescriptorSet();
        TRYR(glyphRes, createTextGlyphImage());
        TRYR(sceneRes, createScenesImageArray());
        TRYR(textureRes, createTextureArray());
        updateDescriptorSets();
        
        TRYR(sceneRenderpassRes, createSceneRenderpass());
        createSceneDescriptorSetLayout();
        createScenePipelineLayout();
        TRYR(moduleRes1, createScenePipelineModules());

        createVectorEngineDescriptorSetLayout();
        createVectorEnginePipelineLayout();
        TRYR(moduleRes2, createVectorEnginePipelineModules());

        setupRenderpassBeginInfo();
        setupViewport();
        setupScissor();
        setupSubmitInfo();
        setupPresentInfo();

        return Result::eSuccess;
    }

    void VulkanInterface::setupRenderpassBeginInfo() {
        SPLITGUI_PROFILE;

        vk_renderpassBeginInfo.renderPass          = vk_renderpass;
        vk_renderpassBeginInfo.renderArea.offset.x = 0;
        vk_renderpassBeginInfo.renderArea.offset.y = 0;
        vk_renderpassBeginInfo.renderArea.extent   = vk_swapchainExtent;
        vk_renderpassBeginInfo.clearValueCount     = vk_clearValues.size();
        vk_renderpassBeginInfo.pClearValues        = vk_clearValues.data();
    }

    void VulkanInterface::setupViewport() {
        SPLITGUI_PROFILE;

        vk_viewport.x        = 0.0f;
        vk_viewport.y        = 0.0f;
        vk_viewport.width    = vk_swapchainExtent.width;
        vk_viewport.height   = vk_swapchainExtent.height;
        vk_viewport.minDepth = 0.0f;
        vk_viewport.maxDepth = 1.0f;
    }

    void VulkanInterface::setupScissor() {
        SPLITGUI_PROFILE;

        vk_scissor.offset.x = 0;
        vk_scissor.offset.y = 0;
        vk_scissor.extent   = vk_swapchainExtent;
    }

    void VulkanInterface::setupSubmitInfo() {
        SPLITGUI_PROFILE;

        vk_waitStages = {vk::PipelineStageFlagBits::eColorAttachmentOutput};

        vk_submitInfo.waitSemaphoreCount   = 1;
        vk_submitInfo.pWaitDstStageMask    = &vk_waitStages;
        vk_submitInfo.commandBufferCount   = 1;
        vk_submitInfo.signalSemaphoreCount = 1;
    }

    void VulkanInterface::setupPresentInfo() {
        SPLITGUI_PROFILE;

        vk_presentInfo.waitSemaphoreCount = 1;
        vk_presentInfo.swapchainCount     = 1;
        vk_presentInfo.pSwapchains        = &vk_swapchain;
        vk_presentInfo.pResults           = nullptr;
    }
}