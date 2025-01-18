#include "vulkan.hpp"

namespace SplitGui {
    VulkanInterface::~VulkanInterface() {
        cleanupSyncObj();
                
        vk_device.freeCommandBuffers(vk_commandPool, vk_commandBuffers.size(), vk_commandBuffers.data());

        cleanupVertexAndIndexBuffers();

        vk_device.destroyCommandPool(vk_commandPool);
        
        cleanupFrameBuffers();

        vk_device.unmapMemory(vk_vertexUniformBufferMemory);
        vk_device.freeMemory(vk_vertexUniformBufferMemory);
        vk_device.destroyBuffer(vk_vertexUniformBuffer);

        //vk_device.freeDescriptorSets(vk_descriptorPool, vk_descriptorSets.size(), vk_descriptorSets.data());
        vk_device.destroyDescriptorPool(vk_descriptorPool);

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
            FT_Done_Face(ft_face);
        }

        FT_Done_FreeType(ft_lib);
    }

    void VulkanInterface::cleanupFrameBuffers() {
        for (int i = 0; i < vk_swapchainFramebuffers.size(); i++) {
            vk_device.destroyFramebuffer(vk_swapchainFramebuffers[i]);
        }
        vk_swapchainFramebuffers.clear();
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
        for (int i = 0; i < vk_swapchainImageViews.size(); i++) {
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
}