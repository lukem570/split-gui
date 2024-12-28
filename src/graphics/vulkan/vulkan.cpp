#include "../interface.hpp"
#include "../../path.cpp"

#ifdef DEV
#define VALIDATION true
#else
#define VALIDATION false
#endif





std::vector<const char*> instanceExtensions = {
    VK_KHR_SURFACE_EXTENSION_NAME,  
#ifdef _WIN32  
    VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif defined(__WAYLAND__)
    VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME,
#elif defined(__USE_XCB__)
    VK_KHR_XCB_SURFACE_EXTENSION_NAME,
#elif defined(__USE_XLIB__)
    VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
#elif defined(__APPLE__)
    VK_KHR_MACOS_SURFACE_EXTENSION_NAME
#else
    #error "Unknown platform, no suitable surface extension defined"
#endif
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
                createSwapchain();
                createImageViews();
                createRenderpass();
                createPipeline();
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

            void instanceVulkan() {
                vk::ApplicationInfo appInfo;
                appInfo.pApplicationName   = "Split-gui";
                appInfo.applicationVersion = 0;
                appInfo.pEngineName        = "Split-gui";
                appInfo.engineVersion      = 0;
                appInfo.apiVersion         = vk::ApiVersion12;

                vk::InstanceCreateInfo createInfo;
                createInfo.pApplicationInfo = &appInfo;

                if (VALIDATION) {
                    const char* layers[] = {
                        "VK_LAYER_KHRONOS_validation"
                    };

                    createInfo.enabledLayerCount   = 1;
                    createInfo.ppEnabledLayerNames = layers;
                }

                createInfo.setEnabledExtensionCount(static_cast<uint32_t>(instanceExtensions.size()));
                createInfo.setPpEnabledExtensionNames(instanceExtensions.data());

                vk_instance = vk::createInstance(createInfo);
            }

            // Device selction could be more robust but this will do for now
            void createPhysicalDevice() { 
                std::vector<vk::PhysicalDevice> physicalDevices = vk_instance.enumeratePhysicalDevices();

                int selection = -1;
                
                for (int i = 0; i < physicalDevices.size(); i++) {
                    if (physicalDevices[i].getProperties().apiVersion < vk::ApiVersion12) {
                        continue;
                    }

                    selection = i;
                    break;
                }

                if (selection == -1) {
                    printf("ERROR: No suitable devices found for vulkan\n");
                    throw;
                }

                vk_physicalDevice = physicalDevices[selection];

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

                vk::DeviceCreateInfo deviceCreateInfo;
                deviceCreateInfo.queueCreateInfoCount = 1;
                deviceCreateInfo.pQueueCreateInfos    = &queueCreateInfo;
                deviceCreateInfo.pEnabledFeatures     = &deviceFeatures;

                vk_device = vk_physicalDevice.createDevice(deviceCreateInfo);
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
                vk_surface = window.createSurface(vk_instance);
            }

            void createSwapchain() {
                vk::SurfaceCapabilitiesKHR capabilities      = vk_physicalDevice.getSurfaceCapabilitiesKHR(vk_surface);
                std::vector<vk::SurfaceFormatKHR> formats    = vk_physicalDevice.getSurfaceFormatsKHR(vk_surface);
                std::vector<vk::PresentModeKHR> presentModes = vk_physicalDevice.getSurfacePresentModesKHR(vk_surface);

                vk_swapchainExtent = chooseSwapExtent(capabilities);
                vk_surfaceFormat   = chooseSwapSurfaceFormat(formats);
                vk_presentMode     = chooseSwapPresentMode(presentModes);

                vk::SwapchainCreateInfoKHR createInfo;
                createInfo.minImageCount         = capabilities.minImageCount + 1;
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

                vk_swapchain       = vk_device.createSwapchainKHR(createInfo);
                vk_swapchainImages = vk_device.getSwapchainImagesKHR(vk_swapchain);
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

                    vk_swapchainImageViews[i] = vk_device.createImageView(createInfo);
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

                vk_renderpass = vk_device.createRenderPass(createInfo);
            }

            inline vk::ShaderModule createShaderModule(const std::vector<char>& code) {
                vk::ShaderModuleCreateInfo createInfo;
                createInfo.codeSize = code.size();
                createInfo.pCode    = reinterpret_cast<const uint32_t*>(code.data());

                return vk_device.createShaderModule(createInfo);
            }

            void createPipeline() {

                const std::vector<char> vertexShaderFile   = readFile("shaders/vertex.spv");
                const std::vector<char> fragmentShaderFile = readFile("shaders/fragment.spv");

                vk::ShaderModule vertexShader   = createShaderModule(vertexShaderFile);
                vk::ShaderModule fragmentShader = createShaderModule(fragmentShaderFile);
            }
    };
}