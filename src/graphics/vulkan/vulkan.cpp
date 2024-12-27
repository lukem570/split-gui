#include "../interface.hpp"

#ifdef DEV
#define VALIDATION true
#else
#define VALIDATION false
#endif

class VulkanInterface : GraphicsLibInterface {
    public:
        void instance() override {
            instanceVulkan();
            createPhysicalDevice();
            getQueueFamilies();
            createDevice();
        }

        void submitWindow(glfw::Window& window) override { // need to add physical device surface support
            pWindow = &window;
            createSurface(window);
            createSwapchain();
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
                const std::vector<const char*> validationLayers = {
                    "VK_LAYER_KHRONOS_validation"
                };

                createInfo.enabledLayerCount   = validationLayers.size();
                createInfo.ppEnabledLayerNames = validationLayers.data();
            }

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

            printf("Using %s\n", vk_physicalDevice.getProperties().deviceName);
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

        vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) {
            if (capabilities.currentExtent.width != UINT32_MAX) {
                return capabilities.currentExtent;
            } else {
                vk::Extent2D actualExtent = {std::get<0>(pWindow->getSize()), std::get<1>(pWindow->getSize())};
                actualExtent.width  = (((capabilities.minImageExtent.width) > ((((capabilities.maxImageExtent.width) < (actualExtent.width)) ? (capabilities.maxImageExtent.width) : (actualExtent.width)))) ? (capabilities.minImageExtent.width) : ((((capabilities.maxImageExtent.width) < (actualExtent.width)) ? (capabilities.maxImageExtent.width) : (actualExtent.width))));
                actualExtent.height = (((capabilities.minImageExtent.height) > ((((capabilities.maxImageExtent.height) < (actualExtent.height)) ? (capabilities.maxImageExtent.height) : (actualExtent.height)))) ? (capabilities.minImageExtent.height) : ((((capabilities.maxImageExtent.height) < (actualExtent.height)) ? (capabilities.maxImageExtent.height) : (actualExtent.height))));
                return actualExtent;
            }
        }

        vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) {
            for (const auto& availableFormat : availableFormats) {
                if (availableFormat.format == vk::Format::eB8G8R8A8Srgb && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                    return availableFormat;
                }
            }

            return availableFormats[0];
        }

        vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes) {
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
            createInfo.clipped               = vk::True;
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
};