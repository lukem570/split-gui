#include "vulkan.hpp"

namespace SplitGui {
    inline void VulkanInterface::createSwapchain() {
        SPLITGUI_PROFILE;

        std::vector<vk::SurfaceFormatKHR> formats      = vk_physicalDevice.getSurfaceFormatsKHR(vk_surface);
        vk::SurfaceCapabilitiesKHR        capabilities = vk_physicalDevice.getSurfaceCapabilitiesKHR(vk_surface);
        std::vector<vk::PresentModeKHR>   presentModes = vk_physicalDevice.getSurfacePresentModesKHR(vk_surface);

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

        Logger::info("Created Swapchain");
    }

    inline void VulkanInterface::createImageViews() {
        vk_swapchainImageViews.resize(vk_swapchainImages.size());

        for (unsigned int i = 0; i < vk_swapchainImageViews.size(); i++){

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

        Logger::info("Created Image Views");
    }

    Result VulkanInterface::recreateSwapchain() {
        SPLITGUI_PROFILE;

        frameMutex.lock();

        vk::Result fenceRes = vk_device.waitForFences(vk_inFlightFences.size(), vk_inFlightFences.data(), vk::True, UINT64_MAX);

        if (fenceRes != vk::Result::eSuccess) {
            return Result::eFailedToWaitForFences;
        }

        cleanupImageViews();
        cleanupScenesImageArray();
        cleanupDepthResources();
        cleanupFrameBuffers();
        
        vk_device.destroySwapchainKHR(vk_swapchain);

        createSwapchain();
        createImageViews();
        TRYR(depthRes, createDepthResources());
        TRYR(sceneRes, createScenesImageArray());
        createFramebuffers();

        updateDescriptorSets();

        setupRenderpassBeginInfo();
        setupViewport();
        setupScissor();

        frameMutex.unlock();

        Logger::info("Recreated Swapchain");

        return Result::eSuccess;
    }
}