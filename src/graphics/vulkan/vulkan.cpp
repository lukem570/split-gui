#include <splitgui/lib.h>
#include <splitgui/graphics.hpp>
#include <splitgui/window.hpp>
#include <vector>
#include <tuple>
#include <string>
#include <array>
#include "../../path.cpp"

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

            void submitWindow(SplitGui::Window& window) override { // need to add physical device surface support
                pWindow = &window;
                createSurface(window);
                createSwapchain();
                createImageViews();
                createRenderpass();
                createDescriptorSet();
                createPipelineLayout();
                createPipeline();
            }

        protected:
            SplitGui::Window*               pWindow;
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
            vk::DescriptorSetLayout     vk_descriptorSetLayout;
            vk::PipelineLayout          vk_pipelineLayout;
            vk::Pipeline                vk_pipeline;
            std::vector<vk::Image>      vk_swapchainImages;
            std::vector<vk::ImageView>  vk_swapchainImageViews;
            unsigned int                graphicsQueueFamilyIndex = -1;
            std::vector<const char *>   enabled_layers;
            std::vector<const char *>   enabled_instance_extensions;
            std::vector<const char *>   enabled_device_extensions;

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

                if (VALIDATION) {
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
                            "vkCreateInstance Failure");
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

            void createSurface(SplitGui::Window& window) {

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

                vk::ResultValue<uint32_t> isSupported = vk_physicalDevice.getSurfaceSupportKHR(graphicsQueueFamilyIndex, vk_surface);
                if (isSupported.value == 0) {
                    printf("Error: The physical device does not support the surface\n");
                    //throw;
                }
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

            void createDescriptorSet() {
                vk::DescriptorSetLayoutBinding uboLayerBinding;
                uboLayerBinding.binding            = 0;
                uboLayerBinding.descriptorType     = vk::DescriptorType::eUniformBuffer;
                uboLayerBinding.descriptorCount    = 1;
                uboLayerBinding.stageFlags         = vk::ShaderStageFlagBits::eVertex;
                uboLayerBinding.pImmutableSamplers = nullptr;

                vk::DescriptorSetLayoutBinding samplerLayoutBinding;
                samplerLayoutBinding.binding            = 0;
                samplerLayoutBinding.descriptorType     = vk::DescriptorType::eCombinedImageSampler;
                samplerLayoutBinding.descriptorCount    = 1;
                samplerLayoutBinding.stageFlags         = vk::ShaderStageFlagBits::eFragment;
                samplerLayoutBinding.pImmutableSamplers = nullptr;

                std::array<vk::DescriptorSetLayoutBinding, 2> bindings = {uboLayerBinding, samplerLayoutBinding};

                vk::DescriptorSetLayoutCreateInfo createInfo;
                createInfo.bindingCount = bindings.size();
                createInfo.pBindings    = bindings.data();
                
                vk::ResultValue<vk::DescriptorSetLayout> result = vk_device.createDescriptorSetLayout(createInfo);

                if (result.result != vk::Result::eSuccess) {
                    printf("Error: error creating descriptor set layout");
                    throw;
                }

                vk_descriptorSetLayout = result.value;
            }

            void createPipelineLayout() {
                vk::PipelineLayoutCreateInfo createInfo;
                createInfo.setLayoutCount = 1;
                createInfo.pSetLayouts    = &vk_descriptorSetLayout;

                vk::ResultValue<vk::PipelineLayout> result = vk_device.createPipelineLayout(createInfo);

                if (result.result != vk::Result::eSuccess) {
                    printf("Error: error creating pipeline layout");
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

                vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
                
                vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
                inputAssembly.topology               = vk::PrimitiveTopology::eTriangleList;
                inputAssembly.primitiveRestartEnable = vk::False;

                vk::PipelineViewportStateCreateInfo viewport;
                viewport.viewportCount = 1;
                viewport.scissorCount  = 1;

                vk::PipelineRasterizationStateCreateInfo rasterizer;
                rasterizer.depthClampEnable        = vk::False;
                rasterizer.rasterizerDiscardEnable = vk::False;
                rasterizer.polygonMode             = vk::PolygonMode::eFill;
                rasterizer.lineWidth               = 1.0f;
                rasterizer.cullMode                = vk::CullModeFlagBits::eBack;
                rasterizer.frontFace               = vk::FrontFace::eClockwise;

                vk::PipelineMultisampleStateCreateInfo multisampling;
                multisampling.sampleShadingEnable  = vk::False;
                multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;

                vk::PipelineColorBlendAttachmentState colorBlendAttachment;
                colorBlendAttachment.blendEnable         = vk::False;
                colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eOne;
                colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
                colorBlendAttachment.colorBlendOp        = vk::BlendOp::eAdd;
                colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
                colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
                colorBlendAttachment.alphaBlendOp        = vk::BlendOp::eAdd;
                colorBlendAttachment.colorWriteMask     |= vk::ColorComponentFlagBits::eR;
                colorBlendAttachment.colorWriteMask     |= vk::ColorComponentFlagBits::eG;
                colorBlendAttachment.colorWriteMask     |= vk::ColorComponentFlagBits::eB;
                colorBlendAttachment.colorWriteMask     |= vk::ColorComponentFlagBits::eA;

                vk::PipelineColorBlendStateCreateInfo colorBlending;
                colorBlending.logicOpEnable   = vk::False;
                colorBlending.attachmentCount = 1;
                colorBlending.pAttachments    = &colorBlendAttachment;

                vk::GraphicsPipelineCreateInfo pipelineInfo;
                pipelineInfo.stageCount          = shaderStages.size();
                pipelineInfo.pStages             = shaderStages.data();
                pipelineInfo.pVertexInputState   = &vertexInputInfo;
                pipelineInfo.pInputAssemblyState = &inputAssembly;
                pipelineInfo.pVertexInputState   = &vertexInputInfo;
                pipelineInfo.pRasterizationState = &rasterizer;
                pipelineInfo.pMultisampleState   = &multisampling;
                pipelineInfo.pColorBlendState    = &colorBlending;
                pipelineInfo.layout              = vk_pipelineLayout;
                pipelineInfo.renderPass          = vk_renderpass;
                pipelineInfo.subpass             = 0;

                vk::ResultValue<vk::Pipeline> result = vk_device.createGraphicsPipeline(nullptr, pipelineInfo);

                if (result.result != vk::Result::eSuccess) {
                    printf("Error: error creating pipeline");
                    throw;
                } 

                vk_pipeline = result.value;
            }
    };
}