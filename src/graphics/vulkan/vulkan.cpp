#include "../interface.hpp"
#include "../../path.cpp"

#include <thread>

#ifdef DEV
#define VALIDATION true
#else
#define VALIDATION false
#endif

std::vector<const char*> instanceExtensions = {
    VK_KHR_SURFACE_EXTENSION_NAME,  
    VK_KHR_WM_SURFACE_EXTENSION_NAME,
};

namespace SplitGui {

    class VulkanInterface : GraphicsLibInterface {
        public:
            void instance() override {
                instanceVulkan();
                createPhysicalDevice();
                getQueueFamilies();
                createDevice();
            }

            void submitWindow(glfw::Window* window) override { // need to add physical device surface support
                pWindow = window;
                createSurface(*window);
                //createSwapchain();
                //createImageViews();
                //createRenderpass();
                //createPipeline();
            }

        protected:
            glfw::Window*               pWindow;
        private:
            vk::Instance                vk_instance;
            vk::PhysicalDevice          vk_physicalDevice;
            vk::Device                  vk_device;
            vk::SurfaceKHR              vk_surface;
            vk::SwapchainKHR            vk_swapchain;
            vk::SurfaceFormatKHR        vk_surfaceFormat;
            vk::PresentModeKHR          vk_presentMode;
            vk::Extent2D                vk_swapchainExtent;
            vk::RenderPass              vk_renderpass;
            std::vector<vk::Image>      vk_swapchainImages;
            std::vector<vk::ImageView>  vk_swapchainImageViews;
            unsigned int                graphicsQueueFamilyIndex = -1;

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
                    printf(
                        "Unable to find the Vulkan runtime on the system.\n\n"
                        "This likely indicates that no Vulkan capable drivers are installed.",
                        "Installation Failure\n");
                    throw;
                }

                std::vector<const char *> enabled_layers;

                VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

                std::vector<const char *> instanceLayers = {"VK_LAYER_KHRONOS_validation"};

                vk::Bool32 validation_found = VK_FALSE;
                if (VALIDATION) {
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

                vk::ApplicationInfo appInfo;
                appInfo.pApplicationName   = "Split-gui";
                appInfo.applicationVersion = 0;
                appInfo.pEngineName        = "Split-gui";
                appInfo.engineVersion      = 0;
                appInfo.apiVersion         = VK_API_VERSION_1_0;

                vk::InstanceCreateInfo createInfo;
                createInfo.pApplicationInfo = &appInfo;

                if (VALIDATION) {
                    createInfo.enabledLayerCount   = instanceLayers.size();
                    createInfo.ppEnabledLayerNames = instanceLayers.data();
                }

                createInfo.setEnabledExtensionCount(static_cast<uint32_t>(instanceExtensions.size()));
                createInfo.setPpEnabledExtensionNames(instanceExtensions.data());

                vk::ResultValue<vk::Instance> result = vk::createInstance(createInfo);

                if (result.result == vk::Result::eErrorIncompatibleDriver) {
                    printf(
                        "Cannot find a compatible Vulkan installable client driver (ICD).\n\n"
                        "Please look at the Getting Started guide for additional information.\n",
                        "vkCreateInstance Failure");
                    throw;
                } else if (result.result == vk::Result::eErrorExtensionNotPresent) {
                    printf(
                        "Cannot find a specified extension library.\n"
                        "Make sure your layers path is set appropriately.\n",
                        "vkCreateInstance Failure");
                    throw;
                } else if (result.result != vk::Result::eSuccess) {
                    printf(
                        "vkCreateInstance failed.\n\n"
                        "Do you have a compatible Vulkan installable client driver (ICD) installed?\n"
                        "Please look at the Getting Started guide for additional information.\n",
                        "vkCreateInstance Failure");
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
                    if (queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics) {
                        graphicsQueueFamilyIndex = i;
                        break;
                    }
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

                std::vector<const char*> extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

                vk::DeviceCreateInfo deviceCreateInfo;
                deviceCreateInfo.queueCreateInfoCount    = 1;
                deviceCreateInfo.pQueueCreateInfos       = &queueCreateInfo;
                deviceCreateInfo.pEnabledFeatures        = &deviceFeatures;
                deviceCreateInfo.enabledExtensionCount   = extensions.size();
                deviceCreateInfo.ppEnabledExtensionNames = extensions.data();

                vk::ResultValue<vk::Device> result = vk_physicalDevice.createDevice(deviceCreateInfo);

                if (result.result != vk::Result::eSuccess) {
                    printf("Error creating logical device\n");
                    throw;
                }

                vk_device = result.value;

                VULKAN_HPP_DEFAULT_DISPATCHER.init(vk_device);
            }

            inline vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) {
                if (capabilities.currentExtent.width != UINT32_MAX) {
                    return capabilities.currentExtent;
                } else {
                    vk::Extent2D actualExtent = {(uint32_t)std::get<0>(pWindow->getSize()), (uint32_t)std::get<1>(pWindow->getSize())};
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

            void createSurface(glfw::Window& window) {
                vk::XlibSurfaceCreateInfoKHR createInfo;
                createInfo.dpy = glfwGetX11Display();
                createInfo.window = glfwGetX11Window(window);
                createInfo.pNext = nullptr;
                
                printf("alpha\n");
                vk::ResultValue<vk::SurfaceKHR> result = vk_instance.createXlibSurfaceKHR(createInfo);

                if (result.result != vk::Result::eSuccess) {
                    printf("Error creating surface\n");
                    throw;
                }

                vk_surface = result.value;

                vk::ResultValue<uint32_t> isSupported = vk_physicalDevice.getSurfaceSupportKHR(graphicsQueueFamilyIndex, vk_surface);
                if (isSupported.value == 0) {
                    printf("Error: The physical device does not support the surface\n");
                    //throw;
                }
            }

            void createSwapchain() {
                printf("a\n");


                vk::ResultValue<std::vector<vk::SurfaceFormatKHR>> formats      = vk_physicalDevice.getSurfaceFormatsKHR(vk_surface); printf("b\n");
                vk::ResultValue<vk::SurfaceCapabilitiesKHR>        capabilities = vk_physicalDevice.getSurfaceCapabilitiesKHR(vk_surface); printf("c\n");
                vk::ResultValue<std::vector<vk::PresentModeKHR>>   presentModes = vk_physicalDevice.getSurfacePresentModesKHR(vk_surface); printf("d\n");

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
                vk::ResultValue<std::vector<vk::Image>> result_swapchainImages = vk_device.getSwapchainImagesKHR(vk_swapchain);

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

            void createPipeline() {

                const std::vector<char> vertexShaderFile   = readFile("shaders/vertex.spv");
                const std::vector<char> fragmentShaderFile = readFile("shaders/fragment.spv");

                vk::ShaderModule vertexShader   = createShaderModule(vertexShaderFile);
                vk::ShaderModule fragmentShader = createShaderModule(fragmentShaderFile);
            }
    };
}