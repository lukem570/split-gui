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

        cleanupScenes();
        cleanupVertexAndIndexBuffers();
        cleanupScenesImageArray();

        vk_device.destroyCommandPool(vk_commandPool);
        
        cleanupFrameBuffers();
        cleanupDepthResources();
        
        vk_device.freeDescriptorSets(vk_descriptorPool, 1, &vk_descriptorSet);
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
            ft::FT_Done_Face(ft_face);
        }

        ft::FT_Done_FreeType(ft_lib);

        Logger::info("Cleaned Up Vulkan");
    }

    inline void VulkanInterface::cleanupFrameBuffers() {
        for (unsigned int i = 0; i < vk_swapchainFramebuffers.size(); i++) {
            vk_device.destroyFramebuffer(vk_swapchainFramebuffers[i]);
        }
        vk_swapchainFramebuffers.clear();
    }

    inline void VulkanInterface::cleanupDepthResources() {
        vk_device.destroyImageView(vk_depthImageView);
        vk_device.freeMemory(vk_depthImageMemory);
        vk_device.destroyImage(vk_depthImage);
    }

    inline void VulkanInterface::cleanupSyncObj() {
        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vk_device.destroySemaphore(vk_imageAvailableSemaphores[i]);
            vk_device.destroySemaphore(vk_renderFinishedSemaphores[i]);
            vk_device.destroyFence(vk_inFlightFences[i]);
        }

        vk_imageAvailableSemaphores.clear();
        vk_renderFinishedSemaphores.clear();
        vk_inFlightFences.clear();
    }

    inline void VulkanInterface::cleanupImageViews() {
        for (unsigned int i = 0; i < vk_swapchainImageViews.size(); i++) {
            vk_device.destroyImageView(vk_swapchainImageViews[i]);
        }
        vk_swapchainImageViews.clear();
    }

    inline void VulkanInterface::cleanupVertexAndIndexBuffers() {
        if (vk_vertexBufferMemory) {
            vk_device.freeMemory(vk_vertexBufferMemory);
            vk_device.destroyBuffer(vk_vertexBuffer);
        }

        if (vk_indexBufferMemory) {
            vk_device.freeMemory(vk_indexBufferMemory);
            vk_device.destroyBuffer(vk_indexBuffer);
        }
    }

    inline void VulkanInterface::cleanupScenesImageArray() {
        vk_device.destroyImageView(vk_scenesImageArrayImageView);
        vk_device.destroySampler(vk_scenesImageArraySampler);
        vk_device.freeMemory(vk_scenesImageArrayImageMemory);
        vk_device.destroyImage(vk_scenesImageArrayImages);
    }

    inline void VulkanInterface::cleanupSceneFrameBuffers(SceneRef& ref) {
        for (unsigned int j = 0; j < scenes[ref.sceneNumber].framebuffers.size(); j++) {
            vk_device.destroyFramebuffer(scenes[ref.sceneNumber].framebuffers[j]);
        }
    }

    inline void VulkanInterface::cleanupSceneOutputImages(SceneRef& ref) {
        vk_device.destroyImageView(scenes[ref.sceneNumber].outputImageView);
        vk_device.freeMemory(scenes[ref.sceneNumber].outputImageMemory);
        vk_device.destroyImage(scenes[ref.sceneNumber].outputImage);
    }

    inline void VulkanInterface::cleanupSceneDepthImages(SceneRef& ref) {
        vk_device.destroyImageView(scenes[ref.sceneNumber].depthImageView);
        vk_device.freeMemory(scenes[ref.sceneNumber].depthImageMemory);
        vk_device.destroyImage(scenes[ref.sceneNumber].depthImage);
    }

    inline void VulkanInterface::cleanupScenes() {

        for (unsigned int i = 0; i < scenes.size(); i++) {

            vk_device.freeDescriptorSets(scenes[i].descriptorPool, 1, &scenes[i].descriptorSet);

            vk_device.freeMemory(scenes[i].dataUniformBufferMemory);
            vk_device.destroyBuffer(scenes[i].dataUniformBuffer);

            SceneRef ref;
            ref.sceneNumber = i;

            cleanupSceneVertexAndIndexBuffers(ref);
            cleanupSceneDepthImages(ref);
            cleanupSceneOutputImages(ref);
            cleanupSceneFrameBuffers(ref);

            vk_device.destroyDescriptorPool(scenes[i].descriptorPool);
            
            vk_device.destroyPipeline(scenes[i].pipeline);
        }

        vk_device.destroyShaderModule(vk_sceneVertexModule);
        vk_device.destroyShaderModule(vk_sceneFragmentModule);

        vk_device.destroyDescriptorSetLayout(vk_sceneDescriptorSetLayout);
        vk_device.destroyPipelineLayout(vk_scenePipelineLayout);
    }

    inline void VulkanInterface::cleanupSceneVertexAndIndexBuffers(SceneRef& ref) {
        if (scenes[ref.sceneNumber].vertexBufferMemory) {
            vk_device.freeMemory(scenes[ref.sceneNumber].vertexBufferMemory);
            vk_device.destroyBuffer(scenes[ref.sceneNumber].vertexBuffer);
        }

        if (scenes[ref.sceneNumber].indexBufferMemory) {
            vk_device.freeMemory(scenes[ref.sceneNumber].indexBufferMemory);
            vk_device.destroyBuffer(scenes[ref.sceneNumber].indexBuffer);
        }
    }
}