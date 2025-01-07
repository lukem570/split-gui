#include <splitgui/lib.h>
#include <splitgui/graphics.hpp>
#include <splitgui/window.hpp>
#include <splitgui/structs.hpp>
#include <vector>
#include <tuple>
#include <string>
#include <array>
#include <stack>
#include "../../path.cpp"

// should prolly make this accessible to the user
#define MAX_FRAMES_IN_FLIGHT 2

std::vector<const char*> instanceExtensions = {
    VK_KHR_SURFACE_EXTENSION_NAME,  
    VK_KHR_WM_SURFACE_EXTENSION_NAME,
};

namespace SplitGui {
    class VulkanInterface : GraphicsLibInterface {
        public:

#pragma region Constructor

            VulkanInterface(bool validation = false) {
                vk_validation = validation;
                vk_clearColor.color = vk::ClearColorValue{0.0f, 0.0f, 0.0f, 1.0f};
            }

#pragma region Cleanup

            ~VulkanInterface() {
                //vk_device.freeDescriptorSets(vk_descriptorPool, 1, &vk_descriptorSet);
                //vk_device.destroyDescriptorPool(vk_descriptorPool);

                cleanupSyncObj();
                
                vk_device.freeCommandBuffers(vk_commandPool, vk_commandBuffers.size(), vk_commandBuffers.data());

                cleanupVertexAndIndexBuffers();

                vk_device.destroyCommandPool(vk_commandPool);
                
                cleanupFrameBuffers();

                vk_device.destroyPipeline(vk_graphicsPipeline);
                vk_device.destroyPipelineLayout(vk_graphicsPipelineLayout);
                //vk_device.destroyDescriptorSetLayout(vk_descriptorSetLayout);
                vk_device.destroyRenderPass(vk_renderpass);
                
                cleanupImageViews();

                vk_device.destroySwapchainKHR(vk_swapchain);
                vk_device.destroy();

                vk_instance.destroySurfaceKHR(vk_surface);
                vk_instance.destroy();
            }

#pragma region Api instance

            void instance() override {
                instanceVulkan();
                createPhysicalDevice();
            }

            void submitWindow(SplitGui::Window& window) override { // need to add physical device surface support
                pWindow = &window;
                createSurface(window);
                getQueueFamilies();
                createDevice();
                getQueues();
                createSwapchain();
                createImageViews();
                createRenderpass();
                //createDescriptorSetLayout();
                createGraphicsPipelineLayout();
                createGraphicsPipeline();
                createFramebuffers();
                createCommandPool();
                createVertexBuffer();
                createIndexBuffer();
                createCommandBuffers();
                createSyncObj();
                //createDescriptorPool();
                //createDescriptorSet();

                setupRenderpassBeginInfo();
                setupViewport();
                setupScissor();
                setupSubmitInfo();
                setupPresentInfo();
            }

#pragma region Draw frame

            void drawFrame() override {
                vk_runtimeResult = vk_device.waitForFences(1, &vk_inFlightFences[currentFrame], vk::True, UINT64_MAX);

                if (vk_runtimeResult != vk::Result::eSuccess) {
                    printf("Error Waiting for fences\n");
                    throw;
                }

                vk::ResultValue<uint32_t> result = vk_device.acquireNextImageKHR(vk_swapchain, UINT64_MAX, vk_imageAvailableSemaphores[currentFrame], nullptr);

                if (result.result != vk::Result::eSuccess) {
                    if (result.result == vk::Result::eErrorOutOfDateKHR || vk_runtimeResult == vk::Result::eSuboptimalKHR) {
                        recreateSwapchain();
                        return;
                    }
                
                    printf("Error getting next swapchain image\n");
                    throw;
                }

                vk_runtimeResult = vk_device.resetFences(1, &vk_inFlightFences[currentFrame]);

                if (vk_runtimeResult != vk::Result::eSuccess) {
                    printf("Error resetting fences\n");
                    throw;
                }

                imageIndex = result.value;

                vk_commandBuffers[currentFrame].reset();
                vk_runtimeResult = vk_commandBuffers[currentFrame].begin(vk_beginInfo);

                if (vk_runtimeResult != vk::Result::eSuccess) {
                    printf("Error beginning command buffer\n");
                    throw;
                }

                vk_renderpassBeginInfo.framebuffer = vk_swapchainFramebuffers[imageIndex];
                
                vk_commandBuffers[currentFrame].beginRenderPass(vk_renderpassBeginInfo, vk::SubpassContents::eInline);

                vk_commandBuffers[currentFrame].bindPipeline(vk::PipelineBindPoint::eGraphics, vk_graphicsPipeline);
                vk_commandBuffers[currentFrame].bindVertexBuffers(0, 1, &vk_vertexBuffer, &vk_vertexBufferOffsets);
                vk_commandBuffers[currentFrame].bindIndexBuffer(vk_indexBuffer, 0, vk::IndexType::eUint16);
                //vk_commandBuffers[currentFrame].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, vk_graphicsPipelineLayout, 0, 1, &vk_descriptorSet, 0, nullptr);

                vk_commandBuffers[currentFrame].setViewport(0, 1, &vk_viewport);
                vk_commandBuffers[currentFrame].setScissor(0, 1, &vk_scissor);

                vk_commandBuffers[currentFrame].drawIndexed(indices.size(), 1, 0, 0, 0);

                vk_commandBuffers[currentFrame].endRenderPass();

                vk_runtimeResult = vk_commandBuffers[currentFrame].end();

                if (vk_runtimeResult != vk::Result::eSuccess) {
                    printf("Error ending command buffer\n");
                    throw;
                }

                vk_submitInfo.pWaitSemaphores   = &vk_imageAvailableSemaphores[currentFrame];
                vk_submitInfo.pCommandBuffers   = &vk_commandBuffers[currentFrame];
                vk_submitInfo.pSignalSemaphores = &vk_renderFinishedSemaphores[currentFrame];

                vk_runtimeResult = vk_graphicsQueue.submit(1, &vk_submitInfo, vk_inFlightFences[currentFrame]);

                if (vk_runtimeResult != vk::Result::eSuccess) {
                    printf("Error sending command to gpu\n");
                    throw;
                }

                vk_presentInfo.pWaitSemaphores = &vk_renderFinishedSemaphores[currentFrame];
                vk_presentInfo.pImageIndices   = &imageIndex;

                vk_runtimeResult = vk_presentQueue.presentKHR(&vk_presentInfo);

                if (vk_runtimeResult != vk::Result::eSuccess) {
                    if (vk_runtimeResult == vk::Result::eErrorOutOfDateKHR || vk_runtimeResult == vk::Result::eSuboptimalKHR) {
                        recreateSwapchain();
                        return;
                    }
                
                    printf("Error getting next swapchain image\n");
                    throw;
                }

                currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
            }
    
#pragma region Draw rect

            void drawRect(Vec2 x1, Vec2 x2, Vec3 color) override {

                int verticesOffset = vertices.size();

                indices.push_back(verticesOffset + 0); //0, 1, 2, 2, 3, 0
                indices.push_back(verticesOffset + 2);
                indices.push_back(verticesOffset + 1);
                indices.push_back(verticesOffset + 3);
                indices.push_back(verticesOffset + 2);
                indices.push_back(verticesOffset + 0);

                
                Vertex vertOne;
                vertOne.pos   = x1;
                vertOne.color = color;

                Vertex vertTwo;
                vertTwo.pos   = {x1.x, x2.y};
                vertTwo.color = color;

                Vertex vertThree;
                vertThree.pos   = {x2.x, x1.y};
                vertThree.color = color;

                Vertex vertFour;
                vertFour.pos   = x2;
                vertFour.color = color;

                vertices.push_back(vertOne);
                vertices.push_back(vertTwo);
                vertices.push_back(vertThree);
                vertices.push_back(vertFour);
                

                // this could be merged with 'createVertexBuffer' and 'createIndexBuffer' but I am too lazy to care

                vk::DeviceSize   indexBufferSize;
                vk::Buffer       stagingIndexBuffer;
                vk::DeviceMemory stagingIndexBufferMemory;

                vk::DeviceSize   vertexBufferSize;
                vk::Buffer       stagingVertexBuffer;
                vk::DeviceMemory stagingVertexBufferMemory;

                InstanceStagingBuffer(indices,  stagingIndexBuffer,  stagingIndexBufferMemory,  indexBufferSize );
                InstanceStagingBuffer(vertices, stagingVertexBuffer, stagingVertexBufferMemory, vertexBufferSize);

                vk::Buffer       tempIndexBuffer;
                vk::DeviceMemory tempIndexBufferMemory;

                vk::Buffer       tempVertexBuffer;
                vk::DeviceMemory tempVertexBufferMemory;

                printf("new: %d, %d\n", vertexBufferSize / sizeof(SplitGui::Vertex), indexBufferSize / sizeof(u_int16_t));

                createBuffer(
                    indexBufferSize, 
                    vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, 
                    vk::MemoryPropertyFlagBits::eDeviceLocal,
                    tempIndexBuffer,
                    tempIndexBufferMemory
                );

                createBuffer(
                    vertexBufferSize, 
                    vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, 
                    vk::MemoryPropertyFlagBits::eDeviceLocal,
                    tempVertexBuffer,
                    tempVertexBufferMemory
                );

                vk::CommandBuffer commandBuffer = startCopyBuffer();
                vk::BufferCopy copyRegionIndex;
                vk::BufferCopy copyRegionVertex;

                copyBuffer(stagingIndexBuffer,  tempIndexBuffer, indexBufferSize,  commandBuffer, copyRegionIndex);
                copyBuffer(stagingVertexBuffer, tempVertexBuffer, vertexBufferSize, commandBuffer, copyRegionVertex);

                endCopyBuffer(commandBuffer);

                vk_runtimeResult = vk_device.waitForFences(vk_inFlightFences.size(), vk_inFlightFences.data(), true, UINT64_MAX);

                if (vk_runtimeResult != vk::Result::eSuccess) {
                    printf("Error Waiting for fences\n");
                    throw;
                }
                
                cleanupVertexAndIndexBuffers();

                vk_vertexBuffer       = tempVertexBuffer;
                vk_vertexBufferMemory = tempVertexBufferMemory;

                vk_indexBuffer        = tempIndexBuffer;
                vk_indexBufferMemory  = tempIndexBufferMemory;

                vk_device.destroyBuffer(stagingVertexBuffer);
                vk_device.freeMemory(stagingVertexBufferMemory);

                vk_device.destroyBuffer(stagingIndexBuffer);
                vk_device.freeMemory(stagingIndexBufferMemory);
            }

#pragma region Variables

        protected:
            SplitGui::Window*               pWindow;
        private:
            vk::Instance                    vk_instance;
            vk::PhysicalDevice              vk_physicalDevice;
            vk::Device                      vk_device;
            vk::Queue                       vk_graphicsQueue;
            vk::Queue                       vk_presentQueue;
            vk::SurfaceKHR                  vk_surface;
            vk::SwapchainKHR                vk_swapchain;
            vk::SurfaceFormatKHR            vk_surfaceFormat;
            vk::PresentModeKHR              vk_presentMode;
            vk::Extent2D                    vk_swapchainExtent;
            vk::RenderPass                  vk_renderpass;
            vk::DescriptorSetLayout         vk_descriptorSetLayout;
            vk::PipelineLayout              vk_graphicsPipelineLayout;
            vk::Pipeline                    vk_graphicsPipeline;
            vk::PipelineLayout              vk_computePipelineLayout;
            vk::Pipeline                    vk_computePipeline;
            vk::CommandPool                 vk_commandPool;
            vk::ClearValue                  vk_clearColor;
            vk::RenderPassBeginInfo         vk_renderpassBeginInfo;
            vk::Viewport                    vk_viewport;
            vk::DescriptorPool              vk_descriptorPool;
            vk::DescriptorSet               vk_descriptorSet;
            vk::Rect2D                      vk_scissor;
            vk::PipelineStageFlags          vk_waitStages;
            vk::Buffer                      vk_vertexBuffer;
            vk::DeviceMemory                vk_vertexBufferMemory;
            vk::Buffer                      vk_indexBuffer;
            vk::DeviceMemory                vk_indexBufferMemory;
            std::vector<vk::CommandBuffer>  vk_commandBuffers;
            std::vector<vk::Framebuffer>    vk_swapchainFramebuffers;
            std::vector<vk::Image>          vk_swapchainImages;
            std::vector<vk::ImageView>      vk_swapchainImageViews;
            std::vector<vk::Semaphore>      vk_imageAvailableSemaphores;
            std::vector<vk::Semaphore>      vk_renderFinishedSemaphores;
            std::vector<vk::Fence>          vk_inFlightFences;
            bool                            vk_validation;
            unsigned int                    graphicsQueueFamilyIndex = -1;
            unsigned int                    presentQueueFamilyIndex = -1;
            std::vector<const char *>       enabled_layers;
            std::vector<const char *>       enabled_instance_extensions;
            std::vector<const char *>       enabled_device_extensions;

            // runtime variables (here to prevent excess push and pop operations)
            vk::CommandBufferBeginInfo      vk_beginInfo;
            vk::SubmitInfo                  vk_submitInfo;
            vk::DeviceSize                  vk_vertexBufferOffsets = 0;
            vk::DeviceSize                  vk_stagingBufferRegion;
            vk::PresentInfoKHR              vk_presentInfo;
            vk::Result                      vk_runtimeResult;
            unsigned int                    currentFrame = 0;
            uint32_t                        imageIndex = -1;
            std::vector<Vertex>             vertices= {
                {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                {{ 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
                {{ 0.0f,  0.5f}, {0.0f, 0.0f, 1.0f}},
            };
            std::vector<uint16_t>           indices = {
                0, 1, 2,
            };

            vk::Bool32 check_layers(const std::vector<const char *> &check_names, const std::vector<vk::LayerProperties> &layers) {
                for (const auto &name : check_names) {
                    vk::Bool32 found = VK_FALSE;
                    for (const auto &layer : layers) {
                        if (!strcmp(name, layer.layerName)) {
                            found = VK_TRUE;
                            break;
                        }
                    }
                    if (!found) {
                        fprintf(stderr, "Cannot find layer: %s\n", name);
                        return 0;
                    }
                }
                return VK_TRUE;
            }

#pragma region instance

            void instanceVulkan() {
                VkResult err = volkInitialize();
                if (err != VK_SUCCESS) {
                    printf("Unable to find the Vulkan runtime on the system\n");
                    throw;
                }

                VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

                std::vector<const char *> instanceLayers = {"VK_LAYER_KHRONOS_validation"};

                vk::Bool32 validation_found = VK_FALSE;
                if (vk_validation) {
                    vk::ResultValue<std::vector<vk::LayerProperties>> layers = vk::enumerateInstanceLayerProperties();
                    

                    validation_found = check_layers(instanceLayers, layers.value);
                    if (validation_found) {
                        enabled_layers.push_back("VK_LAYER_KHRONOS_validation");
                    } else {
                        printf("Error: Could not find validation layers\n");
                        throw;
                    }
                }

                auto instance_extensions_return = vk::enumerateInstanceExtensionProperties();

                bool surfaceExtFound = false;
                bool platformSurfaceExtFound = false;
                bool portabilityEnumerationActive = false;

                for (const auto &extension : instance_extensions_return.value) {
                    if (!strcmp(VK_KHR_SURFACE_EXTENSION_NAME, extension.extensionName)) {
                        surfaceExtFound = true;
                        enabled_instance_extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
                    } else if (!strcmp(VK_KHR_WM_SURFACE_EXTENSION_NAME, extension.extensionName)) {
                        platformSurfaceExtFound = true;
                        enabled_instance_extensions.push_back(VK_KHR_WM_SURFACE_EXTENSION_NAME);
                    }
                }

                if (!surfaceExtFound) {
                    printf( "Error: failed to find: " VK_KHR_SURFACE_EXTENSION_NAME "\n");
                    throw;
                }

                if (!platformSurfaceExtFound) {
                    printf( "Error: failed to find: " VK_KHR_WM_SURFACE_EXTENSION_NAME "\n");
                    throw;
                }

                vk::ApplicationInfo appInfo;
                appInfo.pApplicationName   = "Split-gui";
                appInfo.applicationVersion = 0;
                appInfo.pEngineName        = "Split-gui";
                appInfo.engineVersion      = 0;
                appInfo.apiVersion         = vk::ApiVersion10;

                vk::InstanceCreateInfo createInfo;
                createInfo.pApplicationInfo = &appInfo;

                if (vk_validation) {
                    createInfo.enabledLayerCount   = enabled_layers.size();
                    createInfo.ppEnabledLayerNames = enabled_layers.data();
                }

                createInfo.setEnabledExtensionCount  (static_cast<uint32_t>(enabled_instance_extensions.size()));
                createInfo.setPpEnabledExtensionNames(                      enabled_instance_extensions.data());

                vk::ResultValue<vk::Instance> result = vk::createInstance(createInfo);

                if (result.result == vk::Result::eErrorIncompatibleDriver) {
                    printf("Error: incompatable driver\n");
                    throw;
                } else if (result.result == vk::Result::eErrorExtensionNotPresent) {
                    printf("Error: could not find vulkan extention\n");
                    throw;
                } else if (result.result != vk::Result::eSuccess) {
                    printf("Error: could not instance vulkan\n");
                    throw;
                }

                vk_instance = result.value;

                VULKAN_HPP_DEFAULT_DISPATCHER.init(vk_instance);
            }

#pragma region Physical device

            // Device selction could be more robust but this will do for now
            void createPhysicalDevice() { 
                vk::ResultValue<std::vector<vk::PhysicalDevice>> physicalDevices = vk_instance.enumeratePhysicalDevices();

                if (physicalDevices.result != vk::Result::eSuccess) {
                    printf("Error enumerating physical devices\n");
                    throw;
                }

                int selection = -1;
                
                for (int i = 0; i < physicalDevices.value.size(); i++) {
                    if (physicalDevices.value[i].getProperties().apiVersion < vk::ApiVersion12) {
                        continue;
                    }

                    selection = i;
                    break;
                }

                if (selection == -1) {
                    printf("ERROR: No suitable devices found for vulkan\n");
                    throw;
                }

                vk_physicalDevice = physicalDevices.value[selection];

                printf("Using %s\n", vk_physicalDevice.getProperties().deviceName.data());
            }

#pragma region Queue families

            void getQueueFamilies() {
                std::vector<vk::QueueFamilyProperties> queueFamilies = vk_physicalDevice.getQueueFamilyProperties();

                for (int i = 0; i < queueFamilies.size(); i++) {

                    vk::ResultValue<vk::Bool32> result = vk_physicalDevice.getSurfaceSupportKHR(i, vk_surface);
                    
                    if(result.result != vk::Result::eSuccess) {
                        printf("ERROR: error getting surface support\n");
                        throw;
                    }

                    if (queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics && graphicsQueueFamilyIndex == -1){
                        graphicsQueueFamilyIndex = i;
                    }

                    if (result.value && presentQueueFamilyIndex == -1) {
                        presentQueueFamilyIndex = i;
                    }
                    
                    if (graphicsQueueFamilyIndex != -1 && presentQueueFamilyIndex != -1) {
                        break;
                    }
                }

                if (graphicsQueueFamilyIndex == -1) {
                    printf("Graphics queue family not found\n");
                    throw;
                }

                if (presentQueueFamilyIndex == -1) {
                    printf("Present queue family not found\n");
                    throw;
                }
            }

#pragma region Device

            // or logical device
            void createDevice() {
                vk::DeviceQueueCreateInfo queueCreateInfo;
                queueCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
                queueCreateInfo.queueCount       = 1;
                float queuePriority              = 1.0;
                queueCreateInfo.pQueuePriorities = &queuePriority;

                vk::PhysicalDeviceFeatures deviceFeatures;
                deviceFeatures = vk_physicalDevice.getFeatures();

                bool swapchainExtFound = false;

                auto device_extension_return = vk_physicalDevice.enumerateDeviceExtensionProperties();

                for (const auto &extension : device_extension_return.value) {
                    if (!strcmp(VK_KHR_SWAPCHAIN_EXTENSION_NAME, extension.extensionName)) {
                        swapchainExtFound = true;
                        enabled_device_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
                    } else if (!strcmp("VK_KHR_portability_subset", extension.extensionName)) {
                        enabled_device_extensions.push_back("VK_KHR_portability_subset");
                    }
                }

                if (!swapchainExtFound) {
                    printf("Error: could not find swapchain extension\n");
                    throw;
                }

                vk::DeviceCreateInfo deviceCreateInfo;
                deviceCreateInfo.queueCreateInfoCount    = 1;
                deviceCreateInfo.pQueueCreateInfos       = &queueCreateInfo;
                deviceCreateInfo.pEnabledFeatures        = &deviceFeatures;
                deviceCreateInfo.enabledExtensionCount   = enabled_device_extensions.size();
                deviceCreateInfo.ppEnabledExtensionNames = enabled_device_extensions.data();

                vk::ResultValue<vk::Device> result = vk_physicalDevice.createDevice(deviceCreateInfo);

                if (result.result != vk::Result::eSuccess) {
                    printf("Error creating logical device\n");
                    throw;
                }

                vk_device = result.value;

                VULKAN_HPP_DEFAULT_DISPATCHER.init(vk_device);
            }

#pragma region Queues

            void getQueues() {
                vk_graphicsQueue = vk_device.getQueue(graphicsQueueFamilyIndex, 0);
                vk_presentQueue  = vk_device.getQueue(presentQueueFamilyIndex , 0);
            }

#pragma region Surface

            void createSurface(SplitGui::Window& window) {
                vk_surface = window.createSurface(vk_instance);
            }

#pragma region Swapchain

            inline vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) {
                if (capabilities.currentExtent.width != UINT32_MAX) {
                    return capabilities.currentExtent;
                } else {
                    vk::Extent2D actualExtent = {(uint32_t)pWindow->getSize().x, (uint32_t)pWindow->getSize().y};
                    actualExtent.width  = (((capabilities.minImageExtent.width) > ((((capabilities.maxImageExtent.width) < (actualExtent.width)) ? (capabilities.maxImageExtent.width) : (actualExtent.width)))) ? (capabilities.minImageExtent.width) : ((((capabilities.maxImageExtent.width) < (actualExtent.width)) ? (capabilities.maxImageExtent.width) : (actualExtent.width))));
                    actualExtent.height = (((capabilities.minImageExtent.height) > ((((capabilities.maxImageExtent.height) < (actualExtent.height)) ? (capabilities.maxImageExtent.height) : (actualExtent.height)))) ? (capabilities.minImageExtent.height) : ((((capabilities.maxImageExtent.height) < (actualExtent.height)) ? (capabilities.maxImageExtent.height) : (actualExtent.height))));
                    return actualExtent;
                }
            }

            inline vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) {
                for (const auto& availableFormat : availableFormats) {
                    if (availableFormat.format == vk::Format::eB8G8R8A8Srgb && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                        return availableFormat;
                    }
                }

                return availableFormats[0];
            }

            inline vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes) {
                for (const auto& availablePresentMode : availablePresentModes) {
                    if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
                        return availablePresentMode;
                    }
                }

                return vk::PresentModeKHR::eFifo;
            }

            void createSwapchain() {
                vk::ResultValue<std::vector<vk::SurfaceFormatKHR>> formats      = vk_physicalDevice.getSurfaceFormatsKHR(vk_surface);
                vk::ResultValue<vk::SurfaceCapabilitiesKHR>        capabilities = vk_physicalDevice.getSurfaceCapabilitiesKHR(vk_surface);
                vk::ResultValue<std::vector<vk::PresentModeKHR>>   presentModes = vk_physicalDevice.getSurfacePresentModesKHR(vk_surface);

                if (capabilities.result != vk::Result::eSuccess || 
                    formats.result      != vk::Result::eSuccess ||
                    presentModes.result != vk::Result::eSuccess) {

                    printf("Error getting physical device props\n");
                    throw;
                }

                vk_swapchainExtent = chooseSwapExtent(capabilities.value);
                vk_surfaceFormat   = chooseSwapSurfaceFormat(formats.value);
                vk_presentMode     = chooseSwapPresentMode(presentModes.value);

                vk::SwapchainCreateInfoKHR createInfo;
                createInfo.minImageCount         = capabilities.value.minImageCount + 1;
                createInfo.pQueueFamilyIndices   = &graphicsQueueFamilyIndex;
                createInfo.surface               = vk_surface;
                createInfo.imageFormat           = vk_surfaceFormat.format;
                createInfo.imageColorSpace       = vk_surfaceFormat.colorSpace;
                createInfo.imageExtent           = vk_swapchainExtent;
                createInfo.presentMode           = vk_presentMode;
                createInfo.imageArrayLayers      = 1;
                createInfo.queueFamilyIndexCount = 1;
                createInfo.imageSharingMode      = vk::SharingMode::eExclusive;
                createInfo.imageUsage            = vk::ImageUsageFlagBits::eColorAttachment;
                createInfo.clipped               = true;
                createInfo.oldSwapchain          = nullptr;

                vk::ResultValue<vk::SwapchainKHR>       result_swapchain       = vk_device.createSwapchainKHR(createInfo);
                vk::ResultValue<std::vector<vk::Image>> result_swapchainImages = vk_device.getSwapchainImagesKHR(result_swapchain.value);

                if (result_swapchain.result       != vk::Result::eSuccess || 
                    result_swapchainImages.result != vk::Result::eSuccess) {

                    printf("Error creating swapchain\n");
                    throw;
                }

                vk_swapchain       = result_swapchain.value;
                vk_swapchainImages = result_swapchainImages.value;
            }

#pragma region Image views

            void createImageViews() {
                vk_swapchainImageViews.resize(vk_swapchainImages.size());

                for (int i = 0; i < vk_swapchainImageViews.size(); i++){

                    vk::ImageViewCreateInfo createInfo;
                    createInfo.image                            = vk_swapchainImages[i];
                    createInfo.viewType                         = vk::ImageViewType::e2D;
                    createInfo.format                           = vk_surfaceFormat.format;
                    createInfo.components.r                     = vk::ComponentSwizzle::eIdentity;
                    createInfo.components.g                     = vk::ComponentSwizzle::eIdentity;
                    createInfo.components.b                     = vk::ComponentSwizzle::eIdentity;
                    createInfo.components.a                     = vk::ComponentSwizzle::eIdentity;
                    createInfo.subresourceRange.aspectMask      = vk::ImageAspectFlagBits::eColor;
                    createInfo.subresourceRange.baseMipLevel    = 0;
                    createInfo.subresourceRange.levelCount      = 1;
                    createInfo.subresourceRange.baseArrayLayer  = 0;
                    createInfo.subresourceRange.layerCount      = 1;

                    vk::ResultValue<vk::ImageView> result = vk_device.createImageView(createInfo);

                    if (result.result != vk::Result::eSuccess) {
                        printf("Error creating imageview\n");
                        throw;
                    }

                    vk_swapchainImageViews[i] = result.value;
                }
            }

#pragma region Renderpass

            void createRenderpass() {
                vk::AttachmentDescription colorAttachment;
                colorAttachment.format              = vk_surfaceFormat.format;
                colorAttachment.samples             = vk::SampleCountFlagBits::e1;
                colorAttachment.loadOp              = vk::AttachmentLoadOp::eClear;
                colorAttachment.storeOp             = vk::AttachmentStoreOp::eStore;
                colorAttachment.stencilLoadOp       = vk::AttachmentLoadOp::eDontCare;
                colorAttachment.stencilStoreOp      = vk::AttachmentStoreOp::eDontCare;
                colorAttachment.initialLayout       = vk::ImageLayout::eUndefined;
                colorAttachment.finalLayout         = vk::ImageLayout::ePresentSrcKHR;

                vk::AttachmentReference colorAttachmentReference;
                colorAttachmentReference.attachment = 0;
                colorAttachmentReference.layout     = vk::ImageLayout::eColorAttachmentOptimal;

                vk::SubpassDescription subpass;
                subpass.pipelineBindPoint           = vk::PipelineBindPoint::eGraphics;
                subpass.colorAttachmentCount        = 1;
                subpass.pColorAttachments           = &colorAttachmentReference;

                vk::SubpassDependency dependency;
                dependency.srcSubpass               = vk::SubpassExternal;
                dependency.dstSubpass               = 0;
                dependency.srcStageMask             = vk::PipelineStageFlagBits::eColorAttachmentOutput;
                dependency.srcAccessMask            = (vk::AccessFlags) 0;
                dependency.dstStageMask             = vk::PipelineStageFlagBits::eColorAttachmentOutput;
                dependency.dstAccessMask           |= vk::AccessFlagBits::eColorAttachmentRead;
                dependency.dstAccessMask           |= vk::AccessFlagBits::eColorAttachmentWrite;

                vk::RenderPassCreateInfo createInfo;
                createInfo.attachmentCount          = 1;
                createInfo.subpassCount             = 1;
                createInfo.dependencyCount          = 1;
                createInfo.pAttachments             = &colorAttachment;
                createInfo.pSubpasses               = &subpass;
                createInfo.pDependencies            = &dependency;

                vk::ResultValue<vk::RenderPass> result = vk_device.createRenderPass(createInfo);
                
                if (result.result != vk::Result::eSuccess) {
                    printf("Error creating renderpass\n");
                    throw;
                }

                vk_renderpass = result.value;
            }

#pragma region Descriptor Layout

            void createDescriptorSetLayout() {

                vk::DescriptorSetLayoutBinding samplerLayoutBinding;
                samplerLayoutBinding.binding            = 0;
                samplerLayoutBinding.descriptorType     = vk::DescriptorType::eCombinedImageSampler;
                samplerLayoutBinding.descriptorCount    = 1;
                samplerLayoutBinding.stageFlags         = vk::ShaderStageFlagBits::eFragment;
                samplerLayoutBinding.pImmutableSamplers = nullptr;

                std::array<vk::DescriptorSetLayoutBinding, 1> bindings = { samplerLayoutBinding };

                vk::DescriptorSetLayoutCreateInfo createInfo;
                createInfo.bindingCount = bindings.size();
                createInfo.pBindings    = bindings.data();
                
                vk::ResultValue<vk::DescriptorSetLayout> result = vk_device.createDescriptorSetLayout(createInfo);

                if (result.result != vk::Result::eSuccess) {
                    printf("Error: error creating descriptor set layout\n");
                    throw;
                }

                vk_descriptorSetLayout = result.value;
            }

#pragma region Graphics pipeline

            void createGraphicsPipelineLayout() {
                vk::PipelineLayoutCreateInfo createInfo;
                createInfo.setLayoutCount         = 0;
                createInfo.pSetLayouts            = nullptr;
                createInfo.pushConstantRangeCount = 0;

                vk::ResultValue<vk::PipelineLayout> result = vk_device.createPipelineLayout(createInfo);

                if (result.result != vk::Result::eSuccess) {
                    printf("Error: error creating graphics pipeline layout\n");
                    throw;
                } 

                vk_graphicsPipelineLayout = result.value;
            }

            inline vk::ShaderModule createShaderModule(const std::vector<char>& code) {
                vk::ShaderModuleCreateInfo createInfo;
                createInfo.codeSize = code.size();
                createInfo.pCode    = reinterpret_cast<const uint32_t*>(code.data());

                vk::ResultValue<vk::ShaderModule> result = vk_device.createShaderModule(createInfo);

                if (result.result != vk::Result::eSuccess) {
                    printf("Error creating shader module\n");
                    throw;
                }

                return result.value;
            }

            void createGraphicsPipeline() {

                const std::vector<char> vertexShaderFile   = readFile("../shaders/vertex.spv");
                const std::vector<char> geometryShaderFile = readFile("../shaders/geometry.spv");
                const std::vector<char> fragmentShaderFile = readFile("../shaders/fragment.spv");

                vk::ShaderModule vertexShader   = createShaderModule(vertexShaderFile);
                vk::ShaderModule geometryShader = createShaderModule(geometryShaderFile);
                vk::ShaderModule fragmentShader = createShaderModule(fragmentShaderFile);

                vk::PipelineShaderStageCreateInfo vertexCreateInfo;
                vertexCreateInfo.stage  = vk::ShaderStageFlagBits::eVertex;
                vertexCreateInfo.module = vertexShader;
                vertexCreateInfo.pName  = "main";

                vk::PipelineShaderStageCreateInfo geometryCreateInfo;
                geometryCreateInfo.stage  = vk::ShaderStageFlagBits::eGeometry;
                geometryCreateInfo.module = geometryShader;
                geometryCreateInfo.pName  = "main";

                vk::PipelineShaderStageCreateInfo fragmentCreateInfo;
                fragmentCreateInfo.stage  = vk::ShaderStageFlagBits::eFragment;
                fragmentCreateInfo.module = fragmentShader;
                fragmentCreateInfo.pName  = "main";

                std::array<vk::PipelineShaderStageCreateInfo, 3> shaderStages = {vertexCreateInfo, geometryCreateInfo, fragmentCreateInfo};

                std::vector<vk::DynamicState> dynamicStates = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};

                vk::PipelineDynamicStateCreateInfo dynamicState;
                dynamicState.dynamicStateCount = dynamicStates.size();
                dynamicState.pDynamicStates    = dynamicStates.data();

                vk::VertexInputBindingDescription bindingDescription;
                bindingDescription.binding   = 0;
                bindingDescription.stride    = sizeof(Vertex);
                bindingDescription.inputRate = vk::VertexInputRate::eVertex;
                
                std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions;
                attributeDescriptions[0].binding  = 0;
                attributeDescriptions[0].location = 0;
                attributeDescriptions[0].format   = vk::Format::eR32G32Sfloat;
                attributeDescriptions[0].offset   = offsetof(Vertex, Vertex::pos);

                attributeDescriptions[1].binding  = 0;
                attributeDescriptions[1].location = 1;
                attributeDescriptions[1].format   = vk::Format::eR32G32B32Sfloat;
                attributeDescriptions[1].offset   = offsetof(Vertex, Vertex::color);

                vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
                vertexInputInfo.vertexBindingDescriptionCount   = 1;
                vertexInputInfo.pVertexBindingDescriptions      = &bindingDescription;
                vertexInputInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
                vertexInputInfo.pVertexAttributeDescriptions    = attributeDescriptions.data();
                
                vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
                inputAssembly.topology               = vk::PrimitiveTopology::eTriangleList;
                inputAssembly.primitiveRestartEnable = vk::False;

                vk::PipelineViewportStateCreateInfo viewportState;
                viewportState.viewportCount = 1;
                viewportState.scissorCount  = 1;

                vk::PipelineRasterizationStateCreateInfo rasterizer;
                rasterizer.depthClampEnable        = vk::False;
                rasterizer.rasterizerDiscardEnable = vk::False;
                rasterizer.polygonMode             = vk::PolygonMode::eFill;
                rasterizer.lineWidth               = 1.0f;
                rasterizer.cullMode                = vk::CullModeFlagBits::eBack;
                rasterizer.frontFace               = vk::FrontFace::eClockwise;
                rasterizer.depthBiasEnable         = vk::False;

                vk::PipelineMultisampleStateCreateInfo multisampling;
                multisampling.sampleShadingEnable  = vk::False;
                multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;

                vk::PipelineColorBlendAttachmentState colorBlendAttachment;
                colorBlendAttachment.colorWriteMask      = vk::ColorComponentFlagBits::eR;
                colorBlendAttachment.colorWriteMask     |= vk::ColorComponentFlagBits::eG;
                colorBlendAttachment.colorWriteMask     |= vk::ColorComponentFlagBits::eB;
                colorBlendAttachment.colorWriteMask     |= vk::ColorComponentFlagBits::eA;
                colorBlendAttachment.blendEnable         = vk::False;

                vk::PipelineColorBlendStateCreateInfo colorBlending;
                colorBlending.logicOpEnable   = vk::False;
                colorBlending.attachmentCount = 1;
                colorBlending.pAttachments    = &colorBlendAttachment;

                vk::GraphicsPipelineCreateInfo pipelineInfo;
                pipelineInfo.stageCount          = shaderStages.size();
                pipelineInfo.pStages             = shaderStages.data();
                pipelineInfo.pVertexInputState   = &vertexInputInfo;
                pipelineInfo.pInputAssemblyState = &inputAssembly;
                pipelineInfo.pViewportState      = &viewportState;
                pipelineInfo.pRasterizationState = &rasterizer;
                pipelineInfo.pMultisampleState   = &multisampling;
                pipelineInfo.pColorBlendState    = &colorBlending;
                pipelineInfo.pDynamicState       = &dynamicState;
                pipelineInfo.layout              = vk_graphicsPipelineLayout;
                pipelineInfo.renderPass          = vk_renderpass;
                pipelineInfo.subpass             = 0;
                pipelineInfo.basePipelineHandle  = nullptr;
                pipelineInfo.basePipelineIndex   = -1;

                vk::ResultValue<vk::Pipeline> result = vk_device.createGraphicsPipeline(nullptr, pipelineInfo);

                if (result.result != vk::Result::eSuccess) {
                    printf("Error: error creating graphics pipeline\n");
                    throw;
                } 

                vk_device.destroyShaderModule(fragmentShader);
                vk_device.destroyShaderModule(geometryShader);
                vk_device.destroyShaderModule(vertexShader);

                vk_graphicsPipeline = result.value;
            }

#pragma region Buffers

            void createFramebuffers() {
                vk_swapchainFramebuffers.resize(vk_swapchainImageViews.size());

                for (int i = 0; i < vk_swapchainImageViews.size(); i++) {
                    vk::FramebufferCreateInfo framebufferInfo;
                    framebufferInfo.renderPass      = vk_renderpass;
                    framebufferInfo.attachmentCount = 1;
                    framebufferInfo.pAttachments    = &vk_swapchainImageViews[i];
                    framebufferInfo.width           = vk_swapchainExtent.width;
                    framebufferInfo.height          = vk_swapchainExtent.height;
                    framebufferInfo.layers          = 1;

                    vk::ResultValue<vk::Framebuffer> result = vk_device.createFramebuffer(framebufferInfo);
                    
                    if (result.result != vk::Result::eSuccess) {
                        printf("Error: error creating frame buffers\n");
                        throw;
                    }

                    vk_swapchainFramebuffers[i] = result.value;
                }
            }

            void createCommandPool() {
                vk::CommandPoolCreateInfo commandPoolInfo;
                commandPoolInfo.flags            = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
                commandPoolInfo.queueFamilyIndex = graphicsQueueFamilyIndex;

                vk::ResultValue<vk::CommandPool> result = vk_device.createCommandPool(commandPoolInfo);
                
                if (result.result != vk::Result::eSuccess) {
                    printf("Error: error creating command pool\n");
                    throw;
                } 
                
                vk_commandPool = result.value;
            }

            uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) {

                vk::PhysicalDeviceMemoryProperties memProperties = vk_physicalDevice.getMemoryProperties();

                for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
                    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                        return i;
                    }
                }

                printf("failed to find suitable memory type");
                throw;
            }

            void createBuffer(
                vk::DeviceSize             size,
                vk::BufferUsageFlags       usage,
                vk::MemoryPropertyFlags    properties,
                vk::Buffer&                out_buffer,
                vk::DeviceMemory&          out_bufferMemory
            ) {
                vk::BufferCreateInfo createInfo;
                createInfo.size        = size;
                createInfo.usage       = usage;
                createInfo.sharingMode = vk::SharingMode::eExclusive;

                vk::ResultValue<vk::Buffer> result_buffer = vk_device.createBuffer(createInfo);

                if (result_buffer.result != vk::Result::eSuccess) {
                    printf("Error: error creating buffer\n");
                    throw;
                } 

                out_buffer = result_buffer.value;

                vk::MemoryRequirements memoryRequirements = vk_device.getBufferMemoryRequirements(out_buffer);

                vk::MemoryAllocateInfo allocInfo;
                allocInfo.allocationSize  = memoryRequirements.size;
                allocInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits, properties);

                vk::ResultValue<vk::DeviceMemory> result_deviceMemory = vk_device.allocateMemory(allocInfo);

                if (result_deviceMemory.result != vk::Result::eSuccess) {
                    printf("Error: error allocating buffer\n");
                    throw;
                }

                out_bufferMemory = result_deviceMemory.value;

                vk::Result result_bind = vk_device.bindBufferMemory(out_buffer, out_bufferMemory, 0);

                if (result_bind != vk::Result::eSuccess) {
                    printf("Error: error binding buffer\n");
                    throw;
                } 
            }

            vk::CommandBuffer startCopyBuffer() {
                vk::CommandBufferAllocateInfo allocInfo;
                allocInfo.level              = vk::CommandBufferLevel::ePrimary;
                allocInfo.commandPool        = vk_commandPool;
                allocInfo.commandBufferCount = 1;

                vk::ResultValue<std::vector<vk::CommandBuffer>> result_commandBuffer = vk_device.allocateCommandBuffers(allocInfo);

                if (result_commandBuffer.result != vk::Result::eSuccess) {
                    printf("Error: error allocating command buffer\n");
                    throw;
                }

                vk::CommandBufferBeginInfo beginInfo;
                beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

                vk::Result result_begin = result_commandBuffer.value.front().begin(beginInfo);

                if (result_begin != vk::Result::eSuccess) {
                    printf("Error: error beginning command buffer\n");
                    throw;
                }

                return result_commandBuffer.value.front();
            }

            void copyBuffer(vk::Buffer src, vk::Buffer dst, vk::DeviceSize size, vk::CommandBuffer commandBuffer, vk::BufferCopy& copyRegion) {
                copyRegion.size = size;
                copyRegion.srcOffset = 0; 
                copyRegion.dstOffset = 0; 

                commandBuffer.copyBuffer(src, dst, 1, &copyRegion);
            }

            void endCopyBuffer(vk::CommandBuffer commandBuffer) {
                vk::Result result_end = commandBuffer.end();

                if (result_end != vk::Result::eSuccess) {
                    printf("Error: error allocating command buffer\n");
                    throw;
                }

                vk::SubmitInfo submitInfo;
                submitInfo.commandBufferCount = 1;
                submitInfo.pCommandBuffers    = &commandBuffer;

                vk::Result result_submit = vk_graphicsQueue.submit(1, &submitInfo, nullptr);

                if (result_submit != vk::Result::eSuccess) {
                    printf("Error: error submitting queue\n");
                    throw;
                }

                vk::Result result_waitIdle = vk_graphicsQueue.waitIdle();

                if (result_waitIdle != vk::Result::eSuccess) {
                    printf("Error: error waiting idle on queue\n");
                    throw;
                }

                vk_device.freeCommandBuffers(vk_commandPool, 1, &commandBuffer);
            }

#pragma region StagingBuffer

            template <typename T>
            void InstanceStagingBuffer(
                std::vector<T>    dataToUpload,
                vk::Buffer&       out_buffer,
                vk::DeviceMemory& out_memory,
                vk::DeviceSize&   out_size
            ) {
                
                out_size = sizeof(dataToUpload[0]) * dataToUpload.size();

                createBuffer(
                    out_size, 
                    vk::BufferUsageFlagBits::eTransferSrc, 
                    vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                    out_buffer,
                    out_memory
                );

                vk::ResultValue<void*> result_memory = vk_device.mapMemory(out_memory, 0, out_size);

                if (result_memory.result != vk::Result::eSuccess) {
                    printf("Error: error mapping staging buffer\n");
                    throw;
                } 

                memcpy(result_memory.value, dataToUpload.data(), out_size);

                vk_device.unmapMemory(out_memory);
            }

#pragma region Vertex buffer

            void createVertexBuffer() {

                vk::DeviceSize   bufferSize;
                vk::Buffer       vk_stagingBuffer;
                vk::DeviceMemory vk_stagingBufferMemory;

                InstanceStagingBuffer(vertices, vk_stagingBuffer, vk_stagingBufferMemory, bufferSize);

                createBuffer(
                    bufferSize, 
                    vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, 
                    vk::MemoryPropertyFlagBits::eDeviceLocal,
                    vk_vertexBuffer,
                    vk_vertexBufferMemory
                );

                vk::CommandBuffer commandBuffer = startCopyBuffer();
                vk::BufferCopy copyRegion;

                copyBuffer(vk_stagingBuffer, vk_vertexBuffer, bufferSize, commandBuffer, copyRegion);

                endCopyBuffer(commandBuffer);

                vk_device.destroyBuffer(vk_stagingBuffer);
                vk_device.freeMemory(vk_stagingBufferMemory);
            }

#pragma region Index buffer

            void createIndexBuffer() {

                vk::DeviceSize   bufferSize;
                vk::Buffer       vk_stagingBuffer;
                vk::DeviceMemory vk_stagingBufferMemory;

                InstanceStagingBuffer(indices, vk_stagingBuffer, vk_stagingBufferMemory, bufferSize);

                createBuffer(
                    bufferSize, 
                    vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, 
                    vk::MemoryPropertyFlagBits::eDeviceLocal,
                    vk_indexBuffer,
                    vk_indexBufferMemory
                );

                vk::CommandBuffer commandBuffer = startCopyBuffer();
                vk::BufferCopy copyRegion;

                copyBuffer(vk_stagingBuffer, vk_indexBuffer, bufferSize, commandBuffer, copyRegion);

                endCopyBuffer(commandBuffer);

                vk_device.destroyBuffer(vk_stagingBuffer);
                vk_device.freeMemory(vk_stagingBufferMemory);
            }

#pragma region Command buffer

            void createCommandBuffers() {
                vk::CommandBufferAllocateInfo allocInfo;
                allocInfo.commandPool        = vk_commandPool;
                allocInfo.level              = vk::CommandBufferLevel::ePrimary;
                allocInfo.commandBufferCount = MAX_FRAMES_IN_FLIGHT;

                vk_commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
                
                vk::ResultValue<std::vector<vk::CommandBuffer>> result = vk_device.allocateCommandBuffers(allocInfo);
                    
                if (result.result != vk::Result::eSuccess) {
                    printf("Error: error creating command buffers\n");
                    throw;
                } 

                vk_commandBuffers = result.value;
            }

#pragma region Sync objects

            void createSyncObj() {
                vk::SemaphoreCreateInfo semaphoreInfo;
                vk::FenceCreateInfo fenceInfo;
                fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;

                vk_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
                vk_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
                vk_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

                for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
                {
                    vk::ResultValue<vk::Semaphore> result_imageAvailable = vk_device.createSemaphore(semaphoreInfo);
                    vk::ResultValue<vk::Semaphore> result_renderFinished = vk_device.createSemaphore(semaphoreInfo);
                    vk::ResultValue<vk::Fence>     result_inFlightFence  = vk_device.createFence(fenceInfo);

                    if (result_imageAvailable.result != vk::Result::eSuccess ||
                        result_renderFinished.result != vk::Result::eSuccess ||
                        result_inFlightFence.result  != vk::Result::eSuccess) {

                        printf("Error: error creating sync objects\n");
                        throw;
                    } 

                    vk_imageAvailableSemaphores[i] = result_imageAvailable.value;
                    vk_renderFinishedSemaphores[i] = result_renderFinished.value;
                    vk_inFlightFences[i]           = result_inFlightFence.value;
                }
            }

#pragma region Descriptor set

            void createDescriptorPool() {
                vk::DescriptorPoolSize poolSize;
                poolSize.type            = vk::DescriptorType::eUniformBuffer;
                poolSize.descriptorCount = 1;

                vk::DescriptorPoolCreateInfo createInfo;
                createInfo.poolSizeCount = 1;
                createInfo.pPoolSizes    = &poolSize;
                createInfo.maxSets       = 1;
                
                vk::ResultValue<vk::DescriptorPool> result = vk_device.createDescriptorPool(createInfo);

                if (result.result != vk::Result::eSuccess) {
                    printf("Error: error creating descriptor pool\n");
                    throw;
                } 

                vk_descriptorPool = result.value;
            }

            void createDescriptorSet() {
                vk::DescriptorSetAllocateInfo allocInfo;
                allocInfo.descriptorPool     = vk_descriptorPool;
                allocInfo.descriptorSetCount = 1;
                allocInfo.pSetLayouts        = &vk_descriptorSetLayout;

                vk::ResultValue<std::vector<vk::DescriptorSet>> result = vk_device.allocateDescriptorSets(allocInfo);

                if (result.result != vk::Result::eSuccess) {
                    printf("Error: error creating descriptor pool\n");
                    throw;
                }

                vk_descriptorSet = result.value.front();
            }

#pragma region Setup

            void setupRenderpassBeginInfo() {
                vk_renderpassBeginInfo.renderPass          = vk_renderpass;
                vk_renderpassBeginInfo.renderArea.offset.x = 0;
                vk_renderpassBeginInfo.renderArea.offset.y = 0;
                vk_renderpassBeginInfo.renderArea.extent   = vk_swapchainExtent;
                vk_renderpassBeginInfo.clearValueCount     = 1;
                vk_renderpassBeginInfo.pClearValues        = &vk_clearColor;
            }

            void setupViewport() {
                vk_viewport.x        = 0.0f;
                vk_viewport.y        = 0.0f;
                vk_viewport.width    = vk_swapchainExtent.width;
                vk_viewport.height   = vk_swapchainExtent.height;
                vk_viewport.minDepth = 0.0f;
                vk_viewport.maxDepth = 1.0f;
            }

            void setupScissor() {
                vk_scissor.offset.x = 0;
                vk_scissor.offset.y = 0;
                vk_scissor.extent   = vk_swapchainExtent;
            }

            void setupSubmitInfo() {
                vk_waitStages = {vk::PipelineStageFlagBits::eColorAttachmentOutput};

                vk_submitInfo.waitSemaphoreCount   = 1;
                vk_submitInfo.pWaitDstStageMask    = &vk_waitStages;
                vk_submitInfo.commandBufferCount   = 1;
                vk_submitInfo.signalSemaphoreCount = 1;
            }

            void setupPresentInfo() {
                vk_presentInfo.waitSemaphoreCount = 1;
                vk_presentInfo.swapchainCount     = 1;
                vk_presentInfo.pSwapchains        = &vk_swapchain;
            }

#pragma region Cleanup functions

            void cleanupFrameBuffers() {
                for (int i = 0; i < vk_swapchainFramebuffers.size(); i++) {
                    vk_device.destroyFramebuffer(vk_swapchainFramebuffers[i]);
                }
                vk_swapchainFramebuffers.clear();
            }

            void cleanupSyncObj() {
                for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
                    vk_device.destroySemaphore(vk_imageAvailableSemaphores[i]);
                    vk_device.destroySemaphore(vk_renderFinishedSemaphores[i]);
                    vk_device.destroyFence(vk_inFlightFences[i]);
                }

                vk_imageAvailableSemaphores.clear();
                vk_renderFinishedSemaphores.clear();
                vk_inFlightFences.clear();
            }

            void cleanupImageViews() {
                for (int i = 0; i < vk_swapchainImageViews.size(); i++) {
                    vk_device.destroyImageView(vk_swapchainImageViews[i]);
                }
                vk_swapchainImageViews.clear();
            }

            void cleanupVertexAndIndexBuffers() {
                if (vk_vertexBufferMemory) {
                    vk_device.freeMemory(vk_vertexBufferMemory);
                    vk_device.destroyBuffer(vk_vertexBuffer);
                }

                if (vk_indexBufferMemory) {
                    vk_device.freeMemory(vk_indexBufferMemory);
                    vk_device.destroyBuffer(vk_indexBuffer);
                }
            }

#pragma region Swapchain recreate

            void recreateSwapchain() {

                vk::Result result = vk_device.waitForFences(vk_inFlightFences.size(), vk_inFlightFences.data(), true, UINT64_MAX);

                if (result != vk::Result::eSuccess) {
                    printf("Error: error waiting for fences\n");
                    throw;
                } 

                cleanupImageViews();
                cleanupFrameBuffers();
                vk_device.destroySwapchainKHR(vk_swapchain);

                createSwapchain();
                createImageViews();
                createFramebuffers();

                setupRenderpassBeginInfo();
                setupViewport();
                setupScissor();

                printf("recreated swapchain\n");
            }
    };
}