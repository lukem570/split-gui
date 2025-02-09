#ifndef SPLITGUI_VULKAN_HPP
#define SPLITGUI_VULKAN_HPP

#include <splitgui/result.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/window.hpp>
#include <splitgui/structs.hpp>
#include <vector>
#include <tuple>
#include <cmath>
#include <bits/stdc++.h>
#include <string>
#include <sstream>
#include <array>
#include <stack>
#include <msdfgen/msdfgen.h>
#include <setjmp.h>

namespace ft {
    #include <ft2build.h>

    #include FT_FREETYPE_H
    #include FT_OUTLINE_H
}

#include "msdfLoader.cpp"
#include "../../path.cpp"

// TODO: make this accessible to the user
#define MAX_FRAMES_IN_FLIGHT 2
#define VERTEX_SHADER_PATH   "../shaders/vertex.spv"
#define GEOMETRY_SHADER_PATH "../shaders/geometry.spv"
#define FRAGMENT_SHADER_PATH "../shaders/fragment.spv"

/*
#include "vulkan.hpp"

namespace SplitGui {
    
}
*/

namespace SplitGui {

    class VulkanInterface : GraphicsLibInterface {
        public:
                                 VulkanInterface(bool validation);
                                ~VulkanInterface()                                                                                           override;

[[nodiscard]] Result             instance()                                                                                                  override;
[[nodiscard]] Result             submitWindow(SplitGui::Window& window)                                                                      override;

[[nodiscard]] Result             drawFrame()                                                                                                 override;
              RectRef            drawRect(Vec2 x1, Vec2 x2, Vec3 color, VertexFlags flags = 0, uint16_t textureIndex = 0)                    override;
              void               updateRect(RectRef& ref, Vec2 x1, Vec2 x2)                                                                  override;
              unsigned int       instanceScene(IVec2 x1, IVec2 x2)                                                                           override;
              void               updateScene(unsigned int ref, IVec2 x1, IVec2 x2)                                                           override;
              void               submitTriangleData(unsigned int sceneNumber, std::vector<Vertex>& vertices, std::vector<uint16_t>& indices) override;
[[nodiscard]] Result             updateSceneCameraRotation(unsigned int sceneNumber, Vec3& rotation)                                         override;
[[nodiscard]] Result             updateSceneCameraPosition(unsigned int sceneNumber, Vec3& position)                                         override;
[[nodiscard]] ResultValue<float> drawText(Vec2 x1, std::string& text)                                                                        override;
              void               loadFont(const char* path)                                                                                  override;
[[nodiscard]] Result             submitBuffers()                                                                                             override;

[[nodiscard]] Result             resizeEvent()                                                                                               override;

        protected:
            SplitGui::Window*                   pWindow;
        private:
            // free type variables
            ft::FT_Library                      ft_lib;
            ft::FT_Face                         ft_face;
            bool                                ft_fontInUse = false;

            // vulkan variables
            vk::Instance                        vk_instance;
            vk::PhysicalDevice                  vk_physicalDevice;
            vk::Device                          vk_device;
            vk::Queue                           vk_graphicsQueue;
            vk::Queue                           vk_presentQueue;
            vk::SurfaceKHR                      vk_surface;
            vk::SwapchainKHR                    vk_swapchain;
            vk::SurfaceFormatKHR                vk_surfaceFormat;
            vk::PresentModeKHR                  vk_presentMode;
            vk::Extent2D                        vk_swapchainExtent;
            vk::RenderPass                      vk_renderpass;
            vk::DescriptorSetLayout             vk_descriptorSetLayout;
            vk::PipelineLayout                  vk_graphicsPipelineLayout;
            vk::Pipeline                        vk_graphicsPipeline;
            vk::CommandPool                     vk_commandPool;
            vk::ClearValue                      vk_clearColor;
            vk::RenderPassBeginInfo             vk_renderpassBeginInfo;
            vk::Viewport                        vk_viewport;
            vk::DescriptorPool                  vk_descriptorPool;
            vk::Rect2D                          vk_scissor;
            vk::PipelineStageFlags              vk_waitStages;
            vk::Buffer                          vk_vertexBuffer;
            vk::DeviceMemory                    vk_vertexBufferMemory;
            vk::Buffer                          vk_indexBuffer;
            vk::DeviceMemory                    vk_indexBufferMemory;
            vk::DeviceSize                      vk_sceneBufferSize;
            vk::Buffer                          vk_sceneBuffer;
            vk::DeviceMemory                    vk_sceneBufferMemory;
            vk::Buffer                          vk_vertexUniformBuffer;
            vk::DeviceMemory                    vk_vertexUniformBufferMemory;
            vk::DescriptorSet                   vk_descriptorSet;
            vk::Extent2D                        vk_msdfExtent;
            vk::Image                           vk_textGlyphImages;
            vk::DeviceMemory                    vk_textGlyphImageMemory;
            vk::ImageView                       vk_textGlyphImageView;
            vk::Sampler                         vk_textGlyphSampler;
            std::vector<vk::CommandBuffer>      vk_commandBuffers;
            std::vector<vk::Framebuffer>        vk_swapchainFramebuffers;
            std::vector<vk::Image>              vk_swapchainImages;
            std::vector<vk::ImageView>          vk_swapchainImageViews;
            std::vector<vk::Semaphore>          vk_imageAvailableSemaphores;
            std::vector<vk::Semaphore>          vk_renderFinishedSemaphores;
            std::vector<vk::Fence>              vk_inFlightFences;
            unsigned int                        graphicsQueueFamilyIndex = -1;
            unsigned int                        presentQueueFamilyIndex  = -1;
            std::vector<const char *>           enabledLayers;
            std::vector<const char *>           enabledInstanceExtensions;
            std::vector<const char *>           enabledDeviceExtensions;

            // runtime variables
            vk::CommandBufferBeginInfo          vk_beginInfo;
            vk::SubmitInfo                      vk_submitInfo;
            vk::DeviceSize                      vk_vertexBufferOffsets = 0;
            vk::DeviceSize                      vk_stagingBufferRegion;
            vk::PresentInfoKHR                  vk_presentInfo;
            vk::Result                          vk_runtimeResult;
            unsigned int                        currentFrame     =  0;
            unsigned int                        knownIndicesSize =  0;
            unsigned int                        knownScenesSize  =  0;
            uint32_t                            imageIndex       = -1;
            VertexUniformObject                 vertexUniformObject;
            std::vector<VertexBufferObject>     vertices;
            std::vector<uint16_t>               indices;
            std::vector<SceneObj>               scenes;
            std::unordered_map<char, MSDFImage> charImageMappings;
            bool                                markVerticesForUpdate = false;
            bool                                markScenesForUpdate   = false;

            // debug
            bool                                vk_validation = false;


[[nodiscard]] inline ResultValue<vk::Bool32> checkLayers(const std::vector<const char *> &check_names, const std::vector<vk::LayerProperties> &layers);
              inline vk::Extent2D            chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);
              inline vk::SurfaceFormatKHR    chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
              inline vk::PresentModeKHR      chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
              inline vk::ShaderModule        createShaderModule(const std::vector<char>& code);
[[nodiscard]] inline ResultValue<uint32_t>   findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);
[[nodiscard]] inline Result                  createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Buffer& out_buffer, vk::DeviceMemory& out_bufferMemory);
              inline vk::CommandBuffer       startCopyBuffer();
              inline void                    copyBuffer(vk::Buffer src, vk::Buffer dst, vk::DeviceSize size, vk::CommandBuffer commandBuffer, vk::BufferCopy& copyRegion);
[[nodiscard]] inline Result                  endSingleTimeCommands(vk::CommandBuffer commandBuffer);
[[nodiscard]] inline Result                  vertexBufferSubmit();
[[nodiscard]] inline Result                  scenesSubmit();
              inline void                    updateScenes();

              template <typename T>
[[nodiscard]] inline Result InstanceStagingBuffer(std::vector<T> dataToUpload, vk::Buffer& out_buffer, vk::DeviceMemory& out_memory, vk::DeviceSize& out_size);

[[nodiscard]] inline Result instanceVulkan();
[[nodiscard]] inline Result createPhysicalDevice();
[[nodiscard]] inline Result getQueueFamilies();
[[nodiscard]] inline Result createDevice();
              inline void   getQueues();
[[nodiscard]] inline Result createSurface(SplitGui::Window& window);
              inline void   createSwapchain();
              inline void   createImageViews();
              inline void   createRenderpass();
              inline void   createDescriptorSetLayout();
              inline void   createGraphicsPipelineLayout();
[[nodiscard]] inline Result createGraphicsPipeline();
              inline void   createFramebuffers();
              inline void   createCommandPool();
              inline void   createCommandBuffers();
              inline void   createSyncObj();
              inline void   createDescriptorPool();
              inline void   createDescriptorSet();
[[nodiscard]] inline Result createTextGlyphImage();
[[nodiscard]] inline Result createVertexUniformBuffer();
              inline void   updateDescriptorSets();

              inline void setupRenderpassBeginInfo();
              inline void setupViewport();
              inline void setupScissor();
              inline void setupSubmitInfo();
              inline void setupPresentInfo();

              inline void cleanupFrameBuffers();
              inline void cleanupSyncObj();
              inline void cleanupImageViews();
              inline void cleanupVertexAndIndexBuffers();
              inline void cleanupSceneBuffer();

              void recreateSwapchain();
    };
}

#endif