#include "vulkan.hpp"

namespace SplitGui {
    VulkanInterface::~VulkanInterface() {
        SPLITGUI_PROFILE;

        vk_device.waitIdle();

        cleanupSyncObj();

        cleanupStagingBuffer(vk_textureArrayStagingBuffer);
        cleanupStagingBuffer(vk_rectStagingBuffer);
        cleanupStagingBuffer(vk_indexStagingBuffer);
        cleanupStagingBuffer(vk_vertexStagingBuffer);

        vk_device.freeCommandBuffers(vk_frameCommandPool, vk_commandBuffers.size(), vk_commandBuffers.data());

        vk_device.destroyImageView(vk_textGlyphImageView);
        vk_device.destroySampler(vk_textGlyphSampler);
        vk_device.freeMemory(vk_textGlyphImageMemory);
        vk_device.destroyImage(vk_textGlyphImages);

        vk_device.destroyImageView(vk_textureArrayImageView);
        vk_device.destroySampler(vk_textureArraySampler);
        vk_device.freeMemory(vk_textureArrayImageMemory);
        vk_device.destroyImage(vk_textureArrayImages);

        cleanupVectorEngineInstances();
        cleanupScenes();
        cleanupVertexAndIndexBuffers();
        cleanupScenesImageArray();

        vk_device.destroyCommandPool(vk_frameCommandPool);

        for (unsigned int i = 0; i < graphicsQueueCount; i++) {
            vk_device.destroyCommandPool(vk_interactionCommandPools[i]);
        }
        
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
        SPLITGUI_PROFILE;

        for (unsigned int i = 0; i < vk_swapchainFramebuffers.size(); i++) {
            vk_device.destroyFramebuffer(vk_swapchainFramebuffers[i]);
        }
        vk_swapchainFramebuffers.clear();
    }

    inline void VulkanInterface::cleanupDepthResources() {
        SPLITGUI_PROFILE;

        vk_device.destroyImageView(vk_depthImageView);
        vk_device.freeMemory(vk_depthImageMemory);
        vk_device.destroyImage(vk_depthImage);
    }

    inline void VulkanInterface::cleanupSyncObj() {
        SPLITGUI_PROFILE;

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
        SPLITGUI_PROFILE;

        for (unsigned int i = 0; i < vk_swapchainImageViews.size(); i++) {
            vk_device.destroyImageView(vk_swapchainImageViews[i]);
        }
        vk_swapchainImageViews.clear();
    }

    inline void VulkanInterface::cleanupVertexAndIndexBuffers() {
        SPLITGUI_PROFILE;

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
        SPLITGUI_PROFILE;

        vk_device.destroyImageView(vk_scenesImageArrayImageView);
        vk_device.destroySampler(vk_scenesImageArraySampler);
        vk_device.freeMemory(vk_scenesImageArrayImageMemory);
        vk_device.destroyImage(vk_scenesImageArrayImages);
    }

    inline void VulkanInterface::cleanupSceneFrameBuffers(SceneRef& ref) {
        SPLITGUI_PROFILE;

        for (unsigned int j = 0; j < scenes[ref.sceneNumber].framebuffers.size(); j++) {
            vk_device.destroyFramebuffer(scenes[ref.sceneNumber].framebuffers[j]);
        }
    }

    inline void VulkanInterface::cleanupSceneOutputImages(SceneRef& ref) {
        SPLITGUI_PROFILE;

        vk_device.destroyImageView(scenes[ref.sceneNumber].outputImageView);
        vk_device.freeMemory(scenes[ref.sceneNumber].outputImageMemory);
        vk_device.destroyImage(scenes[ref.sceneNumber].outputImage);
    }

    inline void VulkanInterface::cleanupSceneDepthImages(SceneRef& ref) {
        SPLITGUI_PROFILE;

        vk_device.destroyImageView(scenes[ref.sceneNumber].depthImageView);
        vk_device.freeMemory(scenes[ref.sceneNumber].depthImageMemory);
        vk_device.destroyImage(scenes[ref.sceneNumber].depthImage);
    }

    inline void VulkanInterface::cleanupVectorEngineEdgeResources(VectorEngineObject& vEngine) {
        if (vEngine.edgeBufferMemory) {

            vk_device.freeMemory(vEngine.edgeBufferMemory);
            vk_device.destroyBuffer(vEngine.edgeBuffer);
            
            vk_device.freeMemory(vEngine.transformedEdgeBufferMemory);
            vk_device.destroyBuffer(vEngine.transformedEdgeBuffer);
        }
    }

    inline void VulkanInterface::cleanupVectorEngineInstances() {
        SPLITGUI_PROFILE;

        for (unsigned int i = 0; i < vectorEngineInstances.size(); i++) {

            cleanupStagingBuffer(vectorEngineInstances[i].edgeStagingBuffer);

            vk_device.freeDescriptorSets(vectorEngineInstances[i].descriptorPool, 1, &vectorEngineInstances[i].descriptorSet);

            cleanupVectorEngineEdgeResources(vectorEngineInstances[i]);

            vk_device.destroyDescriptorPool(vectorEngineInstances[i].descriptorPool);
            
            vk_device.destroyPipeline(vectorEngineInstances[i].transformPipeline);
            vk_device.destroyPipeline(vectorEngineInstances[i].renderPipeline);
        }
        
        vk_device.destroyShaderModule(vk_vectorEngineTransformModule);
        vk_device.destroyShaderModule(vk_vectorEngineRenderModule);

        vk_device.destroyDescriptorSetLayout(vk_vectorEngineDescriptorSetLayout);
        vk_device.destroyPipelineLayout(vk_vectorEnginePipelineLayout);
    }

    inline void VulkanInterface::cleanupModelUniform(SceneRef& ref) {
        if (scenes[ref.sceneNumber].modelUniformBufferMemory) {
            vk_device.freeMemory(scenes[ref.sceneNumber].modelUniformBufferMemory);
            vk_device.destroyBuffer(scenes[ref.sceneNumber].modelUniformBuffer);
        }
    }

    inline void VulkanInterface::cleanupScenes() {
        SPLITGUI_PROFILE;

        for (unsigned int i = 0; i < scenes.size(); i++) {

            cleanupStagingBuffer(scenes[i].sceneDataStagingBuffer);
            cleanupStagingBuffer(scenes[i].sceneModelStagingBuffer);
            cleanupStagingBuffer(scenes[i].vertexStagingBuffer);
            cleanupStagingBuffer(scenes[i].indexStagingBuffer);

            vk_device.freeDescriptorSets(scenes[i].descriptorPool, 1, &scenes[i].descriptorSet);

            vk_device.freeMemory(scenes[i].dataUniformBufferMemory);
            vk_device.destroyBuffer(scenes[i].dataUniformBuffer);

            SceneRef ref;
            ref.sceneNumber = i;

            cleanupSceneVertexAndIndexBuffers(ref);
            cleanupSceneDepthImages(ref);
            cleanupSceneOutputImages(ref);
            cleanupSceneFrameBuffers(ref);
            cleanupModelUniform(ref);

            vk_device.destroyDescriptorPool(scenes[i].descriptorPool);
            
            vk_device.destroyPipeline(scenes[i].pipeline);
        }

        vk_device.destroyShaderModule(vk_sceneVertexModule);
        vk_device.destroyShaderModule(vk_sceneFragmentModule);

        vk_device.destroyDescriptorSetLayout(vk_sceneDescriptorSetLayout);
        vk_device.destroyPipelineLayout(vk_scenePipelineLayout);

        vk_device.destroyRenderPass(vk_sceneRenderpass);
    }

    inline void VulkanInterface::cleanupSceneVertexAndIndexBuffers(SceneRef& ref) {
        SPLITGUI_PROFILE;

        if (scenes[ref.sceneNumber].vertexBufferMemory) {
            vk_device.freeMemory(scenes[ref.sceneNumber].vertexBufferMemory);
            vk_device.destroyBuffer(scenes[ref.sceneNumber].vertexBuffer);
        }

        if (scenes[ref.sceneNumber].indexBufferMemory) {
            vk_device.freeMemory(scenes[ref.sceneNumber].indexBufferMemory);
            vk_device.destroyBuffer(scenes[ref.sceneNumber].indexBuffer);
        }
    }

    inline void VulkanInterface::cleanupStagingBuffer(StagingBuffer& stagingBuffer) {
        if (stagingBuffer.memory) {
            vk_device.freeMemory(stagingBuffer.memory);
            vk_device.destroyBuffer(stagingBuffer.buffer);
        }
    }
}