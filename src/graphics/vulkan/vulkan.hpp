#ifndef SPLITGUI_VULKAN_HPP
#define SPLITGUI_VULKAN_HPP

#include <splitgui/result.hpp>
#include <splitgui/graphics.hpp>
#include <splitgui/window.hpp>
#include <splitgui/structs.hpp>
#include <splitgui/logger.hpp>

#include <vulkan/vulkan.hpp>
#include <msdfgen/msdfgen.h>

#include <vector>
#include <tuple>
#include <cmath>
#include <string>
#include <sstream>
#include <array>
#include <stack>
#include <setjmp.h>
#include <unordered_set>
#include <mutex>

namespace ft {
    #include <ft2build.h>

    #include FT_FREETYPE_H
    #include FT_OUTLINE_H
}

#include "msdfLoader.cpp"
#include "../../path.cpp"

// TODO: make this accessible to the user
#define MAX_FRAMES_IN_FLIGHT 4
#define MAX_VECTOR_IMAGES 256
#define MAX_SCENES 16
#define VECTOR_RES 16
#define VERTEX_SHADER_PATH   "shaders/vertex.spv"
#define FRAGMENT_SHADER_PATH "shaders/fragment.spv"
#define SCENE_VERTEX_SHADER_PATH   "shaders/scene-vertex.spv"
#define SCENE_FRAGMENT_SHADER_PATH "shaders/scene-fragment.spv"
#define VECTOR_ENGINE_TRANSFORM_SHADER_PATH "shaders/vector-transform.spv"
#define VECTOR_ENGINE_RENDER_SHADER_PATH "shaders/vector-render.spv"

/*
#include "vulkan.hpp"

namespace SplitGui {
    
}
*/

namespace SplitGui {

    struct StagingBuffer {
        vk::DeviceSize   size = 0;
        vk::Buffer       buffer;
        vk::DeviceMemory memory;
    };

    struct SceneObject {

        vk::DescriptorPool           descriptorPool;
        vk::DescriptorSet            descriptorSet;
        vk::Buffer                   indexBuffer;
        vk::DeviceMemory             indexBufferMemory;
        vk::Buffer                   vertexBuffer;
        vk::DeviceMemory             vertexBufferMemory;
        vk::Buffer                   dataUniformBuffer;
        vk::DeviceMemory             dataUniformBufferMemory;
        vk::Buffer                   modelUniformBuffer;
        vk::DeviceMemory             modelUniformBufferMemory;
        vk::Image                    depthImage;
        vk::DeviceMemory             depthImageMemory;
        vk::ImageView                depthImageView;
        vk::Image                    outputImage;
        vk::DeviceMemory             outputImageMemory;
        vk::ImageView                outputImageView;
        std::vector<vk::Framebuffer> framebuffers;
        vk::Pipeline                 pipeline;

        IVec2 sceneSize;

        LinkList<SceneVertexBufferObject> vertices;
        LinkList<uint16_t>                indices;
        SceneObj                          sceneData;
        std::vector<Mat4>                 models;

        std::optional<VectorEngineRef> vEngineRef;

        StagingBuffer sceneDataStagingBuffer;
        StagingBuffer sceneModelStagingBuffer;
        StagingBuffer vertexStagingBuffer;
        StagingBuffer indexStagingBuffer;

        unsigned int knownIndicesSize = 0;
    };

    struct VectorEngineObject {
        vk::DescriptorPool           descriptorPool;
        vk::DescriptorSet            descriptorSet;
        vk::Buffer                   edgeBuffer; 
        vk::DeviceMemory             edgeBufferMemory; 
        vk::Buffer                   transformedEdgeBuffer;
        vk::DeviceMemory             transformedEdgeBufferMemory;
        vk::Sampler                  outputImageSampler;
        vk::Pipeline                 transformPipeline;
        vk::Pipeline                 renderPipeline;

        LinkList<VectorEdgeBufferObject> edges;
        SceneRef scene;

        StagingBuffer edgeStagingBuffer;
    };

    class VulkanInterface : GraphicsLibInterface {
        public:
                                            VulkanInterface(VulkanFlags flags);
                                        ~VulkanInterface()                                                                                                                  override;

[[nodiscard]] Result                        instance()                                                                                                                         override;
[[nodiscard]] Result                        submitWindow(SplitGui::Window& window)                                                                                             override;

[[nodiscard]] Result                        drawFrame()                                                                                                                        override;
              RectRef                       drawRect(Vec2 x1, Vec2 x2, Vec3 color, float depth = 0.0f, VertexFlags flags = 0, uint16_t textureIndex = 0)                       override;
              void                          updateRect(RectRef& ref, Vec2 x1, Vec2 x2, Vec3 color, float depth = 0.0f)                                                         override;
              void                          deleteRect(RectRef& ref)                                                                                                           override;
              void                          deleteText(TextRef& ref)                                                                                                           override;
[[nodiscard]] Result                        submitRect(RectRef& ref)                                                                                                           override;
[[nodiscard]] ResultValue<SceneRef>         instanceScene(Vec2 x1, Vec2 x2, float depth = 0.0f)                                                                                override;
[[nodiscard]] ResultValue<VectorEngineRef>  instanceVectorEngine(SceneRef& ref)                                                                                                override;
[[nodiscard]] Result                        updateScene(SceneRef& ref, Vec2 x1, Vec2 x2, float depth = 0.0f)                                                                   override;
[[nodiscard]] ResultValue<EdgeRef>          submitEdgeData(VectorEngineRef& ref, std::vector<Edge>& edges)                                                                     override;
[[nodiscard]] ResultValue<TriangleRef>      submitTriangleData(SceneRef& ref, std::vector<Vertex>& vertices, std::vector<uint16_t>& indices, int flags, int textureNumber = 0) override;
[[nodiscard]] Result                        deleteTriangles(SceneRef& sceneRef, TriangleRef& triangleRef)                                                                      override;
[[nodiscard]] Result                        submitSceneData(SceneRef& sceneRef)                                                                                                override;
              void                          updateSceneCameraPosition(SceneRef& ref, Vec3& position)                                                                           override;
              void                          updateSceneCameraView(SceneRef& ref, Mat4& view)                                                                                   override;
              void                          updateSceneCameraProjection(SceneRef& ref, Mat4& projection)                                                                       override;
              ModelRef                      createModel(SceneRef& ref, Mat4& model)                                                                                            override;
[[nodiscard]] ResultValue<TextRef>          drawText(Vec2 x1, std::string& text, Vec3 color, int fontSize, float depth = 0.0f)                                                 override;
[[nodiscard]] Result                        updateText(TextRef& ref, Vec2 x1, Vec3 color, int fontSize, float depth = 0.0f)                                                    override;
[[nodiscard]] Result                        loadFont(const char* path)                                                                                                         override;
[[nodiscard]] ResultValue<TextureRef>       createContourImage(std::vector<Contour>& contours)                                                                                 override;
[[nodiscard]] Result                        submitBuffers()                                                                                                                    override;
              void                          clearBuffers()                                                                                                                     override;

[[nodiscard]] Result                        resizeEvent()                                                                                                                      override;

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
            vk::RenderPassBeginInfo             vk_renderpassBeginInfo;
            vk::Viewport                        vk_viewport;
            vk::DescriptorPool                  vk_descriptorPool;
            vk::Rect2D                          vk_scissor;
            vk::PipelineStageFlags              vk_waitStages;
            vk::Image                           vk_depthImage;
            vk::DeviceMemory                    vk_depthImageMemory;
            vk::ImageView                       vk_depthImageView;
            vk::Buffer                          vk_vertexBuffer;
            vk::DeviceMemory                    vk_vertexBufferMemory;
            vk::Buffer                          vk_indexBuffer;
            vk::DeviceMemory                    vk_indexBufferMemory;
            vk::DescriptorSet                   vk_descriptorSet;
            vk::Extent2D                        vk_msdfExtent;
            vk::Image                           vk_textGlyphImages;
            vk::DeviceMemory                    vk_textGlyphImageMemory;
            vk::ImageView                       vk_textGlyphImageView;
            vk::Sampler                         vk_textGlyphSampler;
            vk::Image                           vk_scenesImageArrayImages;
            vk::DeviceMemory                    vk_scenesImageArrayImageMemory;
            vk::ImageView                       vk_scenesImageArrayImageView;
            vk::Sampler                         vk_scenesImageArraySampler;
            vk::Image                           vk_textureArrayImages;
            vk::DeviceMemory                    vk_textureArrayImageMemory;
            vk::ImageView                       vk_textureArrayImageView;
            vk::Sampler                         vk_textureArraySampler;
            std::array<vk::ClearValue, 2>       vk_clearValues;
            std::array<vk::ClearValue, 2>       vk_sceneClearValues;
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

            StagingBuffer                       vk_textureArrayStagingBuffer;
            StagingBuffer                       vk_rectStagingBuffer;
            StagingBuffer                       vk_vertexStagingBuffer;
            StagingBuffer                       vk_indexStagingBuffer;

            // runtime variables
            vk::CommandBufferBeginInfo          vk_beginInfo;
            vk::SubmitInfo                      vk_submitInfo;
            vk::DeviceSize                      vk_vertexBufferOffsets = 0;
            vk::DeviceSize                      vk_stagingBufferRegion;
            vk::PresentInfoKHR                  vk_presentInfo;
            vk::Result                          vk_runtimeResult;
            unsigned int                        currentFrame     =  0;
            unsigned int                        knownIndicesSize =  0;
            uint32_t                            imageIndex       = -1;
            LinkList<VertexBufferObject>        vertices;
            LinkList<uint16_t>                  indices;
            std::vector<std::vector<Contour>>   vectorImages;
            std::unordered_set<char>            charSet;
            bool                                markVerticesForUpdate = false;

            // debug
            bool                                vk_validation = false;

            // scene variables
            vk::RenderPass                      vk_sceneRenderpass;
            vk::DescriptorSetLayout             vk_sceneDescriptorSetLayout;
            vk::PipelineLayout                  vk_scenePipelineLayout;
            vk::ShaderModule                    vk_sceneVertexModule;
            vk::ShaderModule                    vk_sceneFragmentModule;
            std::vector<SceneObject>            scenes;

            // vector engine variables
            vk::DescriptorSetLayout             vk_vectorEngineDescriptorSetLayout;
            vk::PipelineLayout                  vk_vectorEnginePipelineLayout;
            vk::ShaderModule                    vk_vectorEngineTransformModule;
            vk::ShaderModule                    vk_vectorEngineRenderModule;
            std::vector<VectorEngineObject>     vectorEngineInstances;

            FairMutex queueMutex;
            FairMutex commandPoolMutex;
            FairMutex frameMutex;

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
[[nodiscard]] inline ResultValue<vk::Format> findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features);
              inline Result                  submitTriangles(SceneRef& ref);

[[nodiscard]] inline Result InstanceStagingBuffer(StagingBuffer& stagingBuffer, vk::DeviceSize size);

              inline void imageBarrier(
                  vk::Image& image, 
                  vk::ImageLayout oldLayout, 
                  vk::ImageLayout newLayout, 
                  vk::PipelineStageFlags src, 
                  vk::PipelineStageFlagBits dst, 
                  uint32_t layerCount = 1,
                  vk::AccessFlags srcAccess = {},
                  vk::AccessFlags dstAccess = {}
              );

              inline void   drawScene(SceneObject& scene, unsigned int index);
              inline void   renderScene(SceneObject& scene);
              inline void   renderInterface();
              inline void   renderVectorEngineInstance(VectorEngineObject& vEngine, SceneObject& scene);
[[nodiscard]] inline Result presentFrame();
[[nodiscard]] inline Result waitFences();

[[nodiscard]] inline Result instanceVulkan();
[[nodiscard]] inline Result createPhysicalDevice();
[[nodiscard]] inline Result getQueueFamilies();
[[nodiscard]] inline Result createDevice();
              inline void   getQueues();
[[nodiscard]] inline Result createSurface(SplitGui::Window& window);
              inline void   createSwapchain();
              inline void   createImageViews();
[[nodiscard]] inline Result createRenderpass();
              inline void   createDescriptorSetLayout();
              inline void   createGraphicsPipelineLayout();
[[nodiscard]] inline Result createGraphicsPipeline();
              inline void   createCommandPool();
[[nodiscard]] inline Result createDepthResources();
              inline void   createFramebuffers();
              inline void   createCommandBuffers();
              inline void   createSyncObj();
              inline void   createDescriptorPool();
              inline void   createDescriptorSet();
[[nodiscard]] inline Result createTextGlyphImage();
[[nodiscard]] inline Result createScenesImageArray();
[[nodiscard]] inline Result createTextureArray();
              inline void   updateDescriptorSets();

[[nodiscard]] inline Result createSceneRenderpass();
              inline void   createSceneDescriptorSetLayout();
              inline void   createScenePipelineLayout();
[[nodiscard]] inline Result createScenePipelineModules();
              
              inline void   createSceneDescriptorPool(SceneObject& scene);
[[nodiscard]] inline Result createScenePipeline(SceneObject& scene);
[[nodiscard]] inline Result createSceneDepthResources(SceneObject& scene);
[[nodiscard]] inline Result createSceneOutputResources(SceneObject& scene);
              inline void   createSceneFramebuffers(SceneObject& scene);
              inline void   createSceneDescriptorSet(SceneObject& scene);
[[nodiscard]] inline Result createSceneDataUniform(SceneObject& scene);
[[nodiscard]] inline Result createSceneModelUniform(SceneObject& scene);
              inline void   updateSceneDescriptorSet(SceneObject& scene);
              
              inline void   createVectorEngineDescriptorSetLayout();
              inline void   createVectorEnginePipelineLayout();
[[nodiscard]] inline Result createVectorEnginePipelineModules();

              inline void   createVectorEngineDescriptorPool(VectorEngineObject& vEngine);
              inline void   createVectorEngineDescriptorSet(VectorEngineObject& vEngine);
              inline void   updateVectorEngineDescriptorSet(VectorEngineObject& vEngine);
              inline void   updateVectorEngineEdges(VectorEngineObject& vEngine);
[[nodiscard]] inline Result createVectorEngineTransformPipeline(VectorEngineObject& vEngine);
[[nodiscard]] inline Result createVectorEngineRenderPipeline(VectorEngineObject& vEngine);
[[nodiscard]] inline Result submitVectorEngineEdgeResources(VectorEngineObject& vEngine);

              inline void setupRenderpassBeginInfo();
              inline void setupViewport();
              inline void setupScissor();
              inline void setupSubmitInfo();
              inline void setupPresentInfo();

              inline void cleanupFrameBuffers();
              inline void cleanupDepthResources();
              inline void cleanupSyncObj();
              inline void cleanupImageViews();
              inline void cleanupVertexAndIndexBuffers();
              inline void cleanupScenesImageArray();
              inline void cleanupScenes();
              inline void cleanupVectorEngineInstances();

              inline void cleanupStagingBuffer(StagingBuffer& stagingBuffer);

              inline void cleanupSceneVertexAndIndexBuffers(SceneRef& ref);
              inline void cleanupSceneFrameBuffers(SceneRef& ref);
              inline void cleanupSceneOutputImages(SceneRef& ref);
              inline void cleanupSceneDepthImages(SceneRef& ref);

              inline void cleanupVectorEngineEdgeResources(VectorEngineObject& vEngine);

[[nodiscard]] Result recreateSwapchain();
    };
}

#endif