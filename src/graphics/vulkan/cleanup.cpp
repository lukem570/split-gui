#include "vulkan.hpp"

namespace SplitGui {
    VulkanInterface::~VulkanInterface() {
        vk_device.waitIdle();

        cleanupSyncObj();

        vk_device.freeCommandBuffers(vk_commandPool, vk_commandBuffers.size(), vk_commandBuffers.data());

        vk_device.destroyImageView(vk_textGlyphImageView);
        vk_device.destroySampler(vk_textGlyphSampler);
        vk_device.freeMemory(vk_textGlyphImageMemory);
        vk_device.destroyImage(vk_textGlyphImages);

        vk_device.destroyImageView(vk_textureArrayImageView);
        vk_device.destroySampler(vk_textureArraySampler);
        vk_device.freeMemory(vk_textureArrayImageMemory);
        vk_device.destroyImage(vk_textureArrayImages);
        
        vk_device.freeMemory(vk_vertexUniformBufferMemory);
        vk_device.destroyBuffer(vk_vertexUniformBuffer);

        vk_device.destroySampler(vk_colorAccumSampler);
        vk_device.destroySampler(vk_alphaAccumSampler);

        cleanupVertexAndIndexBuffers();
        cleanupSceneBuffer();

        vk_device.destroyCommandPool(vk_commandPool);
        
        cleanupFrameBuffers();
        cleanupDepthResources();
        cleanupTransparentResources();
        
        vk_device.freeDescriptorSets(vk_descriptorPool, 1, &vk_descriptorSet);
        vk_device.destroyDescriptorPool(vk_descriptorPool);

        vk_device.destroyPipeline(vk_computePipeline);
        vk_device.destroyPipeline(vk_graphicsPipeline);
        vk_device.destroyPipelineLayout(vk_graphicsPipelineLayout);
        vk_device.destroyDescriptorSetLayout(vk_descriptorSetLayout);
        vk_device.destroyRenderPass(vk_renderpass);
        
        cleanupImageViews();

        vk_device.destroySwapchainKHR(vk_swapchain);
        vk_device.destroy();

        vk_instance.destroySurfaceKHR(vk_surface);
        vk_instance.destroy();

        if (ft_fontInUse) {
            ft::FT_Done_Face(ft_face);
        }

        ft::FT_Done_FreeType(ft_lib);

        SPLITGUI_LOG("Cleaned Up Vulkan");
    }

    void VulkanInterface::cleanupFrameBuffers() {
        for (unsigned int i = 0; i < vk_swapchainFramebuffers.size(); i++) {
            vk_device.destroyFramebuffer(vk_swapchainFramebuffers[i]);
        }
        vk_swapchainFramebuffers.clear();
    }

    void VulkanInterface::cleanupDepthResources() {
        vk_device.destroyImageView(vk_depthImageView);
        vk_device.freeMemory(vk_depthImageMemory);
        vk_device.destroyImage(vk_depthImage);
    }

    void VulkanInterface::cleanupTransparentResources() {
        vk_device.destroyImageView(vk_colorAccumImageView);
        vk_device.freeMemory(vk_colorAccumImageMemory);
        vk_device.destroyImage(vk_colorAccumImage);

        vk_device.destroyImageView(vk_alphaAccumImageView);
        vk_device.freeMemory(vk_alphaAccumImageMemory);
        vk_device.destroyImage(vk_alphaAccumImage);

        vk_device.destroyImageView(vk_outputImageView);
        vk_device.freeMemory(vk_outputImageMemory);
        vk_device.destroyImage(vk_outputImage);
    }

    void VulkanInterface::cleanupSyncObj() {
        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vk_device.destroySemaphore(vk_imageAvailableSemaphores[i]);
            vk_device.destroySemaphore(vk_renderFinishedSemaphores[i]);
            vk_device.destroyFence(vk_inFlightFences[i]);
        }

        vk_imageAvailableSemaphores.clear();
        vk_renderFinishedSemaphores.clear();
        vk_inFlightFences.clear();
    }

    void VulkanInterface::cleanupImageViews() {
        for (unsigned int i = 0; i < vk_swapchainImageViews.size(); i++) {
            vk_device.destroyImageView(vk_swapchainImageViews[i]);
        }
        vk_swapchainImageViews.clear();
    }

    void VulkanInterface::cleanupVertexAndIndexBuffers() {
        if (vk_vertexBufferMemory) {
            vk_device.freeMemory(vk_vertexBufferMemory);
            vk_device.destroyBuffer(vk_vertexBuffer);
        }

        if (vk_indexBufferMemory) {
            vk_device.freeMemory(vk_indexBufferMemory);
            vk_device.destroyBuffer(vk_indexBuffer);
        }
    }

    void VulkanInterface::cleanupSceneBuffer() {
        if (vk_sceneBufferMemory) {
            vk_device.freeMemory(vk_sceneBufferMemory);
            vk_device.destroyBuffer(vk_sceneBuffer);
        }
    }
}