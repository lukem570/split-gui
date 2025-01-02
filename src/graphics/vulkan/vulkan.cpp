#include <splitgui/lib.h>
#include <splitgui/graphics.hpp>
#include <splitgui/window.hpp>
#include <vector>
#include <tuple>
#include <string>
#include <array>
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
            VulkanInterface(bool validation = false) {
                vk_validation = validation;
                vk_clearColor.color = vk::ClearColorValue{0.0f, 0.0f, 0.0f, 1.0f};
            }

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
                createPipelineLayout();
                createPipeline();
                createFramebuffers();
                createCommandPool();
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

            void drawFrame() override {
                vk::Result waitResult = vk_device.waitForFences(1, &vk_inFlightFences[currentFrame], vk::True, UINT64_MAX);

                if (waitResult != vk::Result::eSuccess) {
                    printf("Error Waiting for fences\n");
                    throw;
                }

                printf("frame\n");
                vk::ResultValue<uint32_t> result = vk_device.acquireNextImageKHR(vk_swapchain, UINT64_MAX, vk_imageAvailableSemaphores[currentFrame], nullptr);

                if (result.result != vk::Result::eSuccess) {
                    if (result.result == vk::Result::eErrorOutOfDateKHR) {
                        recreateSwapchain();
                        return;
                    }
                
                    printf("Error getting next swapchain image\n");
                    throw;
                }

                vk::Result resetResult = vk_device.resetFences(1, &vk_inFlightFences[currentFrame]);

                if (resetResult != vk::Result::eSuccess) {
                    printf("Error resetting fences\n");
                    throw;
                }

                imageIndex = result.value;

                vk_commandBuffers[currentFrame].reset();
                vk::Result beginResult = vk_commandBuffers[currentFrame].begin(vk_beginInfo);

                if (beginResult != vk::Result::eSuccess) {
                    printf("Error beginning command buffer\n");
                    throw;
                }

                vk_renderpassBeginInfo.framebuffer = vk_swapchainFramebuffers[imageIndex];
                
                vk_commandBuffers[currentFrame].beginRenderPass(vk_renderpassBeginInfo, vk::SubpassContents::eInline);
                vk_commandBuffers[currentFrame].bindPipeline(vk::PipelineBindPoint::eGraphics, vk_pipeline);
                //vk_commandBuffers[currentFrame].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, vk_pipelineLayout, 0, 1, &vk_descriptorSet, 0, nullptr);

                vk_commandBuffers[currentFrame].setViewport(0, 1, &vk_viewport);
                vk_commandBuffers[currentFrame].setScissor(0, 1, &vk_scissor);

                vk_commandBuffers[currentFrame].draw(vertCount, 1, 0, 0);

                vk_commandBuffers[currentFrame].endRenderPass();

                vk::Result endResult = vk_commandBuffers[currentFrame].end();

                if (endResult != vk::Result::eSuccess) {
                    printf("Error ending command buffer\n");
                    throw;
                }

                vk_submitInfo.pWaitSemaphores   = &vk_imageAvailableSemaphores[currentFrame];
                vk_submitInfo.pCommandBuffers   = &vk_commandBuffers[currentFrame];
                vk_submitInfo.pSignalSemaphores = &vk_renderFinishedSemaphores[currentFrame];

                vk::Result graphicsResult = vk_graphicsQueue.submit(1, &vk_submitInfo, vk_inFlightFences[currentFrame]);

                if (graphicsResult != vk::Result::eSuccess) {
                    printf("Error sending command to gpu\n");
                    throw;
                }

                vk_presentInfo.pWaitSemaphores = &vk_renderFinishedSemaphores[currentFrame];
                vk_presentInfo.pImageIndices   = &imageIndex;

                vk::Result presentResult = vk_presentQueue.presentKHR(&vk_presentInfo);

                if (presentResult != vk::Result::eSuccess) {
                    if (presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR) {
                        recreateSwapchain();
                        return;
                    }
                
                    printf("Error getting next swapchain image\n");
                    throw;
                }

                currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
            }

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
            vk::PipelineLayout              vk_pipelineLayout;
            vk::Pipeline                    vk_pipeline;
            vk::CommandPool                 vk_commandPool;
            vk::ClearValue                  vk_clearColor;
            vk::RenderPassBeginInfo         vk_renderpassBeginInfo;
            vk::Viewport                    vk_viewport;
            vk::DescriptorPool              vk_descriptorPool;
            vk::DescriptorSet               vk_descriptorSet;
            vk::Rect2D                      vk_scissor;
            std::vector<vk::CommandBuffer>  vk_commandBuffers;
            std::vector<vk::Framebuffer>    vk_swapchainFramebuffers;
            std::vector<vk::Image>          vk_swapchainImages;
            std::vector<vk::ImageView>      vk_swapchainImageViews;
            std::vector<vk::Semaphore>      vk_imageAvailableSemaphores;
            std::vector<vk::Semaphore>      vk_renderFinishedSemaphores;
            std::vector<vk::Fence>          vk_inFlightFences;
            unsigned int                    graphicsQueueFamilyIndex = -1;
            unsigned int                    presentQueueFamilyIndex = -1;
            std::vector<const char *>       enabled_layers;
            std::vector<const char *>       enabled_instance_extensions;
            std::vector<const char *>       enabled_device_extensions;
            bool                            vk_validation;

            // runtime variables (here to prevent excess push and pop operations)
            vk::CommandBufferBeginInfo      vk_beginInfo;
            vk::SubmitInfo                  vk_submitInfo;
            vk::PresentInfoKHR              vk_presentInfo;
            unsigned int                    currentFrame = 0;
            uint32_t                        imageIndex = -1;
            uint32_t                        vertCount = 3;
            

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
                    }

                    else {
                        printf(
                            "vkEnumerateInstanceLayerProperties failed to find required validation layer.\n\n"
                            "Please look at the Getting Started guide for additional information.\n",
                            "vkCreateInstance Failure");
                        throw;
                    }
                }

                auto instance_extensions_return = vk::enumerateInstanceExtensionProperties();

                vk::Bool32 surfaceExtFound = VK_FALSE;
                vk::Bool32 platformSurfaceExtFound = VK_FALSE;
                bool portabilityEnumerationActive = false;

                for (const auto &extension : instance_extensions_return.value) {
                    if (!strcmp(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, extension.extensionName)) {
                        enabled_instance_extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
                    } else if (!strcmp(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME, extension.extensionName)) {
                        // We want cube to be able to enumerate drivers that support the portability_subset extension, so we have to enable the
                        // portability enumeration extension.
                        portabilityEnumerationActive = true;
                        enabled_instance_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
                    } else if (!strcmp(VK_KHR_SURFACE_EXTENSION_NAME, extension.extensionName)) {
                        surfaceExtFound = 1;
                        enabled_instance_extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
                    } else if (!strcmp(VK_KHR_WM_SURFACE_EXTENSION_NAME, extension.extensionName)) {
                        platformSurfaceExtFound = 1;
                        enabled_instance_extensions.push_back(VK_KHR_WM_SURFACE_EXTENSION_NAME);
                    }
                }

                if (!surfaceExtFound) {
                    printf( "vkEnumerateInstanceExtensionProperties failed to find the " VK_KHR_SURFACE_EXTENSION_NAME
                            " extension.\n\n"
                            "Do you have a compatible Vulkan installable client driver (ICD) installed?\n"
                            "Please look at the Getting Started guide for additional information.\n",
                            "vkCreateInstance Failure\n");
                    throw;
                }

                if (!platformSurfaceExtFound) {
                    printf( "vkEnumerateInstanceExtensionProperties failed to find the " VK_KHR_WM_SURFACE_EXTENSION_NAME
                            " extension.\n\n"
                            "Do you have a compatible Vulkan installable client driver (ICD) installed?\n"
                            "vkCreateInstance Failure\n");
                    throw;
                }

                vk::ApplicationInfo appInfo;
                appInfo.pApplicationName   = "Split-gui";
                appInfo.applicationVersion = 0;
                appInfo.pEngineName        = "Split-gui";
                appInfo.engineVersion      = 0;
                appInfo.apiVersion         = VK_API_VERSION_1_0;

                vk::InstanceCreateInfo createInfo;
                createInfo.pApplicationInfo = &appInfo;
                createInfo.flags = portabilityEnumerationActive ? 
                                        vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR
                                      : static_cast<vk::InstanceCreateFlagBits>(0);

                if (vk_validation) {
                    createInfo.enabledLayerCount   = enabled_layers.size();
                    createInfo.ppEnabledLayerNames = enabled_layers.data();
                }

                createInfo.setEnabledExtensionCount  (static_cast<uint32_t>(enabled_instance_extensions.size()));
                createInfo.setPpEnabledExtensionNames(                      enabled_instance_extensions.data());

                vk::ResultValue<vk::Instance> result = vk::createInstance(createInfo);

                if (result.result == vk::Result::eErrorIncompatibleDriver) {
                    printf(
                        "Cannot find a compatible Vulkan installable client driver (ICD).\n\n"
                        "Please look at the Getting Started guide for additional information.\n",
                        "vkCreateInstance Failure\n");
                    throw;
                } else if (result.result == vk::Result::eErrorExtensionNotPresent) {
                    printf(
                        "Cannot find a specified extension library.\n"
                        "Make sure your layers path is set appropriately.\n",
                        "vkCreateInstance Failure\n");
                    throw;
                } else if (result.result != vk::Result::eSuccess) {
                    printf(
                        "vkCreateInstance failed.\n\n"
                        "Do you have a compatible Vulkan installable client driver (ICD) installed?\n"
                        "Please look at the Getting Started guide for additional information.\n",
                        "vkCreateInstance Failure\n");
                    throw;
                }

                vk_instance = result.value;

                VULKAN_HPP_DEFAULT_DISPATCHER.init(vk_instance);
            }

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

            // or logical device
            void createDevice() {
                vk::DeviceQueueCreateInfo queueCreateInfo;
                queueCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
                queueCreateInfo.queueCount       = 1;
                float queuePriority              = 1.0;
                queueCreateInfo.pQueuePriorities = &queuePriority;

                vk::PhysicalDeviceFeatures deviceFeatures;
                deviceFeatures = vk_physicalDevice.getFeatures();

                vk::Bool32 swapchainExtFound = VK_FALSE;

                auto device_extension_return = vk_physicalDevice.enumerateDeviceExtensionProperties();

                for (const auto &extension : device_extension_return.value) {
                    if (!strcmp(VK_KHR_SWAPCHAIN_EXTENSION_NAME, extension.extensionName)) {
                        swapchainExtFound = 1;
                        enabled_device_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
                    } else if (!strcmp("VK_KHR_portability_subset", extension.extensionName)) {
                        enabled_device_extensions.push_back("VK_KHR_portability_subset");
                    }
                }

                if (!swapchainExtFound) {
                    printf ("vkEnumerateDeviceExtensionProperties failed to find the " VK_KHR_SWAPCHAIN_EXTENSION_NAME
                            " extension.\n\n"
                            "Do you have a compatible Vulkan installable client driver (ICD) installed?\n"
                            "Please look at the Getting Started guide for additional information.\n",
                            "vkCreateInstance Failure\n");
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

            void getQueues() {
                vk_graphicsQueue = vk_device.getQueue(graphicsQueueFamilyIndex, 0);
                vk_presentQueue  = vk_device.getQueue(presentQueueFamilyIndex , 0);
            }

            inline vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) {
                if (capabilities.currentExtent.width != UINT32_MAX) {
                    return capabilities.currentExtent;
                } else {
                    vk::Extent2D actualExtent = {(uint32_t)std::get<0>(pWindow->getWindowData()->handle->getSize()), (uint32_t)std::get<1>(pWindow->getWindowData()->handle->getSize())};
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

            void createSurface(SplitGui::Window& window) { // TODO: make cross platform

                vk::XlibSurfaceCreateInfoKHR createInfo;
                createInfo.dpy    = glfwGetX11Display();
                createInfo.window = glfwGetX11Window(*window.getWindowData()->handle);

                if (createInfo.dpy == None || createInfo.window == None) {
                    printf("Error getting window handles\n");
                    throw;
                }
                
                vk::ResultValue<vk::SurfaceKHR> result = vk_instance.createXlibSurfaceKHR(createInfo);

                if (result.result != vk::Result::eSuccess) {
                    printf("Error creating surface\n");
                    throw;
                }

                vk_surface = result.value;
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

            void createPipelineLayout() { // TODO:
                vk::PipelineLayoutCreateInfo createInfo;
                createInfo.setLayoutCount         = 0;
                createInfo.pSetLayouts            = nullptr;
                createInfo.pushConstantRangeCount = 0;

                vk::ResultValue<vk::PipelineLayout> result = vk_device.createPipelineLayout(createInfo);

                if (result.result != vk::Result::eSuccess) {
                    printf("Error: error creating pipeline layout\n");
                    throw;
                } 

                vk_pipelineLayout = result.value;
            }

            void createPipeline() {

                const std::vector<char> vertexShaderFile   = readFile("../shaders/vertex.spv");
                const std::vector<char> fragmentShaderFile = readFile("../shaders/fragment.spv");

                vk::ShaderModule vertexShader   = createShaderModule(vertexShaderFile);
                vk::ShaderModule fragmentShader = createShaderModule(fragmentShaderFile);

                vk::PipelineShaderStageCreateInfo vertexCreateInfo;
                vertexCreateInfo.stage  = vk::ShaderStageFlagBits::eVertex;
                vertexCreateInfo.module = vertexShader;
                vertexCreateInfo.pName  = "main";

                vk::PipelineShaderStageCreateInfo fragmentCreateInfo;
                fragmentCreateInfo.stage  = vk::ShaderStageFlagBits::eFragment;
                fragmentCreateInfo.module = fragmentShader;
                fragmentCreateInfo.pName  = "main";

                std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages = {vertexCreateInfo, fragmentCreateInfo};

                std::vector<vk::DynamicState> dynamicStates = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};

                vk::PipelineDynamicStateCreateInfo dynamicState;
                dynamicState.dynamicStateCount = dynamicStates.size();
                dynamicState.pDynamicStates    = dynamicStates.data();

                vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
                vertexInputInfo.vertexBindingDescriptionCount   = 0;
                vertexInputInfo.vertexAttributeDescriptionCount = 0;
                
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
                pipelineInfo.layout              = vk_pipelineLayout;
                pipelineInfo.renderPass          = vk_renderpass;
                pipelineInfo.subpass             = 0;
                pipelineInfo.basePipelineHandle  = nullptr;
                pipelineInfo.basePipelineIndex   = -1;

                vk::ResultValue<vk::Pipeline> result = vk_device.createGraphicsPipeline(nullptr, pipelineInfo);

                if (result.result != vk::Result::eSuccess) {
                    printf("Error: error creating pipeline\n");
                    throw;
                } 

                vk_device.destroyShaderModule(fragmentShader);
                vk_device.destroyShaderModule(vertexShader);

                vk_pipeline = result.value;
            }

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
                vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};

                vk_submitInfo.waitSemaphoreCount   = 1;
                vk_submitInfo.pWaitDstStageMask    = waitStages;
                vk_submitInfo.commandBufferCount   = 1;
                vk_submitInfo.signalSemaphoreCount = 1;
            }

            void setupPresentInfo() {
                vk_presentInfo.waitSemaphoreCount = 1;
                vk_presentInfo.swapchainCount     = 1;
                vk_presentInfo.pSwapchains        = &vk_swapchain;
            }

            void recreateSwapchain() {

                // TODO:


                setupRenderpassBeginInfo();
                setupViewport();
                setupScissor();
            }
    };
}